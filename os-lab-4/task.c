
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>

#define NUM_SUPPLIERS 2
#define NUM_SMOKERS 3

sem_t *smoker_sem[NUM_SMOKERS];
sem_t *supplier_ready;
sem_t *done_sem;
sem_t *mutex;

int turn = 0;

void supplier_process() {
    while (1) {
        sem_wait(supplier_ready); // 等待供应许可
        sem_wait(mutex);

        // 根据 turn 放置材料并唤醒对应抽烟者
        switch (turn % 3) {
            case 0:
                printf("Supplier placed TOBACCO and PAPER\n");
                sem_post(smoker_sem[2]); // 唤醒有胶水的抽烟者
                break;
            case 1:
                printf("Supplier placed PAPER and GLUE\n");
                sem_post(smoker_sem[0]); // 唤醒有烟草的抽烟者
                break;
            case 2:
                printf("Supplier placed GLUE and TOBACCO\n");
                sem_post(smoker_sem[1]); // 唤醒有纸的抽烟者
                break;
        }
        turn++;
        sem_post(mutex);

        sem_wait(done_sem); // 等待抽烟完成
    }
}

void smoker_process(int id) {
    while (1) {
        sem_wait(smoker_sem[id]); // 等待材料就绪

        // 抽烟操作
        printf("Smoker %d (has %s) is smoking...\n", id, 
              (id == 0) ? "TOBACCO" : (id == 1) ? "PAPER" : "GLUE");
        sleep(1); // 模拟抽烟耗时

        sem_post(done_sem);      // 通知供应者完成
        sem_post(supplier_ready); // 允许下一次供应
    }
}

int main() {
    // 初始化信号量（使用具名信号量，便于进程间共享）
    supplier_ready = sem_open("/supplier_ready", O_CREAT, 0644, 1);
    done_sem = sem_open("/done_sem", O_CREAT, 0644, 0);
    mutex = sem_open("/mutex", O_CREAT, 0644, 1);

    for (int i = 0; i < NUM_SMOKERS; i++) {
        char name[20];
        sprintf(name, "/smoker_sem_%d", i);
        smoker_sem[i] = sem_open(name, O_CREAT, 0644, 0);
    }

    // 创建供应者和抽烟者进程
    pid_t pid;
    for (int i = 0; i < NUM_SUPPLIERS; i++) {
        pid = fork();
        if (pid == 0) {
            supplier_process();
            exit(0);
        }
    }

    for (int i = 0; i < NUM_SMOKERS; i++) {
        pid = fork();
        if (pid == 0) {
            smoker_process(i);
            exit(0);
        }
    }

    // 主进程等待
    for (int i = 0; i < NUM_SUPPLIERS + NUM_SMOKERS; i++) {
        wait(NULL);
    }

    // 清理信号量
    sem_unlink("/supplier_ready");
    sem_unlink("/done_sem");
    sem_unlink("/mutex");
    for (int i = 0; i < NUM_SMOKERS; i++) {
        char name[20];
        sprintf(name, "/smoker_sem_%d", i);
        sem_unlink(name);
    }

    return 0;
}
