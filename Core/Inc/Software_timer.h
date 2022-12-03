/*
 * Software_timer.h
 *
 *  Created on: Oct 30, 2022
 *      Author: phamk
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define TIMER_LIMIT 4

extern int timer_counter[TIMER_LIMIT];
extern int timer_flag[TIMER_LIMIT];
extern int TIMER_CYCLE;

void setTimer(int duration, int flag);
void timerRun(int flag);

#endif /* INC_SOFTWARE_TIMER_H_ */
