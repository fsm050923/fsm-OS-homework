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
 ![ʾ����ͼ](![alt text](image.png))
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
![alt text](image-1.png)
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

## �塢�ܽ�  
- **`nosync-ex.c`**��δʹ��ͬ�����ƣ���������ݾ������⣬���½����׼ȷ�����ʺ϶��̻߳�����  
- **`sem-ex.c`**��ͨ���ź���ʵ��ͬ���������ȷ�������Ե��ڻ�������  
- **`mutex-ex.c`**��ͨ��������ʵ��ͬ�������ܱ�֤�����ȷ�ԣ��־��нϸߵ����ܡ�  
��ʵ�ʿ����У�Ӧ���ݾ��峡��ѡ����ʵ�ͬ�����ơ�  
```