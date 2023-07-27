#include "bit_op.h"

int count_set_bits(int num)
{
    int count = 0;
    while (num)
    {
        num &= (num - 1); // num과 (num - 1)의 비트 AND 연산을 통해 가장 오른쪽의 1을 지움
        count++;
    }
    return count;
}

int get_nth_bit(int num, int n)
{
    return num & (1 << n);
}

// 4, 5, 6 7 bit check
int count_left_set_bits(int num)
{
    int count = 0;
    num = num >> 4;
    while (num)
    {
        num &= (num - 1); // num과 (num - 1)의 비트 AND 연산을 통해 가장 오른쪽의 1을 지움
        count++;
    }
    return count;
}

// 0, 1, 2, 3 bit check
int count_right_set_bits(int num)
{
    int count = 0;
    num = num & 0b00001111;
    while (num)
    {
        num &= (num - 1); // num과 (num - 1)의 비트 AND 연산을 통해 가장 오른쪽의 1을 지움
        count++;
    }
    return count;
}
