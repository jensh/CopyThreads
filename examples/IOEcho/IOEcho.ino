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
}


void yield() {
	// yield is called within delay()
	Cth.yield();
}


// Thread 1
void readwrite(void) {
	while (true) {
		Cth.wait_available(Serial);

		// Read text. To not block other threads,
		// call Serial.setTimeout(0) before!
		String txt = Serial.readString();

		// Echo in uppercase
		txt.toUpperCase();
		Serial.print(txt);
	}
}


// Thread 2
void counter(int delay_ms) {
	unsigned count;
	for (count = 0; ; count++) {
		Serial.print("Count:");
		Serial.print(count);
		Serial.print(" millis:");
		Serial.print(millis());
		Serial.println(" Your input?");

		Cth.delay(delay_ms);
	}
}


void loop() {
	Cth.start(readwrite);
	Cth.start(counter, 5000);

	Cth.run();
}
