/*
 * Copyright (c) 2017 Jens Hauke. All rights reserved.
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * This is a lightweight threading library
 *
 * 2017-05-08 Jens Hauke <jens.hauke@4k2.de>
 */
#include <stdlib.h>
#include <alloca.h>
#include <string.h>
#include <setjmp.h>
#include "cthread.h"


#define CTH_STATE_START		0
#define CTH_STATE_WAITING	1
#define CTH_STATE_RUNNING	2
#define CTH_STATE_DONE		3

/*
 * Threads running
 */
typedef struct {
	unsigned state:4;
	union {
		struct {
			void (*start)(void *priv);
			void *priv;
		} start;
		struct {
			jmp_buf jmp_buf;
		} running;
	} u;
	void *stack;
} cth_running_t;


static void *cth_base_ptr = NULL;
static jmp_buf cth_jmp_buf;
static cth_running_t *cth_running = NULL;
static unsigned cth_running_n = 0;
static cth_running_t *cth_current = NULL;

typedef struct {
	int (*condition)(unsigned long priv);
	unsigned long priv;
} cth_wait_condition_t;


static inline
size_t cth_current_idx(void) {
	return (size_t)(cth_current - cth_running);
}


static inline
cth_running_t *cth_slot_allocate(void) {
	cth_running_t *cth_new;
	unsigned i;

	// Find a free slot:
	for (i = 0;; i++) {
		if (i == cth_running_n) {
			// Append
			size_t idx_current = cth_current_idx();
			cth_running_n++;
			cth_running = realloc(cth_running, sizeof(cth_running_t) * cth_running_n);

			cth_current = &cth_running[idx_current]; // Update cth_current
			cth_new = &cth_running[i];
			break;
		}
		cth_new = &cth_running[i];

		if (cth_new->state == CTH_STATE_DONE) {
			// Use free slot
			break;
		}
	}
	return cth_new;
}


static inline
void cth_slots_cleanup_and_start(void) {
	while (cth_running[cth_running_n - 1].state == CTH_STATE_DONE) {
		cth_running_n--;
		if (!cth_running_n) break;
	}
	cth_running = realloc(cth_running, sizeof(cth_running_t) * cth_running_n);

	// restart loop
	cth_current = cth_running;
}


void cth_start(void (*start)(void *priv), void *priv) {
	cth_running_t *cth_new = cth_slot_allocate();

	cth_new->state = CTH_STATE_START;
	cth_new->u.start.start = start;
	cth_new->u.start.priv = priv;
}


static
void cth_startloop_helper(void *_start) {
	void (*start)(void) = _start;
	while (1) start();
}


void cth_startloop(void (*start)(void)) {
	cth_start(cth_startloop_helper, start);
}


static inline
void _cth_exit(void) {
	free(cth_current->stack);
	cth_current->state = CTH_STATE_DONE;
}


void cth_run(void) {
	if (cth_base_ptr) return; // Already running

	cth_base_ptr = alloca(0);

	while (cth_running_n) {
		switch (cth_current->state) {
		case CTH_STATE_START:
			if (!setjmp(cth_jmp_buf)) {
				cth_current->state = CTH_STATE_RUNNING;
				cth_current->stack = NULL;
				cth_current->u.start.start(cth_current->u.start.priv);
				_cth_exit();
			}
			break;
		case CTH_STATE_WAITING: {
			cth_wait_condition_t *head = (cth_wait_condition_t *)cth_current->stack;
			if (!head->condition(head->priv)) {
				// Condition false. Keep in state CTH_STATE_WAITING
				break;
			}
			cth_current->state = CTH_STATE_RUNNING;
			// Falling through to CTH_STATE_RUNNING
		}
		case CTH_STATE_RUNNING:
			longjmp(cth_current->u.running.jmp_buf, 1);
			// never be here
			// assert(0);
		}

		cth_current++;
		if (cth_current == cth_running + cth_running_n) {
			cth_slots_cleanup_and_start();
		}
	}

	cth_base_ptr = NULL;
}


static inline
void _cth_backup_stack(char *sp, size_t stack_size) {
//	assert((void*)sp < cth_base_ptr);

	cth_current->stack = realloc(cth_current->stack, stack_size);
	// printf("#%u backup  stack of size %4u %p to   %p\n", cth_current_idx(), stack_size, sp, cth_current->stack);

	memcpy(cth_current->stack, sp, stack_size);
}


static inline
void _cth_restore_stack(char *sp, size_t stack_size) {
//	printf("#%u restore stack of size %4u %p from %p\n", cth_current_idx(), stack_size, sp, cth_current->stack);
//	volatile char x;
	memcpy(sp,
	       cth_current->stack,
	       stack_size);
//	x = *sp; // Use *sp. With -O1 gcc sometimes remove the memcpy, when sp is not used afterwards.
	__asm__ __volatile__("":: "r" (sp)); // "Use" sp
}


void cth_wait(int (*condition)(unsigned long priv), unsigned long priv) __attribute__ ((noinline));
void cth_wait(int (*condition)(unsigned long priv), unsigned long priv) {
	size_t stack_size;
	void *sp;

	if (!cth_base_ptr) return; // cth_run() not called.

	if (condition) {
		// Save condition hook at the stack head. cth_run() has access to it via cth_current->stack.
		cth_wait_condition_t *head = (cth_wait_condition_t *)alloca(sizeof(condition) + sizeof(priv));
		head->condition = condition;
		head->priv = priv;
		cth_current->state = CTH_STATE_WAITING;
	}

	sp = alloca(0);
	stack_size = (size_t)(cth_base_ptr - sp);

	_cth_backup_stack(sp, stack_size);

	if (!setjmp(cth_current->u.running.jmp_buf)) {
		longjmp(cth_jmp_buf, 1);
		// Never be here
	}

	// Recalculate sp and stack_size! (might be clobbered by longjmp)
	sp = alloca(0);
	stack_size = (size_t)(cth_base_ptr - sp);

	_cth_restore_stack(sp, stack_size);
}
