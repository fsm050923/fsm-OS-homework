```markdown
# ���̴߳����¼

## һ������  
���μ�¼�����������߳���ص� C ���Դ����ļ����ֱ��� `nosync-ex.c`��`sem-ex.c` �� `mutex-ex.c`����Щ����ĺ���Ŀ���Ǵ��������̣߳�ÿ���̶߳�ȫ�ֱ��� `sum` ���� 1000000 �μ� 1 ��������֮ͬ������ͬ�����Ƶ�ʹ�á�

---

## ����`nosync-ex.c` �������

### 1. ���빦��  
�˴����ڶ��̻߳����¶�ȫ�ֱ��� `sum` �����ۼӲ�������δʹ���κ�ͬ�����ƣ����ܻ�������ݾ������⡣

### 2. Դ��  
```c
#include<stdio.h>
#include<pthread.h>

int sum = 0;
void *thread(void *arg){
    int i;
    for(i = 0; i < 1000000; i++){
        sum +=1;
    }
}
int main(){
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,thread,NULL);
    pthread_create(&tid2,NULL,thread,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    printf("1000000 + 1000000 = %d\n",sum);
    return 0;
}
```
![alt text](image-1.png)
### 3. �������  
- **ȫ�ֱ��� `sum`**�����ڴ洢�����߳��ۼӵĽ����  
- **`thread` ����**��ÿ���߳�ִ�иú������� `sum` ���� 1000000 �μ� 1 ������  
- **`main` ����**�����������̲߳��ȴ�����ִ����ϣ������� `sum` ��ֵ��  

### 4. ��������  
����û��ͬ�����ƣ�����߳̿���ͬʱ���ʺ��޸� `sum` �������������ս��С�� 2000000��

---

## ����`sem-ex.c` �������

### 1. ���빦��  
�ô���ʹ���ź��� `sem` ��ʵ���߳�ͬ����ȷ��ͬһʱ��ֻ��һ���߳̿��Է��� `sum` ������

### 2. Դ��  
```c
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
int sum = 0;
sem_t sem;
void *thread(void *arg){
    int i;
    for(i = 0; i < 1000000; i++){
        sem_wait(&sem);
        sum +=1;
        sem_post(&sem);
    }
}
int main(){
    pthread_t tid1,tid2;
    sem_init(&sem,0,1);
    pthread_create(&tid1,NULL,thread,NULL);
    pthread_create(&tid2,NULL,thread,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    printf("1000000 + 1000000 = %d\n",sum);
    return 0;
}
```
![alt text](image-3.png)
### 3. �������  
- **ȫ�ֱ��� `sum` �� `sem`**��`sum` ���ڴ洢�ۼӽ����`sem` ���ź����������߳�ͬ����  
- **`thread` ����**��ÿ���߳�ִ�иú������ڶ� `sum` ���м� 1 ����ǰ���� `sem_wait` ��ȡ�ź�����������ɺ���� `sem_post` �ͷ��ź�����  
- **`main` ����**����ʼ���ź��������������̲߳��ȴ�����ִ����ϣ������� `sum` ��ֵ��  

### 4. �ŵ�  
ͨ���ź������ƣ����������ݾ������⣬��֤�����ս������ȷ�ԡ�

---

## �ġ�`mutex-ex.c` �������

### 1. ���빦��  
�˴���ʹ�û����� `mutex` ��ʵ���߳�ͬ����ȷ��ͬһʱ��ֻ��һ���߳̿��Է��� `sum` ������

### 2. Դ��  
```c
#include<stdio.h>
#include<pthread.h>

int sum = 0;
pthread_mutex_t mutex;
void *thread(void *arg){
    int i;
    for(i = 0; i < 1000000; i++){
        pthread_mutex_lock(&mutex);
        sum +=1;
        pthread_mutex_unlock(&mutex);
    }
}
int main(){
    pthread_t tid1,tid2;
    pthread_mutex_init(&mutex,NULL);
    pthread_create(&tid1,NULL,thread,NULL);
    pthread_create(&tid2,NULL,thread,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    printf("1000000 + 1000000 = %d\n",sum);
    return 0;
}
```
![alt text](image-2.png)
### 3. �������  
- **ȫ�ֱ��� `sum` �� `mutex`**��`sum` ���ڴ洢�ۼӽ����`mutex` �ǻ������������߳�ͬ����  
- **`thread` ����**��ÿ���߳�ִ�иú������ڶ� `sum` ���м� 1 ����ǰ���� `pthread_mutex_lock` ��ȡ��������������ɺ���� `pthread_mutex_unlock` �ͷŻ�������  
- **`main` ����**����ʼ�������������������̲߳��ȴ�����ִ����ϣ������� `sum` ��ֵ��  

### 4. �ŵ�  
ʹ�û�����������Ч�ر���������Դ���������ݾ�������֤���ս������ȷ�ԡ�

---
## �塢`����������������` �������

### 1.Դ��
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define M 32             /*������Ŀ*/

#define P(x)     sem_wait(&x)
#define V(x)    sem_post(&x)

int in = 0;         /*�����߷��ò�Ʒ��λ��*/
int out = 0;             /*������ȡ��Ʒ��λ��*/

int buff[M] = {0};     /*�����ʼ��Ϊ0�� ��ʼʱû�в�Ʒ*/

sem_t empty_sem;         /*ͬ���ź�����������ʱ��ֹ�����߷Ų�Ʒ*/
sem_t full_sem;         /*ͬ���ź�������û��Ʒʱ��ֹ����������*/
pthread_mutex_t mutex; /*�����ź����� һ��ֻ��һ���̷߳��ʻ���*/

/*
 *output the buffer
 */
void print()
{
    int i;
    for(i = 0; i < M; i++)
        printf("%d ", buff[i]);
    printf("\n");
}

/*
 *producer - �޸ģ���ʽ��������void*����
 */
void *producer(void *arg)  // �޸ĵ㣺���void*����
{
    for(;;)
    {
        sleep(1);

        P(empty_sem);
        pthread_mutex_lock(&mutex);

        in = in % M;
        printf("(+)produce a product. buffer:");

        buff[in] = 1;
        print();
        ++in;

        pthread_mutex_unlock(&mutex);
        V(full_sem);
    }
    return NULL;  // ��ӷ�����䣨��Ȼ��Զ����ִ�е����
}

/*
 *consumer - �޸ģ���ʽ��������void*����
 */
void *consumer(void *arg)  // �޸ĵ㣺���void*����
{
    for(;;)
    {
        sleep(2);

        P(full_sem);
        pthread_mutex_lock(&mutex);

        out = out % M;
        printf("(-)consume a product. buffer:");

        buff[out] = 0;
        print();
        ++out;

        pthread_mutex_unlock(&mutex);
        V(empty_sem);
    }
    return NULL;  // ��ӷ�����䣨��Ȼ��Զ����ִ�е����
}

void sem_mutex_init()
{
    /*
     *semaphore initialize
     */
    int init1 = sem_init(&empty_sem, 0, M);
    int init2 = sem_init(&full_sem, 0, 0);
    if( (init1 != 0) && (init2 != 0))
    {
        printf("sem init failed \n");
        exit(1);
    }
    /*
     *mutex initialize
     */
    int init3 = pthread_mutex_init(&mutex, NULL);
    if(init3 != 0)
    {
        printf("mutex init failed \n");
        exit(1);
    }
    
}
int main()
{
    pthread_t id1;
    pthread_t id2;
    int i;
    int ret;

    sem_mutex_init();
    
    /*create the producer thread*/
    ret = pthread_create(&id1, NULL, producer, NULL);
    if(ret != 0)
    {
        printf("producer creation failed \n");
        exit(1);
    }
    
    /*create the consumer thread*/
    ret = pthread_create(&id2, NULL, consumer, NULL);
    if(ret != 0)
    {
        printf("consumer creation failed \n");
        exit(1);
    }

    pthread_join(id1,NULL);
    pthread_join(id2,NULL);

    exit(0);
}    
```
![alt text](image.png)
### 2. �������
��δ���ʵ���������� - ������ģ�ͣ����ź����ͻ��������� 32 ���������ķ��ʣ�������ÿ 1 ��������������ÿ 2 �����ѣ�ѭ��������
## �����ܽ�  
- **`nosync-ex.c`**��δʹ��ͬ�����ƣ���������ݾ������⣬���½����׼ȷ�����ʺ϶��̻߳�����  
- **`sem-ex.c`**��ͨ���ź���ʵ��ͬ���������ȷ�������Ե��ڻ�������  
- **`mutex-ex.c`**��ͨ��������ʵ��ͬ�������ܱ�֤�����ȷ�ԣ��־��нϸߵ����ܡ�  
��ʵ�ʿ����У�Ӧ���ݾ��峡��ѡ����ʵ�ͬ�����ơ�  
```