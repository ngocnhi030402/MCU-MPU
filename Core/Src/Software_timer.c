/*
 * Software_timer.c
 *
 *  Created on: Oct 30, 2022
 *      Author: phamk
 */

#include "Software_timer.h"

extern int timer_counter[TIMER_LIMIT] = {0,0,0,0};
extern int timer_flag[TIMER_LIMIT] = {0,0,0,0};
int TIMER_CYCLE = 10;

void setTimer(int duration, int flag){
	timer_counter[flag] = duration / TIMER_CYCLE;
	timer_flag[flag]= 0;
}

void timerRun(int flag){
	if(timer_counter[flag] > 0){
		timer_counter[flag]--;

		if (timer_counter[flag] == 0)
			timer_flag[flag] = 1;
	}
}
