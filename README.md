CopyThreads: A lightweight threading library (not only for microcontrollers)
============================================================================

tl;dr
Create your threads by `Cth.start(start_func, arg)`, call `Cth.yield()`
instead of blocking and run until all threads are done with `Cth.run()`.

The main idea of CopyThreads is to copy the stack to the heap when
switching to another thread. There the name "CopyThreads" comes
from.

CopyThreads is open source, hosted on GitHub
[https://github.com/jensh/CopyThreads](https://github.com/jensh/CopyThreads). While
in a very early stage with much room for improvements, it already
seams to work reliable. So, use at your own risk! It should work on
every architecture with a stack growing downwards. So far it is tested
on x86_64 and atmega328 (Arduino) only. Comments, bug reports,
improvements, pull requests are very welcome!

Motivation
----------
Usually, every thread has its own stack. As the
maximum stack size is mostly only roughly known, you have to choose
conservative plus a little more for every thread. This means, that
tiny threads and even stackless threads consume much more memory
for the stack space than necessary. You have to choose a stack size
which fits the worst case thread. In RAM constrained environments,
like micro controllers, this mostly prevent the usage of threads for
concurrent programming.


Keywords: Green Threads, cooperative threads, cooperative
multitasking, non-preempted


### Features
 * pure C core
 * small RAM overhead (about 60 bytes per thread on AVR MCUs).
 * Threads with large stacks are allowed (if needed. You should avoid).
 * Only the used part of the stack is copied (when switching the
   context). Tiny threads only require little heap memory.
 * Yield from everywhere in the calling stack.

### But
 * A context switch might be expensive (cpu time) on "big" threads.
 * References to stack memory (local variables) are only valid in the
   current thread.
 * Hard to predict overall memory usage. The heap might get very
   fragmented.

### Tips
 * Avoid to many local variables. They are hosted on the stack.
 * Avoid yielding (context switching) within a deep calling tree.
 * Save complex state on the heap and hold only a reference to it on
   the stack.


Arduino usage
-------------

Include the CopyThreads header to your project

```C++
#include <Cth.h>
```

A thread entry is a void function with at most one argument fitting inside a
void pointer. It is started with `Cth.start(athread, some_arg)` and
runs until the function returns. The thread should use ONLY non
blocking functions (e.g. `Stream.available()` or `Stream.read()` but
NOT `Stream.readString()` without `setTimeout(0)`). Time consuming or
busy waiting loops should call `Cth.yield()` to give cpu time to other
threads. `delay()` is allowed, as it implicitly calls `Cth.yield()`
when you implement `yield()` as shown below. But you should prefer
`Cth.delay()`!).

Here an example of an asynchronous blinking LED
```C++
void athread(void) {
	bool onoff = false;
	while (true) {
		digitalWrite(LED_PIN, onoff ? HIGH : LOW);
		onoff = !onoff;

		// Progress with other threads,
		// and continue in 1000ms:
		Cth.delay(1000);
	}
}
```

Waiting for input from the serial interface. This is when
`Serial.available()` returns true.

```C++
void athread(void) {
	while (true) {
		// Wait for Serial.available():
		Cth.wait_available(Serial);

		int ch = Serial.read();
		Serial.println(ch);
	}
}
```

In general, use `Cth.wait()` to wait for something:
```C++

unsigned long something = 0;

int compareWith(unsigned long arg) {
	return something == arg;
}

int buttonXPressed(void) {
	return !digitalRead(BUTTONX);
}

void athread(void) {
	…
	// Wait with a method. Here the long version of Cth.wait_available():
	Cth.wait<HardwareSerial,&HardwareSerial::available>(Serial);
	…
	// Wait with a C function. e.g. until a global var reach 5
	Cth.wait(compareWith, 5);
	…
	// Wait with a C function. e.g. until a button is pressed:
	Cth.wait(buttonXPressd);
	…
	// Dont wait, but give other threads the chance to proceed.
	// e.g. busy wait for input:
	while (!Serial.available()) Cth.yield();
	…
}

```


The `loop` should start the first thread(s) and run it(them).

```C++
void loop() {
	Cth.start(athread);
	// Cth.start(athread2);
	Cth.run();
}
```


If you use `delay()`, you should implement `yield()` and call
`Cth.yield()` within. Less expensive is the use of `Cth.delay()`
instead!

```C++
void yield() {
	Cth.yield();
}
```


### Examples

 * Examples from above [./examples/ExamplesFromReadme/ExamplesFromReadme.ino](./examples/ExamplesFromReadme/ExamplesFromReadme.ino).
 * Blink two LEDs [./examples/CTBlink/CTBlink.ino](./examples/CTBlink/CTBlink.ino).
 * Uppercase echo of the serial input while also counting [./examples/IOEcho/IOEcho.ino](./examples/IOEcho/IOEcho.ino).


Plain C
-----------

Include the CopyThreads header `cthread.h` to your project. Define a
thread with a void function with one `void *` argument. Yield with
`cth_yield()` to switch to the next thread. Start a thread with
`cth_start(athread, an_arg)`. Run all threads with `cth_run()`.

```C
#include <cthread.h>

void athread(void *arg) {
	char *name = (char *)arg;
	unsigned i;

	for (i = 0; i < 4; i++) {
		printf("Thread %s loop %u\n", name, i);
		cth_yield();
	}
}


int main(int argc, char **argv)
{
	cth_start(athread, "th1");
	cth_start(athread, "th2");

	cth_run();

	printf("All threads are done\n");
	return 0;
}
```

### Examples
 * Hello World [./examples/c/hello_world.c](./examples/c/hello_world.c).

Missing features / ToDos
------------------------
 * A scheduler. Currently all threads in state "running" are resumed
   round robin.
 * Direct context switch from current thread to the next thread
   without an intermediate longjmp to `cth_run()`.


Alternative approaches for concurrent programming
-------------------------------------------------
 * Protothread  
   Stackless threads. Just do not use the stack.  
   [https://en.wikipedia.org/wiki/Protothread](https://en.wikipedia.org/wiki/Protothread)  
   [http://dunkels.com/adam/pt/index.html](http://dunkels.com/adam/pt/index.html)

 * Split stacks  
   Grow the stack only when needed. Each function verifies that it has
   enough space in the current stack to execute.  
   [https://gcc.gnu.org/wiki/SplitStacks](https://gcc.gnu.org/wiki/SplitStacks)

 * Event-driven programming  
   Trigger a callback, when an event happens (e.g. can read from X or
   timeout 2sec expired...)  
   [https://en.wikipedia.org/wiki/Event-driven_programming](https://en.wikipedia.org/wiki/Event-driven_programming)

 * Coroutines  
   [https://en.wikipedia.org/wiki/Coroutine](https://en.wikipedia.org/wiki/Coroutine)
