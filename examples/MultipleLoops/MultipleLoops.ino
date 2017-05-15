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

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin (115200);

	Cth.startLoop(loop1);

	Cth.startLoop(loop2);

	// And one thread only once
	Cth.start(startOnce);

}


// the loop function runs over and over again forever
void loop() {
	Serial.println("Mainloop");
	Cth.run();

	// Never be here as Cth.run() runs forever.
	Serial.println("Mainloop done");
	delay(1000);
}


void loop1(void) {
	Serial.print(millis());
	Serial.println(" Loop1");
	Cth.delay(1000);
}


void loop2(void) {
	Serial.print(millis());
	Serial.println(" Loop2");
	Cth.delay(2000);
}


void startOnce(void) {
	Serial.print(millis());
	Serial.println(" Once");
	Cth.delay(1000);
}
