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
     * 미로 찾기 알고리즘..
     * 계속 같은 방향으로 돌면 순환 루프를 빠져나갈 수 없음..
     * 갈림길에서
     * 왼쪽 오른쪽 번갈아 도는 방법?
     *
     * 갈림길이 아니라 직진 / 꺾기 선택 가능한 길인 경우!?
     * 더 가보기 전까지는 직진이 가능한 길인지 알 수 없음..
     * 따라서 무조건 꺾기..?
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

        // 이렇게하면 .. 걍 첨부터 lap==NUM_LAP돼서 계속 DONE돼버림
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
            // 아무 센서도 반응하지 않는 상태
            // 180도 회전 후 앞으로 조금씩 이동?
            // 근데 끊긴 길이라면?
            // 앞으로 좀 더 이동해본 후 180도 회전해야하나?
            // 몇번 이상 ERROR-DEFAULT? 상태가 반복되면 180도 회전하는게 좋을듯
            if (stored_state[1] == ERROR) {
                continuous_error++;
            }

            if (continuous_error > MAX_ERROR) {
                // 180' 회전
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
