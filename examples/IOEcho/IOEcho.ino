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

  Read something from Serial and echo it uppercase. At the same time,
  print a counter.

  2017-05-10 Jens Hauke <jens.hauke@4k2.de>
*/
#include <Cth.h>

void setup() {
	Serial.begin (115200);
	Serial.setTimeout(0); // do not block in Serial.readString()
	Scheduler.startLoop(loopReadwrite);
	Scheduler.start(loopCounter, 5000);
}


// Loop 1
void loopReadwrite(void) {
	while (true) {
		Scheduler.wait_available(Serial);

		// Read text. To not block other threads,
		// call Serial.setTimeout(0) before!
		String txt = Serial.readString();

		// Echo in uppercase
		txt.toUpperCase();
		Serial.print(txt);
	}
}


// Loop 2
void loopCounter(int delay_ms) {
	unsigned count;
	for (count = 0; ; count++) {
		Serial.print("Count:");
		Serial.print(count);
		Serial.println(" Your input?");

		Scheduler.delay(delay_ms);
	}
}
