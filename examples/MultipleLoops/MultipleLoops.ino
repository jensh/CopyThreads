/*
 * Copyright (c) 2017 Jens Hauke. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*

  Multiple loops.

  Show, how to start multiple loops.

  2017-05-15 Jens Hauke <jens.hauke@4k2.de>
*/
#include <Cth.h>
//#include <Scheduler.h>

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin (115200);

	Scheduler.startLoop(loop1);

	Scheduler.startLoop(loop2);

	// And one thread only once
	Scheduler.start(startOnce);

}


// the loop function runs over and over again forever
void loop() {
	Serial.print(millis());
	Serial.println(" Mainloop");

	// delay() Works, but Scheduler.delay(1000) should be preferred!
	// (delay() has more overhead when switching a context.)
	delay(1000);
}


void loop1(void) {
	Serial.print(millis());
	Serial.println(" Loop1");

	Scheduler.delay(1000);
}


void loop2(void) {
	Serial.print(millis());
	Serial.println(" Loop2");

	Scheduler.delay(2000);
}


void startOnce(void) {
	Serial.print(millis());
	Serial.println(" Once");

	Scheduler.delay(1000);

	Serial.print(millis());
	Serial.println(" Once done");
}
