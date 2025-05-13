#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5

sem_t customers;
sem_t barber;
sem_t mutex;
int waiting = 0;

void *barber_thread(void *arg) {
    while (1) {
        sem_wait(&customers);
        sem_wait(&mutex);
        waiting--;
        printf("理发师开始为顾客理发，还有 %d 个顾客等待\n", waiting);
        sem_post(&barber);
        sem_post(&mutex);
        sleep(2);
        printf("理发师完成理发\n");
    }
    return NULL;
}

void *customer_thread(void *arg) {
    int id = *(int *)arg;
    free(arg);
    
    sem_wait(&mutex);
    if (waiting < N) {
        waiting++;
        printf("顾客 %d 进入等待区，当前等待人数: %d\n", id, waiting);
        sem_post(&customers);
        sem_post(&mutex);
        sem_wait(&barber);
        printf("顾客 %d 正在理发\n", id);
    } else {
        sem_post(&mutex);
        printf("顾客 %d 看到没有空椅子，离开了\n", id);
    }
    return NULL;
}

int main() {
    pthread_t barber_tid;
    pthread_t customer_tids[15];
    
    sem_init(&customers, 0, 0);
    sem_init(&barber, 0, 0);
    sem_init(&mutex, 0, 1);
    
    pthread_create(&barber_tid, NULL, barber_thread, NULL);
    
    for (int i = 0; i < 15; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&customer_tids[i], NULL, customer_thread, id);
        sleep(1);
    }
    
    for (int i = 0; i < 15; i++) {
        pthread_join(customer_tids[i], NULL);
    }
    
    sem_destroy(&customers);
    sem_destroy(&barber);
    sem_destroy(&mutex);
    
    return 0;
}
