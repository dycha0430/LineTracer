#include "main_header.h"

#ifndef MOTOR_H_
#define MOTOR_H_

typedef uint16_t speed;
extern speed FAST;
extern speed NORMAL;
extern speed SLOW;
extern speed STOP;

void right_forward();
void right_backward();
void left_backward();
void left_forward();
void move(uint16_t leftDuty,  uint16_t rightDuty);
uint32_t get_left_rpm();
void motor_init();
void pwm_init34(uint16_t period, uint16_t duty3, uint16_t duty4);

void move_forward(uint16_t speed);
void move_backward(uint16_t speed);
void stop();
void turn_left(uint16_t speed);
void turn_right(uint16_t speed);

#endif /* MOTOR_H_ */
