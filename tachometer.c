#include "tachometer.h"

void timer_A3_capture_init() {
   	first_right = 0;
   	first_left = 0;

   	period_left = 0;
   	period_right = 0;

   	left_count = 0;
   	right_count = 0;

    // alternative mode
    P10->SEL0 |= 0x30;
    P10->SEL1 &= ~0x30;
    P10->DIR &= ~0x30;

//     setup timer & input capture mode
    TIMER_A3->CTL &= ~0x0030; // Stop mode?
    TIMER_A3->CTL |= 0x0200; // SMCLK 클럭 소스

//    TIMER_A3->CCTL[0] &= 0x4910;
//    TIMER_A3->CCTL[1] &= 0x4910; // 이건 동작 안함..
    TIMER_A3->CCTL[0] = 0x4910;
    TIMER_A3->CCTL[1] = 0x4910;

    // (01-capture on rising edge)00 (1-sync capture)00(1-capture) 000(1-interrupt enable) 0000
    TIMER_A3->EX0 &= ~0x0007; // Divide by 1

    // configure interrupts
    NVIC->IP[3] = (NVIC->IP[3]&0x0000FFFF) | 0x40400000;
    NVIC->ISER[0] = 0x0000C000;
    TIMER_A3->CTL |= 0x0024; // continuous mode & Timer_A clear
}

void TA3_0_IRQHandler(void) {
    TIMER_A3->CCTL[0] &= ~0x0001;
    period_right = TIMER_A3->CCR[0] - first_right;
    first_right = TIMER_A3->CCR[0];
    ++right_count;
    printf("right count : %d \n", right_count);
}

void TA3_N_IRQHandler(void) {
    TIMER_A3->CCTL[1] &= ~0x0001;
    period_left = TIMER_A3->CCR[1] - first_left;
    first_left = TIMER_A3->CCR[1];
    ++left_count;
    printf("left count : %d \n", left_count);
}

uint32_t get_left_rpm() {
    return 2000000 / period_left;
}

uint32_t get_right_rpm() {
    return 2000000 / period_right;
}


