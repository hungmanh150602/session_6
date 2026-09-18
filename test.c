/*

*/

#define CASE 0

#if CASE == 0
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
function devides 'a' by 'b'
*/
int devide(int a, int b)
{
    return a / b;
}

/*
function that subtracts 'a' until it reaches to 0
*/
int subtract(int *a)
{
    while (*a != 0)
    {
        (*a)--;
    }
}

/*
function that compute the sum of `a` and `b`
*/
int add(int a, int b)
{
    int result = a + b;
    return result;
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < 10; i++)
    {
        printf("i = %d\n", i);
    }

    return 0;
}
#elif CASE == 1
#endif