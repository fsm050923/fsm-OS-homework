#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>

#define MAX_NUM 10000000
#define THREADS 2

// �̲߳����ṹ��
typedef struct
{
    uint64_t start;
    uint64_t end;
    uint64_t result;
} ThreadArgs;

// �̺߳��������� [start, end] �ĺ�
void *calculate_sum(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    uint64_t sum = 0;
    for (uint64_t i = args->start; i <= args->end; i++)
    {
        sum += i;
    }
    args->result = sum;
    return NULL;
}

// ���̼߳���
uint64_t single_thread_sum()
{
    uint64_t sum = 0;
    for (uint64_t i = 1; i <= MAX_NUM; i++)
    {
        sum += i;
    }
    return sum;
}

// ���̼߳���
uint64_t multi_thread_sum()
{
    pthread_t threads[THREADS];
    ThreadArgs args[THREADS];
    uint64_t total_sum = 0;

    // ������㷶Χ
    uint64_t segment = MAX_NUM / THREADS;
    for (int i = 0; i < THREADS; i++)
    {
        args[i].start = i * segment + 1;
        args[i].end = (i == THREADS - 1) ? MAX_NUM : (i + 1) * segment;
        args[i].result = 0;
        pthread_create(&threads[i], NULL, calculate_sum, &args[i]);
    }

    // �ȴ������߳����
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
        total_sum += args[i].result;
    }

    return total_sum;
}

int main()
{
    clock_t start, end;
    uint64_t sum;
    double time_used;

    // ���̲߳���
    start = clock();
    sum = single_thread_sum();
    end = clock();
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("���߳̽��: %lu, ��ʱ: %.5f ��\n", sum, time_used);

    // ���̲߳���
    start = clock();
    sum = multi_thread_sum();
    end = clock();
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("���߳̽��: %lu, ��ʱ: %.5f ��\n", sum, time_used);

    return 0;
}