/*
  Blink Multithreaded.
  Turns on an LED on for one second, then off for one second, repeatedly.
  Turns on another LED on for 800 ms, then off for 800 ms, repeatedly.
*/
#include <cthread.h>

#define LED_PIN1 12
#define LED_PIN2 13

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin (115200);
	pinMode(LED_PIN1, OUTPUT);
	pinMode(LED_PIN2, OUTPUT);
}


void yield() {
	cth_yield();
}


void blink1(void) {
	for (unsigned i = 0; i < 10; i++) {
		Serial.print("LED1 "); Serial.println(i);
		digitalWrite(LED_PIN1, HIGH);
		delay(1000);
		digitalWrite(LED_PIN1, LOW);
		delay(1000);
	}
}


void blink2(int arg) {
	for (unsigned i = 0; i < 10; i++) {
		Serial.print("LED2 "); Serial.print(i);
		Serial.print(" pin "); Serial.println(arg);
		digitalWrite(arg, HIGH);
		delay(800);
		digitalWrite(arg, LOW);
		delay(800);
	}
}


// the loop function runs over and over again forever
void loop() {
	cth_start(blink1);
	cth_start(blink2, LED_PIN2); // int arg example

	// Run threads until all are done.
	cth_run();

	Serial.println("restart");
}
