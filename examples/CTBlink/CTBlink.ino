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

  Blink Multithreaded.

  Turn an LED on for one second, then off for one second, repeatedly.
  Turn another LED on for 500 ms, then off for 500 ms, repeatedly.

  2017-05-08 Jens Hauke <jens.hauke@4k2.de>
*/
#include <Cth.h>

#define LED_PIN1 12
#define LED_PIN2 13

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin (115200);

	pinMode(LED_PIN1, OUTPUT);
	pinMode(LED_PIN2, OUTPUT);

	Scheduler.startLoop(blink1);
	Scheduler.startLoop(blink2);
}


void blink1(void) {
	Serial.print("LED1 HIGH"); Serial.println(millis());

	digitalWrite(LED_PIN1, HIGH);

	Scheduler.delay(1000);

	Serial.print("LED1 LOW "); Serial.println(millis());

	digitalWrite(LED_PIN1, LOW);

	Scheduler.delay(1000);
}


void blink2(void) {
	Scheduler.delay(250);

	Serial.print("LED2 HIGH "); Serial.println(millis());

	digitalWrite(LED_PIN2, HIGH);

	Scheduler.delay(500);

	Serial.print("LED2 LOW  "); Serial.println(millis());

	digitalWrite(LED_PIN2, LOW);

	Scheduler.delay(250);
}
