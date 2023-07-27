#include "led.h"

color RED = 0b00000001;
color BLUE = 0b00000100;
color GREEN = 0b00000010;
color WHITE = 0b00000111;
color YELLOW = 0b00000011;
color CYAN = 0b00000110;
color MAGENTA = 0b00000101;


void turn_on_led(int color) {
    P2->OUT = (~0x07) | color;
}

void turn_off_led() {
    P2->OUT = ~0x07;
}

void led_init() {
    // 0x07 == 0b00000111
    // Setup led as GPIO
    P2->SEL0 &= ~0x07; // 11111000
    P2->SEL1 &= ~0x07;

    // Setup led as Output
    P2->DIR |= 0x07;

    // Turn off LED
    P2->OUT &= ~0x07;
}
