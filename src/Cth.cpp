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
 * 2017-05-10 Jens Hauke <jens.hauke@4k2.de>
 */

#include <Arduino.h>
#include "Cth.h"

static
int millis_ge(unsigned long ms) {
	return ((long)(millis() - ms)) >= 0;
}


void CthClass::delay(unsigned long ms) {
	ms += millis();
	cth_wait(millis_ge, ms);
}

// CthClass has static methods only. No need for defining Cth or Scheduler.
// CthClass Cth;
// CthClass Scheduler;


/*
 * void yield()
 */

// Hack: Allow yield chaining
void yieldCth(void) __attribute__((weak));


// We need yield() to allow delay().
// If you need yield too, please call it yieldCth()!
void yield() {
	if (yieldCth) yieldCth();
	Scheduler.yield();
}


/*
 * void loop() Hacks
 */

// Define the original "loop" is optional when using Cth.
void loopCthMain(void) __attribute__((weak));


// Overwrite the original "loop" to call Scheduler.run().
#undef loop
void loop() {
	if (loopCthMain) Scheduler.startLoop(loopCthMain);
	if (serialEventRun) serialEventRun();
	Scheduler.run();
	// Never be here. Scheduler.run() runs forever
}
