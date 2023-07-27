#include "main_header.h"
#include "sensor.h"
#include "motor.h"
#include "led.h"
#include "tachometer.h"

#define MAX_ERROR 10
#define NUM_LAP 2
/**
 * main.c
 */

int lap = 0;

void init() {
    Clock_Init48MHz();
    led_init();
    ir_sensor_init();
    motor_init();
    timer_A3_capture_init();

    lap = 0;
}

void show_state_with_led(STATE state) {
    switch (state) {
        case START:
            turn_on_led(CYAN);
            break;
        case DEFAULT:
            turn_on_led(WHITE);
            break;
        case TURN_LEFT:
            turn_on_led(BLUE);
            break;
        case TURN_RIGHT:
            turn_on_led(GREEN);
            break;
        case CROSS:
            turn_on_led(YELLOW);
            break;
        case DONE:
            turn_off_led();
            break;
        case ERROR:
            turn_on_led(RED);
            break;
    }
}

void main(void)
{
    init();

    int sensor;
    STATE state = ERROR, before_state = ERROR;
    STATE stored_state[2] = {ERROR, ERROR};
    DIRECTION recent_turn = NONE;
    int continuous_error = 0;

    /*
     * �̷� ã�� �˰���..
     * ��� ���� �������� ���� ��ȯ ������ �������� �� ����..
     * �����濡��
     * ���� ������ ������ ���� ���?
     *
     * �������� �ƴ϶� ���� / ���� ���� ������ ���� ���!?
     * �� ������ �������� ������ ������ ������ �� �� ����..
     * ���� ������ ����..?
     * */
    while (1) {
        sensor = sense_ir();
        before_state = state;
        state = detect_state(sensor);
        if (state != before_state) {
            stored_state[1] = stored_state[0];
            stored_state[0] = before_state;
        }


        if (before_state == START && state != START) lap++;
        else if (stored_state[1] != ERROR && state != ERROR) continuous_error = 0;

        // �̷����ϸ� .. �� ÷���� lap==NUM_LAP�ż� ��� DONE�Ź���
//        if (lap == NUM_LAP) {
//            state = DONE;
//            // Stop
////                stop();
////                break;
//        }
        show_state_with_led(state);

        if (state == START) {
            if (before_state != state)
                move_forward(NORMAL);
        } else if (state == DEFAULT) {
            if (before_state != state) {
                // Go Straight
                move_forward(NORMAL);
            }
        } else if (state == TURN_LEFT) {
            if (before_state != state) {
                turn_left(NORMAL);
                recent_turn = LEFT;
            }
        } else if (state == TURN_RIGHT) {
            if (before_state != state) {
                turn_right(NORMAL);
                recent_turn = RIGHT;
            }
        } else if (state == CROSS) {
            // TODO..
            if (before_state != state) {
                if (recent_turn == LEFT) {
                    turn_right(NORMAL);
                    recent_turn = RIGHT;
                } else {
                    turn_left(NORMAL);
                    recent_turn = LEFT;
                }
            }
        } else if (state == ERROR) {
            // �ƹ� ������ �������� �ʴ� ����
            // 180�� ȸ�� �� ������ ���ݾ� �̵�?
            // �ٵ� ���� ���̶��?
            // ������ �� �� �̵��غ� �� 180�� ȸ���ؾ��ϳ�?
            // ��� �̻� ERROR-DEFAULT? ���°� �ݺ��Ǹ� 180�� ȸ���ϴ°� ������
            if (stored_state[1] == ERROR) {
                continuous_error++;
            }

            if (continuous_error > MAX_ERROR) {
                // 180' ȸ��
                turn_left(NORMAL);
                right_count = 0;
                while (1) {
                    if (right_count > 180) break;
                }
            } else if (before_state != state) {
                int cnt = 0;
                while (cnt++ < 100)
                    move_backward(NORMAL);
            }
        }

        turn_off_ir_led();
    }
}
