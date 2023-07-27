#include "main_header.h"

#ifndef LED_H_
#define LED_H_

typedef int color;

extern color RED;
extern color BLUE;
extern color GREEN;
extern color WHITE;
extern color YELLOW;
extern color CYAN;
extern color MAGENTA;

void led_init();
void turn_on_led();
void turn_off_led();

#endif /* LED_H_ */
