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


template <class C, int (C::*method)(void)>
int condition(unsigned long arg) {
	C *c = (C*)arg;
	return (c->*method)();
}


class CthClass
{
public:
	static inline
	void start(void (*start)(void)) {
		cth_start((cth_thread_t)start, NULL);
	}

#if __SIZEOF_POINTER__ >= __SIZEOF_INT__
	static inline
	void start(void (*start)(int), int arg) {
		cth_start((cth_thread_t)start, (void*)arg);
	}

	static inline
	void start(void (*start)(unsigned), unsigned arg) {
		cth_start((cth_thread_t)start, (void*)arg);
	}
#endif

#if __SIZEOF_POINTER__ >= __SIZEOF_LONG__
	static inline
	void start(void (*start)(long), long arg) {
		cth_start((cth_thread_t)start, (void*)arg);
	}

	static inline
	void start(void (*start)(unsigned long), unsigned long arg) {
		cth_start((cth_thread_t)start, (void*)arg);
	}
#endif

	template <typename T>
	static inline
	void start(void (*start)(T *), T *arg) {
		cth_start((cth_thread_t)start, (void*)arg);
	}


	static inline
	void startLoop(void (*start)(void)) {
		cth_startloop(start);
	}


	static inline
	void run(void) {
		cth_run();
	}

	static inline
	void yield(void) {
		cth_yield();
	}


	static
	void delay(unsigned long ms);

	// Wait for c.available().
	// e.g.: Cth.wait_available(Serial);
	template <class C>
	static inline
	void wait_available(C &c) {
		wait<C, &C::available>(c);
	}

	// Wait for member function
	// e.g.: Cth.wait<HardwareSerial,&HardwareSerial::available>(Serial);
	template <class C, int (C::*method)(void)>
	static inline
	void wait(C &c) {
		cth_wait(condition<C,method>, (unsigned long)&c);
	}

	// Wait for condition(a_long_priv_arg).
	static inline
	void wait(int (*condition)(unsigned long priv), unsigned long priv) {
		cth_wait(condition, priv);
	}

	// Wait for condition(void).
	static inline
	void wait(int (*condition)(void)) {
		cth_wait((cth_condition_t)condition, 0);
	}

#if __SIZEOF_POINTER__ >= __SIZEOF_INT__
	// Wait for condition(int).
	static inline
	void wait(int (*condition)(int), int arg) {
		cth_wait((cth_condition_t)condition, arg);
	}

	// Wait for condition(unsigned int).
	static inline
	void wait(int (*condition)(unsigned int), unsigned int arg) {
		cth_wait((cth_condition_t)condition, arg);
	}
#endif

#if __SIZEOF_POINTER__ >= __SIZEOF_LONG__
	// Wait for condition(long).
	static inline
	void wait(int (*condition)(long), long arg) {
		cth_wait((cth_condition_t)condition, arg);
	}

	// Wait for condition(unsigned int).
	static inline
	void wait(int (*condition)(unsigned long), unsigned long arg) {
		cth_wait((cth_condition_t)condition, arg);
	}
#endif
	// Wait for condition(T *a_ptr_arg)
	template <typename T>
	static inline
	void wait(int (*condition)(T *), T *arg) {
		cth_wait((cth_condition_t)condition, (unsigned long)arg);
	}

};


class SchedulerClass : public CthClass
{
public:
	SchedulerClass();
};


extern SchedulerClass Scheduler;


/*
 * loop() hacks
 */
void loopCthMain(void);

#define loop loopCthMain

#endif /* _CTH_H_ */
