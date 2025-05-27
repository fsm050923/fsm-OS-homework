# 实验四---进程同步实验
# by 软2306范书铭      20232241378
## 1.实验目的
### 
加深对并发协作进程同步与互斥概念的理解，观察和体验并发进程同步与互斥
操作的效果，分析与研究经典进程同步与互斥问题的实际解决方案。了解 Linux 系
统中 IPC 进程同步工具的用法，练习并发协作进程的同步与互斥操作的编程与调试
技术。

## 2.实验说明
### 
学习了IPC中三个对象的具体作用：共享内存、信号灯数组、消息队列，来解决协作并发进程间的同步与互斥的问题。
### 
同时学习了linux中多种指令来观察IPC中三个对象的具体情况：

1.linux 中可用命令 ipcs -m 观察共享内存情况。  $ipcs -m

2.linux 中可用命令 ipcs -s 观察信号灯数组的情况。 $ ipcs -s

3.linux 中可用命令 ipcs -q 观察消息队列的情况。  $ipcs -q

## 3.示例实验
---
### 3.1运行截图
![alt text](image.png)
![alt text](image-1.png)
![alt text](image-2.png)
![alt text](image-4.png)
---
### 3.2代码分析
一、程序功能与设计逻辑分析
1. 核心功能实现
该程序通过共享内存和信号量机制，模拟了多生产者 - 多消费者在有界循环缓冲区中的同步操作，具体功能包括：

缓冲区管理：

使用长度为 8 的共享内存循环缓冲区（buff_ptr），通过取模运算（% buff_num）实现循环写入和读取。

同步与互斥控制：

同步信号量：prod_sem（初始值为缓冲区大小 8）控制生产者不能向满缓冲区写入；cons_sem（初始值为 0）控制消费者不能从空缓冲区读取。
互斥信号量：pmtx_sem和cmtx_sem（初始值为 1）分别确保生产者、消费者对缓冲区操作的互斥性。

进程阻塞与唤醒：

当缓冲区满时生产者通过down(prod_sem)阻塞，缓冲区空时消费者通过down(cons_sem)阻塞；写入 / 读取后通过up操作唤醒对方进程。

2. 关键数据结构
共享内存：
buff_ptr：存储产品（字符）的缓冲区。
pput_ptr和cget_ptr：分别记录生产者写入位置和消费者读取位置的指针。
信号量：
prod_sem：生产者可用空槽数量（初始为 8）。
cons_sem：消费者可用产品数量（初始为 0）。
pmtx_sem和cmtx_sem：保证对缓冲区指针操作的互斥性。
1. 执行流程分析
生产者流程：
down(prod_sem)：检查是否有空槽，无则阻塞。
down(pmtx_sem)：获取互斥锁，避免多生产者竞争。
写入字符到buff_ptr[*pput_ptr]，更新指针*pput_ptr = (*pput_ptr+1) % 8。
up(pmtx_sem)：释放互斥锁。
up(cons_sem)：唤醒消费者（增加可用产品数）。
消费者流程：
down(cons_sem)：检查是否有产品，无则阻塞。
down(cmtx_sem)：获取互斥锁，避免多消费者竞争。
读取字符buff_ptr[*cget_ptr]，更新指针*cget_ptr = (*cget_ptr+1) % 8。
up(cmtx_sem)：释放互斥锁。
up(prod_sem)：唤醒生产者（增加可用空槽数）。
二、程序优点与不足
优点：
清晰的同步机制：通过信号量明确区分同步（空槽 / 产品数量）和互斥（指针操作）逻辑，符合生产者 - 消费者问题的经典解法。
可扩展性：通过命令行参数（rate）调节进程执行速度，方便观察不同速率下的同步效果。
资源管理规范：使用/proc/sysvipc文件检查 IPC 资源是否存在，避免重复创建导致的资源泄漏。
不足：
未处理连续重复字符限制：题目要求 “生产者不能向同一单元连续写两次以上相同字符”，但程序中写入逻辑为'A' + *pput_ptr，同一位置每次写入不同字符（如 Buffer [0] 始终写 'A'），实际未实现该限制。
单一缓冲区大小：缓冲区长度硬编码为 8，缺乏灵活性，可通过宏定义或参数传入优化。
信号量作用域不明确：生产者和消费者使用全局变量存储信号量 ID（如prod_sem），若扩展为多组生产者 - 消费者，可能导致命名冲突。
三、学习心得
1. 操作系统同步机制的核心思想
信号量的关键作用：通过 PV 操作实现进程间的同步与互斥，信号量值的物理意义（如prod_sem表示空槽数）是理解问题的核心。
临界资源与临界区：缓冲区及其指针属于临界资源，通过互斥信号量确保同一时刻仅一个进程访问临界区，避免数据不一致（如指针竞争修改）。
1. IPC（进程间通信）的实践
共享内存的高效性：直接通过内存地址读写数据，适合频繁交换大量数据的场景，但需配合信号量实现同步。
信号量的跨进程可见性：通过键值（key）标识信号量，不同进程可通过相同 key 获取同一信号量，实现进程间协调。
1. 编程实践中的注意事项
资源清理：程序未提供删除共享内存和信号量的接口，运行后需手动通过ipcrm命令清理，否则会导致资源残留。
调试技巧：通过打印进程 ID、操作内容和位置（如printf("%d producer put: %c to Buffer[%d]\n", getpid(), ...)），可清晰观察进程执行顺序和同步效果。
边界条件测试：如缓冲区满时生产者阻塞、缓冲区空时消费者阻塞，需通过多终端启动进程验证，加深对 “阻塞 - 唤醒” 机制的理解。
1. 扩展思考
多生产者 - 多消费者场景：程序中生产者和消费者各自使用独立的互斥信号量（pmtx_sem和cmtx_sem），可支持多个同类型进程并发操作，无需修改核心逻辑。
性能优化：若缓冲区较大，可引入 “批量生产 / 消费” 模式减少 PV 操作次数，但需注意同步逻辑的复杂性。
错误处理增强：当前程序在获取共享内存或信号量失败时直接exit，实际项目中应增加重试机制或日志记录。
四、总结
该程序是生产者 - 消费者问题的经典实现，通过信号量和共享内存清晰展示了进程同步的核心逻辑。学习过程中需重点理解：

同步信号量与互斥信号量的分工；
循环缓冲区的指针操作（取模运算）；
多进程环境下资源共享与竞争的处理。

通过实际运行和调试，可直观观察到进程如何通过信号量实现 “阻塞 - 唤醒” 的协同工作，加深对操作系统进程同步机制的理解。同时，程序的不足也为进一步优化提供了方向，如增加字符重复限制、动态配置缓冲区大小等，有助于提升系统设计的灵活性和健壮性。

## 4. 独立实验
### 4.1源代码
---
```c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>

#define NUM_SUPPLIERS 2
#define NUM_SMOKERS 3
typedef struct {
    sem_t supplier_ready;
    sem_t done_sem;
    sem_t mutex;
    sem_t smoker_sem[NUM_SMOKERS];
    int turn;
} SharedData;

SharedData *shared;

sem_t *smoker_sem[NUM_SMOKERS];
sem_t *supplier_ready;
sem_t *done_sem;
sem_t *mutex;
void cleanup(int sig) {
    sem_unlink("/supplier_ready");
    sem_unlink("/done_sem");
    sem_unlink("/mutex");
    for (int i = 0; i < NUM_SMOKERS; i++) {
        char name[20];
        sprintf(name, "/smoker_sem_%d", i);
        sem_unlink(name);
    }
    exit(0);
}
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
shared = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, 
              MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // 初始化信号量（匿名信号量，shared指定跨进程共享）
    sem_init(&shared->supplier_ready, 1, 1); // 第二个参数1表示跨进程共享
    sem_init(&shared->done_sem, 1, 0);
    sem_init(&shared->mutex, 1, 1);
    for (int i = 0; i < NUM_SMOKERS; i++) {
        sem_init(&shared->smoker_sem[i], 1, 0);
    }
     signal(SIGINT, cleanup);
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
```
### 4.2运行截图

![alt text](image-1.png)
### 4.3代码分析

---

### **代码逐行解释**

#### **1. 头文件引入**
```c
#include <stdio.h>      // 标准输入输出（如printf）
#include <stdlib.h>     // 系统函数（如exit, malloc）
#include <unistd.h>     // 进程控制（如fork, sleep）
#include <semaphore.h>  // 信号量操作（sem_open, sem_wait等）
#include <sys/wait.h>   // 进程等待（wait）
#include <fcntl.h>      // 文件控制选项（O_CREAT等）
```
**作用**：包含必要的库，支持信号量、进程控制等功能。

---

#### **2. 宏定义**
```c
#define NUM_SUPPLIERS 2 // 供应者进程数
#define NUM_SMOKERS 3   // 抽烟者进程数
```
**作用**：定义常量，便于维护和修改。

---

#### **3. 全局变量声明**
```c
sem_t *smoker_sem[NUM_SMOKERS]; // 三个抽烟者的信号量
sem_t *supplier_ready;          // 控制供应者是否可以供应
sem_t *done_sem;                // 通知供应者抽烟完成
sem_t *mutex;                   // 保护共享变量turn的互斥锁
int turn = 0;                   // 记录当前供应者应放置的材料组合
```
**作用**：声明全局信号量和共享变量，用于进程间同步。

---

#### **4. 供应者进程函数 `supplier_process()`**
```c
void supplier_process() {
    while (1) {
        sem_wait(supplier_ready); // 等待供应许可（初值为1，首次直接通过）
        sem_wait(mutex);          // 进入临界区保护turn变量

        switch (turn % 3) {       // 根据turn值决定放置的材料组合
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
        turn++;                   // 更新轮次
        sem_post(mutex);          // 退出临界区

        sem_wait(done_sem);       // 等待抽烟者完成抽烟
    }
}
```
**关键点**：
- 通过 `supplier_ready` 控制供应节奏。
- 使用 `mutex` 保护 `turn` 变量，确保原子性操作。
- 根据 `turn` 值唤醒对应抽烟者，轮转三种材料组合。

---

#### **5. 抽烟者进程函数 `smoker_process(int id)`**
```c
void smoker_process(int id) {
    while (1) {
        sem_wait(smoker_sem[id]); // 等待自己的信号量被触发

        // 抽烟操作
        printf("Smoker %d (has %s) is smoking...\n", id, 
              (id == 0) ? "TOBACCO" : (id == 1) ? "PAPER" : "GLUE");
        sleep(1);                 // 模拟抽烟耗时

        sem_post(done_sem);       // 通知供应者完成
        sem_post(supplier_ready); // 允许下一轮供应
    }
}
```
**关键点**：
- 每个抽烟者等待自己的信号量（由供应者触发）。
- 抽烟后释放 `done_sem` 和 `supplier_ready`，形成循环。

---

#### **6. 主函数 `main()`**
```c
int main() {
    // 初始化信号量（具名信号量，跨进程共享）
    supplier_ready = sem_open("/supplier_ready", O_CREAT, 0644, 1);
    done_sem = sem_open("/done_sem", O_CREAT, 0644, 0);
    mutex = sem_open("/mutex", O_CREAT, 0644, 1);

    for (int i = 0; i < NUM_SMOKERS; i++) {
        char name[20];
        sprintf(name, "/smoker_sem_%d", i);
        smoker_sem[i] = sem_open(name, O_CREAT, 0644, 0);
    }

    // 创建供应者和抽烟者子进程
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

    // 主进程等待所有子进程结束（实际不会结束）
    for (int i = 0; i < NUM_SUPPLIERS + NUM_SMOKERS; i++) {
        wait(NULL);
    }

    // 清理信号量
    sem_unlink("/supplier_ready");
    // ...（其他信号量清理略）
    return 0;
}
```
**关键点**：
- 使用具名信号量（`sem_open`）实现进程间共享。
- 通过 `fork()` 创建多个供应者和抽烟者进程。
- 主进程等待所有子进程（实际是无限循环，需手动终止）。

---
---

### **代码思路总结**
1. **初始化信号量**：创建具名信号量，控制供应者、抽烟者的同步。
2. **创建子进程**：启动多个供应者和抽烟者进程。
3. **供应者逻辑**：轮流放置两种材料，唤醒对应抽烟者。
4. **抽烟者逻辑**：等待材料就绪后抽烟，完成后通知供应者。
5. **循环控制**：通过 `turn` 和信号量实现无限循环的“供应-抽烟-供应”流程。

---

### **潜在改进点**
1. **错误处理**：未检查 `sem_open` 和 `fork()` 的返回值，实际需添加错误处理。
2. **资源释放**：程序结束时需清理所有具名信号量（如 `sem_unlink`）。
3. **日志输出**：增加时间戳或进程ID，便于调试多进程并发行为。

通过上述分析，你可以清晰地讲解代码逻辑，并从容应对老师的提问。