/*
 * (c) 2017-05-08 Jens Hauke <jens.hauke@4k2.de>
 */
#ifndef _CTHREAD_H_
#define _CTHREAD_H_
#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif


void cth_start(void (*start)(void *priv), void *priv);

void cth_run(void);

void cth_yield(void);


#ifdef __cplusplus
}/* extern "C" */
#endif
#endif /* _CTHREAD_H_ */
