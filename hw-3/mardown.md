---
title: "Git ʹ������̱߳��ʵ��"
date: 2025-04-08 16:10:43
updated: 2025-04-08 16:22:43
tags:
  - linux
  - �߳�
categories:
  - �̳�
aplayer: true
---



# Git ʹ������̱߳��ʵ�鱨��

## 1. GitHub �ֿⴴ�������

Ϊ��ʵ�ֱ����������֮���Эͬ��������ʵ�����ȴ�����һ�� GitHub �ֿ⣬����ʵ�����ͼ�¼�ϴ����òֿ⣬�����ڲ�ͬ������ͬ���͹������汾��

## 2. SSH ��Կ������ GitHub ���ܵ�¼

ʹ�����������������ssh��Կ���������� acidbarium@163.com ��Ϊ��Կ��ע�ͣ�����?GitHub���ܵ�¼��

```bash
ssh-keygen -t ed25519 -C "acidbarium@163.com"
```

ʹ��������������鿴SSH��Կ??

```bash
cat~/.ssh/id_ed25519.pub
```

���ù�Կ����� GitHub �е� SSH ��Կ���ã�

Ȼ��ʹ��������������֤ SSH ��Կ�Ƿ�����ȷ���ò��ܳɹ����� GitHub��

```bash
ssh -T git@github.com
```

������ȷ����ʾ�ɹ���Ϣ��



## 3. Linux������������

Ϊ���д��뿪����������Linux�ϴ���һ���µ��ļ��У�


ͨ���������������git

```bash
sudo yum install -y git
```



ͨ�� Git ��¡�ֿ⵽Linux��

```bash
git clone git@github.com:AcidBarium/osHomework.git
```



�ڲֿ��������У�ʹ����������ͬ�����´��벢�ύ���ģ�

```bash
git pull origin main
git add .
git commit -m "�ύ��Ϣ"
git push origin main
```


## 4. ���̱߳��ʵ��

### 4.1 �������̳߳������

���ȱ�дһ���򵥵Ķ��̳߳��򣬸ó���ʹ�� `pthread` �ⴴ��һ���̲߳���� "Ciallo world!"�������Ǵ���ʾ����

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *worker(void *arg)
{
    printf("Ciallo world!\n");
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, worker, NULL))
    {
        printf("can not create\n");
        exit(1);
    }

    printf("main waiting for thread\n");

    pthread_join(tid, NULL);

    exit(0);
}
```



ͨ�������ָ�����

```bash
gcc cialloWorld.c -o cialloWorld -pthread
```

���н������
```
main waiting for thread
Ciallo world!
```



### 4.2 ���߳����ܲ���

Ϊ�˲��Զ��߳��ڴ��ģ�����е�������������д��һ�����̼߳���͵��̼߳���Աȵĳ��򡣸ó������� 1 �� `MAX_NUM`��10000000��������֮�ͣ���ʹ��2���̷߳���������񣬴Ӷ���߼���Ч�ʡ�

��д��������

```c
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
```


���иó���󣬿��Թ۲쵽���̼߳����ʱ�����Ե��ڵ��̼߳��㣬�ڶ��̵߳�֧���£�����ļ����ٶȴ�Լ�����һ����֤���˶��߳��ڲ��м����е����ơ�
