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
 * This is the c++ part of a lightweight threading library
 *
 * 2017-05-10 Jens Hauke <jens.hauke@4k2.de>
 */
#ifndef _CTH_H_
#define _CTH_H_

#include <stddef.h>
#include "cthread.h"

class CthClass
{
public:
	static inline
	void start(void (*start)(void)) {
		cth_start((cth_thread_t)start, NULL);
	}

	static inline
	void start(void (*start)(int), unsigned arg) {
		cth_start((cth_thread_t)start, (void*)arg);
	}

	static inline
	void start(void (*start)(unsigned), unsigned arg) {
		cth_start((cth_thread_t)start, (void*)arg);
	}

	static inline
	void start(void (*start)(long), long arg) {
		cth_start((cth_thread_t)start, (void*)arg);
	}

	static inline
	void start(void (*start)(unsigned long), unsigned long arg) {
		cth_start((cth_thread_t)start, (void*)arg);
	}

	static inline
	void start(void (*start)(char *), char *arg) {
		cth_start((cth_thread_t)start, (void*)arg);
	}

	template <typename T>
	static inline
	void start(void (*start)(T *), T *arg) {
		cth_start((cth_thread_t)start, (void*)arg);
	}

	static inline
	void run(void) {
		cth_run();
	}

	static inline
	void yield(void) {
		cth_yield();
	}
};

extern CthClass Cth;

#endif /* _CTH_H_ */
