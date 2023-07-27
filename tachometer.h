#include "main_header.h"

#ifndef TACHO_METER_H_
#define TACHO_METER_H_

volatile uint16_t first_left;
volatile uint16_t first_right;

volatile uint16_t period_left;
volatile uint16_t period_right;

volatile uint16_t left_count;
volatile uint16_t right_count;

void timer_A3_capture_init();

void TA3_0_IRQHandler(void);
void TA3_N_IRQHandler(void);

uint32_t get_left_rpm();
uint32_t get_right_rmp();

#endif /* TACHO_METER_H_ */
