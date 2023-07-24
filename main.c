#include "main_header.h"
#include "sensor.h"
#include "motor.h"
#include "led.h"

#define NUM_LAP 2
/**
 * main.c
 */

int lap = 0;

void init() {
    Clock_Init48MHz();
    led_init();
    ir_sensor_init();

    lap = 0;
}

void main(void)
{
    init();

    int sensor;
    STATE state = NONE, before_state = NONE;

    while (1) {
        sensor = sense_ir();
        before_state = state;
        state = detect_state(sensor);
        if (before_state == START && state != START) lap++;

        if (state == START) {
            printf("start line\n");
            if (lap == NUM_LAP) {
                // Stop
                printf("Stop! (Done)");
                break;
            }
        } else if (state == DEFAULT) {
            printf("default state\n");
            // Go Straight
        } else {
            printf("None state..\n");
        }

        turn_off_ir_led();
    }
}
