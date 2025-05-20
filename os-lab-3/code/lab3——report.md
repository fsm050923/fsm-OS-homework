# ����ͨ��ʵ�鱨��

## һ ʵ��Ŀ��
###  1.ͨ����ϤLinuxϵͳ�еĹܵ�ͨ�Ż��ƣ�����Խ��̼�ͨ�Ż��Ƶ����
### 2.�۲첢���鲢�����̼�ͨ�ź�Э����Ч��
### 3.��ϰ��??���ܵ���?���̼�ͨ�ŵı�̺͵��Լ���



## �� demo-code�Ķ����
```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
 int pid; 
 int pipe1[2];
 int pipe2[2];
 int x;
 if (pipe(pipe1) < 0) {
 perror("failed to create pipe1");
 exit(EXIT_FAILURE);
 }
 if (pipe(pipe2) < 0) {
 perror("failed to create pipe2");
 exit(EXIT_FAILURE);
 }
 pid = fork();
 if (pid < 0) {
 perror("failed to create new process");
 exit(EXIT_FAILURE);
 } else if (pid == 0) {
 // ?����=>?���̣�?����ͨ��pipe2[1]��?д
 // ?����<=?���̣�?����ͨ��pipe1[0]��
 // ��ˣ���?�����н�pipe1[1]��pipe2[0]�ر�
 close(pipe1[1]);
 close(pipe2[0]);
 do {
 read(pipe1[0], &x, sizeof(int));
 printf("child %d read: %d\n", getpid(), x++);
 write(pipe2[1], &x, sizeof(int));
 } while (x <= 9);
 close(pipe1[0]);
 close(pipe2[1]);
 } else {
 // ?����<=?����:?���̴�pipe2[0]��ȡ?���̴���������
 // ?����=>?����:?���̽����µ�ֵͨ��pipe1[1]д�룬����?����
 // ��ˣ�?���̻��ȹر�pipe1[0]��pipe2[1]��?
 close(pipe1[0]);
 close(pipe2[1]);
 x = 1;
 do {
 write(pipe1[1], &x, sizeof(int));
 read(pipe2[0], &x, sizeof(int));
 printf("parent %d read: %d\n", getpid(), x++);
 } while (x <= 9);
 close(pipe1[1]);
 close(pipe2[0]);
 }
 return EXIT_SUCCESS;
}

```

###  ���н����
![alt text](image-1.png)
### ��������

  #### ���ӽ���ͨ�ţ�
##### 1 ���������ӽ��̷������� x��

##### 2 �ӽ��̽��� x�������󷵻ظ������̡�

##### 3 �������ٽ��յ������ x���������������͡�

##### 4 ѭ��ֱ�� x > 9��

#### ˫��ܵ���

pipe1��������д �� �ӽ��̶���

pipe2���ӽ���д �� �����̶���

## �� fork1.c �Ķ����
### ����
```c
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int value=5;     //where?

int main(){
	
	int i;     //where?
	
	pid_t pid;
	
	for(i=0;i<2;i++){    // How many new processes and printfs??
		
	
		pid=fork();
	
		if(pid==0){
			value += 15;
			printf("Child: value = %d\n",value);
		}
		else if(pid>0){
			wait(NULL);
			printf("PARNET: value = %d\n",value);
			exit(0);   //Notice??What will happen with or without this line?
		}
	}
}

```
### ���н��
![alt text](image-2.png)
### �������
#### ���̴�������
(1) ��ʼ���̣�main��
i = 0��

fork() �� ���� �ӽ���1��

�����̣�

pid > 0 �� ���� else if(pid>0) ��֧��

wait(NULL) �ȴ��ӽ��̽�����

��ӡ "PARENT: value = 5"����Ϊ value ��ʼ�� 5��������û���޸�������

exit(0) �� ��������ֹ���ؼ������û�� exit(0)�������̻����ѭ������

�ӽ���1��

pid == 0 �� ���� if(pid==0) ��֧��

value += 15 �� value = 20��

��ӡ "Child: value = 20"��

i++ �� i = 1����Ϊ i �Ǿֲ��������ӽ������Լ��ĸ�������

����ѭ����

i = 1�����ӽ���1ִ�У���

fork() �� ���� �ӽ���2��

�ӽ���1�������̣���

pid > 0 �� ���� else if(pid>0) ��֧��

wait(NULL) �ȴ��ӽ���2 ������

��ӡ "PARENT: value = 20"����Ϊ�ӽ���1 ֮ǰ�޸��� value����

exit(0) �� �ӽ���1 ��ֹ��

�ӽ���2��

pid == 0 �� ���� if(pid==0) ��֧��

value += 15 �� value = 35����Ϊ�ӽ���1 �Ѿ��޸� value Ϊ 20����

��ӡ "Child: value = 35"��

i++ �� i = 2��ѭ��������


## �� 3��������
### ����
```C
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

// ����f(x)�ĺ���
int calculate_fx(int x) {
    if (x == 1) return 1;
    return calculate_fx(x - 1) * x;
}

// ����f(y)�ĺ���
int calculate_fy(int y) {
    if (y == 1 || y == 2) return 1;
    return calculate_fy(y - 1) + calculate_fy(y - 2);
}

int main() {
    int x, y;
    printf("������x��y��ֵ���ÿո�ָ���: ");
    scanf("%d %d", &x, &y);
    
    // ���������ܵ���һ������fx��һ������fy
    int pipe_fx[2], pipe_fy[2];
    if (pipe(pipe_fx) == -1 || pipe(pipe_fy) == -1) {
        perror("�ܵ�����ʧ��");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid_fx, pid_fy;
    
    // ��������f(x)���ӽ���
    pid_fx = fork();
    if (pid_fx == 0) {
        close(pipe_fx[0]); // �رն���
        int result = calculate_fx(x);
        write(pipe_fx[1], &result, sizeof(result));
        close(pipe_fx[1]);
        exit(0);
    } else if (pid_fx < 0) {
        perror("����f(x)����ʧ��");
        exit(EXIT_FAILURE);
    }
    
    // ��������f(y)���ӽ���
    pid_fy = fork();
    if (pid_fy == 0) {
        close(pipe_fy[0]); // �رն���
        int result = calculate_fy(y);
        write(pipe_fy[1], &result, sizeof(result));
        close(pipe_fy[1]);
        exit(0);
    } else if (pid_fy < 0) {
        perror("����f(y)����ʧ��");
        exit(EXIT_FAILURE);
    }
    
    // �����̵ȴ�����ȡ���
    close(pipe_fx[1]); // �ر�д��
    close(pipe_fy[1]); // �ر�д��
    
    int fx_result, fy_result;
    read(pipe_fx[0], &fx_result, sizeof(fx_result));
    read(pipe_fy[0], &fy_result, sizeof(fy_result));
    
    close(pipe_fx[0]);
    close(pipe_fy[0]);
    
    // �ȴ��ӽ��̽���
    waitpid(pid_fx, NULL, 0);
    waitpid(pid_fy, NULL, 0);
    
    // �������ս��
    int fxy_result = fx_result + fy_result;
    printf("f(x,y) = f(%d) + f(%d) = %d + %d = %d\n", x, y, fx_result, fy_result, fxy_result);
    
    return 0;
}
```
### ���н��
![alt text](image.png)
