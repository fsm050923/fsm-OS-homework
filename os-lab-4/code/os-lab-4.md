# ʵ����---����ͬ��ʵ��
# by ��2306������      20232241378
## 1.ʵ��Ŀ��
### 
����Բ���Э������ͬ���뻥��������⣬�۲�����鲢������ͬ���뻥��
������Ч�����������о��������ͬ���뻥�������ʵ�ʽ���������˽� Linux ϵ
ͳ�� IPC ����ͬ�����ߵ��÷�����ϰ����Э�����̵�ͬ���뻥������ı�������
������

## 2.ʵ��˵��
### 
ѧϰ��IPC����������ľ������ã������ڴ桢�źŵ����顢��Ϣ���У������Э���������̼��ͬ���뻥������⡣
### 
ͬʱѧϰ��linux�ж���ָ�����۲�IPC����������ľ��������

1.linux �п������� ipcs -m �۲칲���ڴ������  $ipcs -m

2.linux �п������� ipcs -s �۲��źŵ����������� $ ipcs -s

3.linux �п������� ipcs -q �۲���Ϣ���е������  $ipcs -q

## 3.ʾ��ʵ��
---
### 3.1���н�ͼ
![alt text](image.png)
![alt text](image-1.png)
![alt text](image-2.png)
![alt text](image-4.png)
---
### 3.2�������
һ��������������߼�����
1. ���Ĺ���ʵ��
�ó���ͨ�������ڴ���ź������ƣ�ģ���˶������� - �����������н�ѭ���������е�ͬ�����������幦�ܰ�����

����������

ʹ�ó���Ϊ 8 �Ĺ����ڴ�ѭ����������buff_ptr����ͨ��ȡģ���㣨% buff_num��ʵ��ѭ��д��Ͷ�ȡ��

ͬ���뻥����ƣ�

ͬ���ź�����prod_sem����ʼֵΪ��������С 8�����������߲�������������д�룻cons_sem����ʼֵΪ 0�����������߲��ܴӿջ�������ȡ��
�����ź�����pmtx_sem��cmtx_sem����ʼֵΪ 1���ֱ�ȷ�������ߡ������߶Ի����������Ļ����ԡ�

���������뻽�ѣ�

����������ʱ������ͨ��down(prod_sem)��������������ʱ������ͨ��down(cons_sem)������д�� / ��ȡ��ͨ��up�������ѶԷ����̡�

2. �ؼ����ݽṹ
�����ڴ棺
buff_ptr���洢��Ʒ���ַ����Ļ�������
pput_ptr��cget_ptr���ֱ��¼������д��λ�ú������߶�ȡλ�õ�ָ�롣
�ź�����
prod_sem�������߿��ÿղ���������ʼΪ 8����
cons_sem�������߿��ò�Ʒ��������ʼΪ 0����
pmtx_sem��cmtx_sem����֤�Ի�����ָ������Ļ����ԡ�
1. ִ�����̷���
���������̣�
down(prod_sem)������Ƿ��пղۣ�����������
down(pmtx_sem)����ȡ������������������߾�����
д���ַ���buff_ptr[*pput_ptr]������ָ��*pput_ptr = (*pput_ptr+1) % 8��
up(pmtx_sem)���ͷŻ�������
up(cons_sem)�����������ߣ����ӿ��ò�Ʒ������
���������̣�
down(cons_sem)������Ƿ��в�Ʒ������������
down(cmtx_sem)����ȡ������������������߾�����
��ȡ�ַ�buff_ptr[*cget_ptr]������ָ��*cget_ptr = (*cget_ptr+1) % 8��
up(cmtx_sem)���ͷŻ�������
up(prod_sem)�����������ߣ����ӿ��ÿղ�������
���������ŵ��벻��
�ŵ㣺
������ͬ�����ƣ�ͨ���ź�����ȷ����ͬ�����ղ� / ��Ʒ�������ͻ��⣨ָ��������߼������������� - ����������ľ���ⷨ��
����չ�ԣ�ͨ�������в�����rate�����ڽ���ִ���ٶȣ�����۲첻ͬ�����µ�ͬ��Ч����
��Դ����淶��ʹ��/proc/sysvipc�ļ���� IPC ��Դ�Ƿ���ڣ������ظ��������µ���Դй©��
���㣺
δ���������ظ��ַ����ƣ���ĿҪ�� �������߲�����ͬһ��Ԫ����д����������ͬ�ַ�������������д���߼�Ϊ'A' + *pput_ptr��ͬһλ��ÿ��д�벻ͬ�ַ����� Buffer [0] ʼ��д 'A'����ʵ��δʵ�ָ����ơ�
��һ��������С������������Ӳ����Ϊ 8��ȱ������ԣ���ͨ���궨�����������Ż���
�ź�����������ȷ�������ߺ�������ʹ��ȫ�ֱ����洢�ź��� ID����prod_sem��������չΪ���������� - �����ߣ����ܵ���������ͻ��
����ѧϰ�ĵ�
1. ����ϵͳͬ�����Ƶĺ���˼��
�ź����Ĺؼ����ã�ͨ�� PV ����ʵ�ֽ��̼��ͬ���뻥�⣬�ź���ֵ���������壨��prod_sem��ʾ�ղ��������������ĺ��ġ�
�ٽ���Դ���ٽ���������������ָ�������ٽ���Դ��ͨ�������ź���ȷ��ͬһʱ�̽�һ�����̷����ٽ������������ݲ�һ�£���ָ�뾺���޸ģ���
1. IPC�����̼�ͨ�ţ���ʵ��
�����ڴ�ĸ�Ч�ԣ�ֱ��ͨ���ڴ��ַ��д���ݣ��ʺ�Ƶ�������������ݵĳ�������������ź���ʵ��ͬ����
�ź����Ŀ���̿ɼ��ԣ�ͨ����ֵ��key����ʶ�ź�������ͬ���̿�ͨ����ͬ key ��ȡͬһ�ź�����ʵ�ֽ��̼�Э����
1. ���ʵ���е�ע������
��Դ��������δ�ṩɾ�������ڴ���ź����Ľӿڣ����к����ֶ�ͨ��ipcrm������������ᵼ����Դ������
���Լ��ɣ�ͨ����ӡ���� ID���������ݺ�λ�ã���printf("%d producer put: %c to Buffer[%d]\n", getpid(), ...)�����������۲����ִ��˳���ͬ��Ч����
�߽��������ԣ��绺������ʱ��������������������ʱ��������������ͨ�����ն�����������֤������� ������ - ���ѡ� ���Ƶ���⡣
1. ��չ˼��
�������� - �������߳����������������ߺ������߸���ʹ�ö����Ļ����ź�����pmtx_sem��cmtx_sem������֧�ֶ��ͬ���ͽ��̲��������������޸ĺ����߼���
�����Ż������������ϴ󣬿����� ���������� / ���ѡ� ģʽ���� PV ��������������ע��ͬ���߼��ĸ����ԡ�
��������ǿ����ǰ�����ڻ�ȡ�����ڴ���ź���ʧ��ʱֱ��exit��ʵ����Ŀ��Ӧ�������Ի��ƻ���־��¼��
�ġ��ܽ�
�ó����������� - ����������ľ���ʵ�֣�ͨ���ź����͹����ڴ�����չʾ�˽���ͬ���ĺ����߼���ѧϰ���������ص���⣺

ͬ���ź����뻥���ź����ķֹ���
ѭ����������ָ�������ȡģ���㣩��
����̻�������Դ�����뾺���Ĵ���

ͨ��ʵ�����к͵��ԣ���ֱ�۹۲쵽�������ͨ���ź���ʵ�� ������ - ���ѡ� ��Эͬ����������Բ���ϵͳ����ͬ�����Ƶ���⡣ͬʱ������Ĳ���ҲΪ��һ���Ż��ṩ�˷����������ַ��ظ����ơ���̬���û�������С�ȣ�����������ϵͳ��Ƶ�����Ժͽ�׳�ԡ�

## 4. ����ʵ��
### 4.1Դ����
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
        sem_wait(supplier_ready); // �ȴ���Ӧ���
        sem_wait(mutex);

        // ���� turn ���ò��ϲ����Ѷ�Ӧ������
        switch (turn % 3) {
            case 0:
                printf("Supplier placed TOBACCO and PAPER\n");
                sem_post(smoker_sem[2]); // �����н�ˮ�ĳ�����
                break;
            case 1:
                printf("Supplier placed PAPER and GLUE\n");
                sem_post(smoker_sem[0]); // �������̲ݵĳ�����
                break;
            case 2:
                printf("Supplier placed GLUE and TOBACCO\n");
                sem_post(smoker_sem[1]); // ������ֽ�ĳ�����
                break;
        }
        turn++;
        sem_post(mutex);

        sem_wait(done_sem); // �ȴ��������
    }
}

void smoker_process(int id) {
    while (1) {
        sem_wait(smoker_sem[id]); // �ȴ����Ͼ���

        // ���̲���
        printf("Smoker %d (has %s) is smoking...\n", id, 
              (id == 0) ? "TOBACCO" : (id == 1) ? "PAPER" : "GLUE");
        sleep(1); // ģ����̺�ʱ

        sem_post(done_sem);      // ֪ͨ��Ӧ�����
        sem_post(supplier_ready); // ������һ�ι�Ӧ
    }
}

int main() {
shared = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, 
              MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // ��ʼ���ź����������ź�����sharedָ������̹���
    sem_init(&shared->supplier_ready, 1, 1); // �ڶ�������1��ʾ����̹���
    sem_init(&shared->done_sem, 1, 0);
    sem_init(&shared->mutex, 1, 1);
    for (int i = 0; i < NUM_SMOKERS; i++) {
        sem_init(&shared->smoker_sem[i], 1, 0);
    }
     signal(SIGINT, cleanup);
    // ��ʼ���ź�����ʹ�þ����ź��������ڽ��̼乲��
    supplier_ready = sem_open("/supplier_ready", O_CREAT, 0644, 1);
    done_sem = sem_open("/done_sem", O_CREAT, 0644, 0);
    mutex = sem_open("/mutex", O_CREAT, 0644, 1);

    for (int i = 0; i < NUM_SMOKERS; i++) {
        char name[20];
        sprintf(name, "/smoker_sem_%d", i);
        smoker_sem[i] = sem_open(name, O_CREAT, 0644, 0);
    }

    // ������Ӧ�ߺͳ����߽���
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

    // �����̵ȴ�
    for (int i = 0; i < NUM_SUPPLIERS + NUM_SMOKERS; i++) {
        wait(NULL);
    }

    // �����ź���
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
### 4.2���н�ͼ

![alt text](image-1.png)
### 4.3�������

---

### **�������н���**

#### **1. ͷ�ļ�����**
```c
#include <stdio.h>      // ��׼�����������printf��
#include <stdlib.h>     // ϵͳ��������exit, malloc��
#include <unistd.h>     // ���̿��ƣ���fork, sleep��
#include <semaphore.h>  // �ź���������sem_open, sem_wait�ȣ�
#include <sys/wait.h>   // ���̵ȴ���wait��
#include <fcntl.h>      // �ļ�����ѡ�O_CREAT�ȣ�
```
**����**��������Ҫ�Ŀ⣬֧���ź��������̿��Ƶȹ��ܡ�

---

#### **2. �궨��**
```c
#define NUM_SUPPLIERS 2 // ��Ӧ�߽�����
#define NUM_SMOKERS 3   // �����߽�����
```
**����**�����峣��������ά�����޸ġ�

---

#### **3. ȫ�ֱ�������**
```c
sem_t *smoker_sem[NUM_SMOKERS]; // ���������ߵ��ź���
sem_t *supplier_ready;          // ���ƹ�Ӧ���Ƿ���Թ�Ӧ
sem_t *done_sem;                // ֪ͨ��Ӧ�߳������
sem_t *mutex;                   // �����������turn�Ļ�����
int turn = 0;                   // ��¼��ǰ��Ӧ��Ӧ���õĲ������
```
**����**������ȫ���ź����͹�����������ڽ��̼�ͬ����

---

#### **4. ��Ӧ�߽��̺��� `supplier_process()`**
```c
void supplier_process() {
    while (1) {
        sem_wait(supplier_ready); // �ȴ���Ӧ��ɣ���ֵΪ1���״�ֱ��ͨ����
        sem_wait(mutex);          // �����ٽ�������turn����

        switch (turn % 3) {       // ����turnֵ�������õĲ������
            case 0:
                printf("Supplier placed TOBACCO and PAPER\n");
                sem_post(smoker_sem[2]); // �����н�ˮ�ĳ�����
                break;
            case 1:
                printf("Supplier placed PAPER and GLUE\n");
                sem_post(smoker_sem[0]); // �������̲ݵĳ�����
                break;
            case 2:
                printf("Supplier placed GLUE and TOBACCO\n");
                sem_post(smoker_sem[1]); // ������ֽ�ĳ�����
                break;
        }
        turn++;                   // �����ִ�
        sem_post(mutex);          // �˳��ٽ���

        sem_wait(done_sem);       // �ȴ���������ɳ���
    }
}
```
**�ؼ���**��
- ͨ�� `supplier_ready` ���ƹ�Ӧ���ࡣ
- ʹ�� `mutex` ���� `turn` ������ȷ��ԭ���Բ�����
- ���� `turn` ֵ���Ѷ�Ӧ�����ߣ���ת���ֲ�����ϡ�

---

#### **5. �����߽��̺��� `smoker_process(int id)`**
```c
void smoker_process(int id) {
    while (1) {
        sem_wait(smoker_sem[id]); // �ȴ��Լ����ź���������

        // ���̲���
        printf("Smoker %d (has %s) is smoking...\n", id, 
              (id == 0) ? "TOBACCO" : (id == 1) ? "PAPER" : "GLUE");
        sleep(1);                 // ģ����̺�ʱ

        sem_post(done_sem);       // ֪ͨ��Ӧ�����
        sem_post(supplier_ready); // ������һ�ֹ�Ӧ
    }
}
```
**�ؼ���**��
- ÿ�������ߵȴ��Լ����ź������ɹ�Ӧ�ߴ�������
- ���̺��ͷ� `done_sem` �� `supplier_ready`���γ�ѭ����

---

#### **6. ������ `main()`**
```c
int main() {
    // ��ʼ���ź����������ź���������̹���
    supplier_ready = sem_open("/supplier_ready", O_CREAT, 0644, 1);
    done_sem = sem_open("/done_sem", O_CREAT, 0644, 0);
    mutex = sem_open("/mutex", O_CREAT, 0644, 1);

    for (int i = 0; i < NUM_SMOKERS; i++) {
        char name[20];
        sprintf(name, "/smoker_sem_%d", i);
        smoker_sem[i] = sem_open(name, O_CREAT, 0644, 0);
    }

    // ������Ӧ�ߺͳ������ӽ���
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

    // �����̵ȴ������ӽ��̽�����ʵ�ʲ��������
    for (int i = 0; i < NUM_SUPPLIERS + NUM_SMOKERS; i++) {
        wait(NULL);
    }

    // �����ź���
    sem_unlink("/supplier_ready");
    // ...�������ź��������ԣ�
    return 0;
}
```
**�ؼ���**��
- ʹ�þ����ź�����`sem_open`��ʵ�ֽ��̼乲��
- ͨ�� `fork()` ���������Ӧ�ߺͳ����߽��̡�
- �����̵ȴ������ӽ��̣�ʵ��������ѭ�������ֶ���ֹ����

---
---

### **����˼·�ܽ�**
1. **��ʼ���ź���**�����������ź��������ƹ�Ӧ�ߡ������ߵ�ͬ����
2. **�����ӽ���**�����������Ӧ�ߺͳ����߽��̡�
3. **��Ӧ���߼�**�������������ֲ��ϣ����Ѷ�Ӧ�����ߡ�
4. **�������߼�**���ȴ����Ͼ�������̣���ɺ�֪ͨ��Ӧ�ߡ�
5. **ѭ������**��ͨ�� `turn` ���ź���ʵ������ѭ���ġ���Ӧ-����-��Ӧ�����̡�

---

### **Ǳ�ڸĽ���**
1. **������**��δ��� `sem_open` �� `fork()` �ķ���ֵ��ʵ������Ӵ�����
2. **��Դ�ͷ�**���������ʱ���������о����ź������� `sem_unlink`����
3. **��־���**������ʱ��������ID�����ڵ��Զ���̲�����Ϊ��

ͨ����������������������ؽ�������߼���������Ӧ����ʦ�����ʡ�