#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;

/* ============================================================
 * 1. CRASH
 * ============================================================ */
void test_crash(void)
{
    int *p = NULL;

    printf("[CRASH] About to dereference NULL...\n");
    fflush(stdout);

    *p = 100;
}

/* ============================================================
 * 2. INVALID WRITE
 * ============================================================ */
void test_invalid_write(void)
{
    int *p;

    p = malloc(5 * sizeof(int));

    printf("[INVALID WRITE] Writing outside allocated memory...\n");

    for (int i = 0; i <= 5; i++)
    {
        p[i] = i;
    }

    free(p);
}

/* ============================================================
 * 3. MEMORY LEAK
 * ============================================================ */
void test_leak(void)
{
    int *p;

    p = malloc(100 * sizeof(int));

    if (p == NULL)
        return;

    p[0] = 123;

    printf("[LEAK] Allocated memory but will not free it\n");
}

/* ============================================================
 * 4. USE AFTER FREE
 * ============================================================ */
void test_uaf(void)
{
    int *p;

    p = malloc(sizeof(int));

    if (p == NULL)
        return;

    *p = 123;

    free(p);

    printf("[UAF] Reading memory after free: %d\n", *p);
}

/* ============================================================
 * 5. DOUBLE FREE
 * ============================================================ */
void test_double_free(void)
{
    int *p;

    p = malloc(sizeof(int));

    if (p == NULL)
        return;

    *p = 123;

    free(p);

    printf("[DOUBLE FREE] Freeing same memory again...\n");

    free(p);
}

/* ============================================================
 * 6. UNINITIALIZED MEMORY
 * ============================================================ */
void test_uninitialized(void)
{
    int x;
    int y;

    printf("[UNINITIALIZED] Using uninitialized variable...\n");

    y = x + 10;

    printf("x = %d\n", x);
    printf("y = %d\n", y);
}

/* ============================================================
 * 7. THREAD / DEADLOCK
 * ============================================================ */
void *thread_a(void *arg)
{
    (void)arg;

    printf("[THREAD A] Locking mutex A\n");
    fflush(stdout);

    pthread_mutex_lock(&mutex_a);

    sleep(1);

    printf("[THREAD A] Waiting for mutex B\n");
    fflush(stdout);

    pthread_mutex_lock(&mutex_b);

    printf("[THREAD A] Got both mutexes\n");

    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);

    return NULL;
}

void *thread_b(void *arg)
{
    (void)arg;

    printf("[THREAD B] Locking mutex B\n");
    fflush(stdout);

    pthread_mutex_lock(&mutex_b);

    sleep(1);

    printf("[THREAD B] Waiting for mutex A\n");
    fflush(stdout);

    pthread_mutex_lock(&mutex_a);

    printf("[THREAD B] Got both mutexes\n");

    pthread_mutex_unlock(&mutex_a);
    pthread_mutex_unlock(&mutex_b);

    return NULL;
}

void test_deadlock(void)
{
    pthread_t t1;
    pthread_t t2;

    printf("[DEADLOCK] Starting two threads...\n");

    pthread_create(&t1, NULL, thread_a, NULL);
    pthread_create(&t2, NULL, thread_b, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("[DEADLOCK] Finished\n");
}

/* ============================================================
 * 8. FILE / SYSCALL
 * ============================================================ */
void test_file(void)
{
    int fd;
    char buffer[32];

    printf("[FILE] Opening file...\n");

    fd = open("test.txt", O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return;
    }

    ssize_t n = read(fd, buffer, sizeof(buffer) - 1);

    if (n > 0)
    {
        buffer[n] = '\0';
        printf("File content: %s\n", buffer);
    }

    close(fd);
}

/* ============================================================
 * 9. SLOW SYSTEM CALL
 * ============================================================ */
void test_sleep(void)
{
    printf("[SLEEP] Sleeping for 10 seconds...\n");
    fflush(stdout);

    sleep(10);

    printf("[SLEEP] Finished\n");
}

/* ============================================================
 * MAIN
 * ============================================================ */
void print_usage(const char *program)
{
    printf("\nUsage:\n");
    printf("  %s crash\n", program);
    printf("  %s invalid\n", program);
    printf("  %s leak\n", program);
    printf("  %s uaf\n", program);
    printf("  %s doublefree\n", program);
    printf("  %s uninit\n", program);
    printf("  %s deadlock\n", program);
    printf("  %s file\n", program);
    printf("  %s sleep\n", program);
    printf("\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "crash") == 0)
    {
        test_crash();
    }
    else if (strcmp(argv[1], "invalid") == 0)
    {
        test_invalid_write();
    }
    else if (strcmp(argv[1], "leak") == 0)
    {
        test_leak();
    }
    else if (strcmp(argv[1], "uaf") == 0)
    {
        test_uaf();
    }
    else if (strcmp(argv[1], "doublefree") == 0)
    {
        test_double_free();
    }
    else if (strcmp(argv[1], "uninit") == 0)
    {
        test_uninitialized();
    }
    else if (strcmp(argv[1], "deadlock") == 0)
    {
        test_deadlock();
    }
    else if (strcmp(argv[1], "file") == 0)
    {
        test_file();
    }
    else if (strcmp(argv[1], "sleep") == 0)
    {
        test_sleep();
    }
    else
    {
        printf("Unknown test: %s\n", argv[1]);
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}