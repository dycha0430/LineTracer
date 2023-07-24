#include "main_header.h"

#ifndef SENSOR_H_
#define SENSOR_H_

typedef int surface;
extern surface SF_WHITE;
extern surface SF_BLACK;

enum STATE {
    START = 0,
    DEFAULT = 1,
    NARROW = 2,
    WIDE = 3,
    NONE = 4,
};

enum DIRECTION {
    STRAIGHT = 0,
    LEFT = 1,
    RIGHT = 2,
};

void ir_sensor_init();
void turn_on_ir_led();
void turn_off_ir_led();
void charge_capacitor();
int sense_ir();
int sense_nth_ir(int ir_num);
STATE detect_state(int sensor);

#endif /* SENSOR_H_ */
