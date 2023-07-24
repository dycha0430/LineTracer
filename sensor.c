#include "sensor.h"

surface SF_WHITE = 0;
surface SF_BLACK = 1;

void ir_sensor_init() {
    // 0, 2, 4, 6 IR Emitter
    P5->SEL0 &= ~-0x08;
    P5->SEL1 &= ~0x08;
    P5->DIR |= 0x08;
    P5->OUT &= ~0x08;

    // 1, 3, 5, 7 IR Emitter
    P9->SEL0 &= ~-0x04;
    P9->SEL1 &= ~0x04;
    P9->DIR |= 0x04;
    P9->OUT &= ~0x04;

    P7->SEL0 &= ~0xFF;
    P7->SEL1 &= ~0xFF;
    P7->DIR &= ~0xFF;
}

void turn_on_ir_led() {
    P5->OUT |= 0x08;
    P9->OUT |= 0x04;

}

void turn_off_ir_led() {
    P5->OUT &= ~0x08;
    P9->OUT &= ~0x04;
    Clock_Delay1ms(10);
}

void charge_capacitor() {
    P7->DIR = 0xFF;
    P7->OUT = 0xFF;
    Clock_Delay1us(10);
}

int sense_ir() {
    turn_on_ir_led();
    charge_capacitor();
    P7->DIR = 0x00;
    Clock_Delay1us(1000);

    int sensor = P7->IN;
    return sensor;
}

// ir_num 이 0이면 맨 오른쪽 센서
// ir_num 이 1이면 오른쪽에서 2번째 센서
// ir_num 이 2이면 오른쪽에서 3번째 센서
int sense_nth_ir(int ir_num) {
    assert(ir_num >= 0 && ir_num <= 7);
    int sensor = sense_ir();

    return sensor & (1 << ir_num);
}









/*
 * State 감지
 * */
STATE detect_state(int sensor) {
    int set_bits = count_set_bits(sensor);
    if (set_bits >= 6) return START;
    else if (set_bits >= 2) return DEFAULT;
    return NONE;
}
