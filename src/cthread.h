/*
 * Copyright (c) 2017 Jens Hauke. All rights reserved.
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * This is a lightweight threading library
 *
 * 2017-05-08 Jens Hauke <jens.hauke@4k2.de>
 */
#ifndef _CTHREAD_H_
#define _CTHREAD_H_
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

typedef void (*cth_thread_t)(void *priv);
typedef int (*cth_condition_t)(unsigned long priv);

void cth_start(void (*start)(void *priv), void *priv);

void cth_startloop(void (*start)(void));

void cth_run(void);

void cth_wait(int (*condition)(unsigned long priv), unsigned long priv);

static inline
void cth_yield(void) {
	cth_wait(NULL, 0);
}

#ifdef __cplusplus
}/* extern "C" */
#endif
#endif /* _CTHREAD_H_ */
