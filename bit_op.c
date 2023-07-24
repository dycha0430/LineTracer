#include "bit_op.h"

int count_set_bits(int num) {
    int count = 0;
    while (num) {
        num &= (num - 1); // num과 (num - 1)의 비트 AND 연산을 통해 가장 오른쪽의 1을 지움
        count++;
    }
    return count;
}
