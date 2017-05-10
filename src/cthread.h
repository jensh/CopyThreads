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
#ifndef _CTHREAD_H_
#define _CTHREAD_H_
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

typedef void (*cth_thread_t)(void *priv);

void cth_start(void (*start)(void *priv), void *priv);

void cth_run(void);

void cth_yield(void);


#ifdef __cplusplus
}/* extern "C" */

static inline
void cth_start(void (*start)(void)) {
	cth_start((cth_thread_t)start, NULL);
}


static inline
void cth_start(void (*start)(int), unsigned arg) {
	cth_start((cth_thread_t)start, (void*)arg);
}

static inline
void cth_start(void (*start)(unsigned), unsigned arg) {
	cth_start((cth_thread_t)start, (void*)arg);
}

static inline
void cth_start(void (*start)(long), long arg) {
	cth_start((cth_thread_t)start, (void*)arg);
}

static inline
void cth_start(void (*start)(unsigned long), unsigned long arg) {
	cth_start((cth_thread_t)start, (void*)arg);
}

static inline
void cth_start(void (*start)(char *), char *arg) {
	cth_start((cth_thread_t)start, (void*)arg);
}

template <typename type>
static inline
void cth_start(void (*start)(type *), type *arg) {
	cth_start((cth_thread_t)start, (void*)arg);
}

#endif
#endif /* _CTHREAD_H_ */
