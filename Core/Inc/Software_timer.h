/*
 * Software_timer.h
 *
 *  Created on: Oct 30, 2022
 *      Author: phamk
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include <stdint.h>
#include <stdlib.h>

#define TIMER_LIMIT 4

#define TIMER_TEST_0	0
#define TIMER_AUTO_A	1
#define TIMER_AUTO_B	2
#define TIMER_SET_LONG	3
#define TIMER_PD		4
#define TIMER_DUR		5
#define TIMER_BLINK		6

extern int timer_counter[TIMER_LIMIT];
extern int timer_flag[TIMER_LIMIT];
extern int TIMER_CYCLE;

void setTimer(int duration, int flag);
void timerRun(int flag);
void timer_setDuration(int index, int duration);
int	timer_checkFlag(int index);
void timer_clear(int index);


#endif /* INC_SOFTWARE_TIMER_H_ */
