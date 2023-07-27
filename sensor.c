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
 * 가운데쪽 2개여야 디폴트..
 * */
STATE detect_state(int sensor) {
    int set_bits = count_set_bits(sensor);
    if (set_bits >= 7) return CROSS;
    else if (set_bits == 6) return START;
    else if (set_bits >= 2) {
        // 오/왼 활성화 개수 비교만 함
        int left_set_bits = count_left_set_bits(sensor);
        int right_set_bits = count_right_set_bits(sensor);
        if (abs(left_set_bits-right_set_bits) <= 1) {
            // TODO: 차이가 1까지로 여유롭게 할 수도 있을 듯
            return DEFAULT;
        } else if (left_set_bits > right_set_bits) {
            return TURN_LEFT;
        } else {
            return TURN_RIGHT;
        }
    } else if (set_bits == 1) {
        int sensor4 = get_nth_bit(sensor, 4); // right
        int sensor5 = get_nth_bit(sensor, 5); // left
        if (sensor4 || sensor5) return DEFAULT; // return NARROW?

        // TODO!! 좁은길에도 이 로직을 적용할 필요?
        int left_set_bits = count_left_set_bits(sensor);
        int right_set_bits = count_right_set_bits(sensor);
        if (left_set_bits > right_set_bits) {
            return TURN_LEFT;
        } else {
            return TURN_RIGHT;
        }
    }

    return ERROR; // 센서에 아무것도 감지되지 않는 상태
}
