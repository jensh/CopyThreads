CopyThreads: A lightweight threading library
==============================================

tl;dr
Create your threads by `cth_start(start_func, arg)`, call `cth_yield()`
instead of blocking and run until all threads are done with `cth_run()`.

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
#include <cthread.h>
```

If you use `delay()`, you should implement `yield()` and call
`cth_yield()` within:

```C++
void yield() {
	cth_yield();
}
```

A thread is a void function with at most one argument fitting inside a
long. It is started with `cth_start(athread, some_arg)` and runs until
the function returns. The thread should use ONLY non blocking functions
(e.g. `Stream.available()` or `Stream.read()` but NOT
`Stream.readString()` without `setTimeout(0)`). Time consuming or busy
waiting loops should call `cth_yield()` to give cpu time to other
threads. `delay()` is allowed, as it implicitly calls `cth_yield()`
when you implement `yield()` as shown above.


```C++
void athread(void) {
	while (true) {
		// busy wait for input
		while (!Serial.available()) cth_yield();

		int ch = Serial.read();
		Serial.print(ch);
	}
}
```

The `loop` should start the first thread(s) and run them.

```C++
void loop() {
	cth_start(athread);
	cth_run();
}
```

### Examples

 * Blink two LEDs [./examples/CTBlink/CTBlink.ino](./examples/CTBlink/CTBlink.ino).
 * Uppercase echo of the serial input while also counting [./examples/IOEcho/IOEcho.ino](./examples/IOEcho/IOEcho.ino).


Plain C
-----------

### Examples
 * Hello World [./examples/c/hello_world.c](./examples/c/hello_world.c).

Missing features / ToDos
------------------------
 * A scheduler. Currently all threads in state "running" are resumed
   round robin.
 * Direct context switch from current thread to the next thread
   without an intermediate longjmp to `cth_run()`.
 * Build-in "`delay()`". Threads in state "sleeping" might be woken up at
   a scheduled time. This would be cheaper than doing a full context
   switch to the thread just to see, that the threat has to sleep
   longer.
 * Build-in "`wait_for()`". Sleep until a boolean function returns
   true. Check a "`wait_for()`" condition without the need to resume the
   thread. e.g. "`wait_for(Serial0_available);`" instead of an
   expensive "`while (!Serial0_available()) cth_yield();`".

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
