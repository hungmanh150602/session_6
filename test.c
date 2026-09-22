/*
CASE 0: multi-thread
CASE 1: multi-process
CASE 2: Valgrind
*/

#define CASE 2

#if CASE == 0
#include <stdio.h>
#include <pthread.h>

pthread_t th[2];

void *thread_func(void *arg)
{
    pthread_t id = pthread_self();
    if (pthread_equal(id, th[0]))
    {
        printf("Thread 1 join thread 2.\n");
        pthread_join(th[1], NULL);
    }
    else
    {
        printf("Thread 2 join thread 1.\n");
        pthread_join(th[0], NULL);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (pthread_create(&th[0], NULL, thread_func, NULL) != 0)
    {
        printf("Create thread 1 fail.\n");
        return -1;
    }

    if (pthread_create(&th[1], NULL, thread_func, NULL) != 0)
    {
        printf("Create thread 2 fail.\n");
        return -1;
    }

    if (pthread_join(th[0], NULL))
    {
        printf("Join thread 1 fail.\n");
        return -1;
    }
    if (pthread_join(th[1], NULL))
    {
        printf("Join thread 2 fail.\n");
        return -1;
    }
    return 0;
}
#elif CASE == 1
#elif CASE == 2
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int x;

    printf("x = %d\n", x);

    return 0;
}
#endif