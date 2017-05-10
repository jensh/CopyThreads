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
 * A "hello world" for copy threads.
 *
 * Demonstrate the usage of
 * cth_start(), cth_yield() and cth_run().
 *
 * 2017-05-10 Jens Hauke <jens.hauke@4k2.de>
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cthread.h>


void athread(void *arg) {
	char *name = (char *)arg;
	unsigned i;
	printf("Hello World!\n");

	for (i = 0; i < 4; i++) {
		printf("Thread %s loop %u\n", name, i);

		// Start an additional thread from "th1" at i==1
		if (i == 1 && strcmp(name, "th1") == 0) {
			printf("Starting thread th3 from th1\n");
			cth_start(athread, "th3");
		}

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

/*
 * Example output:
 *
 * ./hello_world
Hello World!
Thread th1 loop 0
Hello World!
Thread th2 loop 0
Thread th1 loop 1
Starting thread th3 from th1
Thread th2 loop 1
Hello World!
Thread th3 loop 0
Thread th1 loop 2
Thread th2 loop 2
Thread th3 loop 1
Thread th1 loop 3
Thread th2 loop 3
Thread th3 loop 2
Thread th3 loop 3
All threads are done
*/

/*
 * Local Variables:
 *  compile-command: "make hello_world && ./hello_world"
 * End:
 */
