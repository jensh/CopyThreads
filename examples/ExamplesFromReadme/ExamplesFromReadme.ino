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

  Some example threads implementing stuff from the README.md.

  LED attached from pin 13 to ground.

  For the "button press" thread attach a push button between
  ground and pin 2.

  Input is read from and printed to Serial.

  2017-05-12 Jens Hauke <jens.hauke@4k2.de>
*/
#include <Cth.h>

#define LED_PIN 13
#define BUTTON_PIN 2

void setup() {
	Serial.begin (115200);
	Serial.setTimeout(0); // do not block in Serial.readString()

	pinMode(LED_PIN, OUTPUT);
	pinMode(BUTTON_PIN, INPUT_PULLUP);

	Scheduler.startLoop(loopBlink);
	Scheduler.startLoop(loopSerial);
	Scheduler.startLoop(loopButton);
	Scheduler.start(athreadSomething, 5);
}


void loop() {
	// Start threads
	Serial.println("MainLoop");
	Scheduler.delay(1000);
}


// Blink LED on pin LED_PIN every second.
void loopBlink(void) {
	bool onoff = false;

	digitalWrite(LED_PIN, onoff ? HIGH : LOW);

	Serial.print("LED is ");
	Serial.println(onoff ? "on" : "off");

	onoff = !onoff;

	// Progress with other threads,
		// and continue in 1000ms:
	Scheduler.delay(1000);
}


void loopSerial(void) {
	// Wait for Serial.available():
	Scheduler.wait_available(Serial);

	// Wait with a method. Here the long version of Scheduler.wait_available():
	Scheduler.wait<HardwareSerial,&HardwareSerial::available>(Serial);

	// Print as numeric value
	int ch = Serial.read();
	Serial.print("Read:");
	Serial.println(ch);
}


unsigned long buttonPressCount = 0;

int buttonPressed(void) {
	return !digitalRead(BUTTON_PIN);
}


void loopButton(void) {
	Scheduler.wait(buttonPressed);

	buttonPressCount++;
	Serial.println("ButtonPressed");

	// de-bounce 500ms
	Scheduler.delay(500);
}


void athreadPrintDelayed(const char *text) {
	Scheduler.delay(1000);
	Serial.println(text);
}


int compareButtonPressCountWith(unsigned long arg) {
	return buttonPressCount == arg;
}


void athreadSomething(int count) {
	while (true) {
		// Wait for count button presses
		Scheduler.wait(compareButtonPressCountWith, count);

		Serial.print("ButtonPressCount:");
		Serial.println(buttonPressCount);

		buttonPressCount = 0; // reset

		// Start a thread from within a thread
		Scheduler.start(athreadPrintDelayed, "Heyho");
	}
}
