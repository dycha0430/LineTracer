#include "motor.h"

void pwm_init34(uint16_t period, uint16_t duty3, uint16_t duty4) {
    // CCR0 period
    TIMER_A0->CCR[0] = period;

    // divide by 1
    TIMER_A0->EX0 = 0x0000;

    // toggle/reset
    TIMER_A0->CCTL[3] = 0x0040;
    TIMER_A0->CCR[3] = duty3;
    TIMER_A0->CCTL[4] = 0x0040;
    TIMER_A0->CCR[4] = duty4;

    // 0x200 -> SMCLK
    // 0b1100 0000 => input divider / 8
    // 0b0011 0000 => up/down mode
    TIMER_A0->CTL = 0x02F0;

    // set alternative (주어지는 PWM?)
    P2->DIR |= 0xC0;
    P2->SEL0 |= 0xC0;
    P2->SEL1 &= ~0xC0;
}

void motor_init() {
    // SLP
    P3->SEL0 &= ~0xC0;  // 1100 0000 == 0xC0 (6, 7번 bit enable)
    P3->SEL1 &= ~0xC0;
    // Output으로 설정
    P3->DIR |= 0xC0;
    P3->OUT &= ~0xC0;

    // DIR
    P5->SEL0 &= ~0x30; // 0011 0000 == 0x30 (4, 5번 bit enable)
    P5->SEL1 &= ~0x30;
    P5->DIR |= 0x30;
    P5->OUT &= ~0x30;

    // PWM (Enable 의 의미...?)
    P2->SEL0 &= ~0xC0; // 1100 0000
    P2->SEL1 &= ~0xC0;
    P2->DIR |= 0xC0;
    P2->OUT &= ~0xC0;

    pwm_init34(7500, 0, 0);
}

void move(uint16_t leftDuty,  uint16_t rightDuty) {
    P3->OUT |= 0xC0;
    TIMER_A0->CCR[3] = rightDuty; // P2.6 == right motor enable
    TIMER_A0->CCR[4] = leftDuty;
}

void left_forward() {
    P5->OUT &= ~0x10;
}

void left_backward() {
    P5->OUT |= 0x10;
}

void right_forward() {
    P5->OUT &= ~0x20;
}

void right_backward() {
    P5->OUT |= 0x20;
}
