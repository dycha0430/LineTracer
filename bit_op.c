#include "bit_op.h"

int count_set_bits(int num) {
    int count = 0;
    while (num) {
        num &= (num - 1); // num�� (num - 1)�� ��Ʈ AND ������ ���� ���� �������� 1�� ����
        count++;
    }
    return count;
}
