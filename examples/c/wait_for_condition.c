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
 * Demonstrate the usage of cth_wait().
 *
 * 2017-05-10 Jens Hauke <jens.hauke@4k2.de>
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cthread.h>

unsigned thread2count = 0;

int compare_t2count(unsigned long arg) {
	// printf("Compare %u == %u\n", thread2count, uarg);
	return thread2count == arg;
}


void thread1(void *arg) {
	printf("Thread 1. Wait for thread2count == 3\n");
	cth_wait(compare_t2count, 3);
	printf("Thread 1 awake and done.\n");
}


void thread2(void *arg) {
	unsigned i;
	for (i = 0; i < 10; i++) {
		printf("Thread 2: %u\n", i);
		thread2count = i;
		cth_yield();
	}
}


int main(int argc, char **argv)
{
	cth_start(thread1, "th1");
	cth_start(thread2, "th2");

	cth_run();

	printf("All threads are done\n");
	return 0;
}

/*
 * Local Variables:
 *  compile-command: "make wait_for_condition && ./wait_for_condition"
 * End:
 */
