/*
 * (c) 2017-05-10 Jens Hauke <jens.hauke@4k2.de>
 *
 * Read something from Serial and echo it uppercase. At the same time,
 * print a counter.
 */
#include <cthread.h>

void setup() {
	Serial.begin (115200);
	Serial.setTimeout(0); // do not block in Serial.readString()
}


void yield() {
	// yield is called within delay()
	cth_yield();
}


// Thread 1
void readwrite(void) {
	while (true) {
		while (!Serial.available()) cth_yield();

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
		Serial.println(" Your input?");

		delay(delay_ms);
	}
}


void loop() {
	cth_start(readwrite);
	cth_start(counter, 5000);

	cth_run();
}
