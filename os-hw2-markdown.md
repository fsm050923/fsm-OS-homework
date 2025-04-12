---
title: "OpenEulsecondʾ������2"
date: 2025-03-14 20:10:43
updated: 2025-03-14 20:22:43
tags:
  - linux
  - ��������
categories:
  - ����
aplaysecond: true
---


# OpenEulsecondʾ������2

## ��ȡ���� ID ʵ��

### 1. ����Դ�����ļ�

�����������������ָ������д����
```bash
vi first.cpp
```

### 2. ��д����

�� `first.cpp` ��д�����´��룺

```cpp
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    pid_t my_pid;
    my_pid = getpid();
    printf("My process ID is %d\n", my_pid);

    return 0;
}
```



### 3. ���벢���д���

������룺

```bash
g++ first.cpp -o first

```

���г���

```bash
./first
```



```bash
My process ID is 18290
```

�ɴ˿ɼ�����ǰ����Ľ��̺ţ�PID��Ϊ 18290��


## ���̴����븸�ӽ��̹�ϵʵ��

### 1. ����Դ�����ļ�

ʹ��������������༭�ļ� `second.cpp`��

```bash
vi second.cpp
```

### 2.��д����

��д��������
```cpp
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t child_pid;
    child_pid = fork();
    if (child_pid < 0)
    {
        psecondror("Fork failed");
        return 1;
    }
    else if (child_pid == 0)
    {
        printf("Child process:My PID is %d \n", getpid());
    }
    else
    {
        printf("Parent process: My PID is %d \n", getpid());
        printf("Parent process: Child process ID is %d \n", child_pid);
    }
    return 0;
}
```



### 3. ���벢���г���  

������룺

```bash
g++ second.cpp -o second
```

���г���

```bash
./second
```

�õ��Ľ������ͼ��ʾ

![alt text](https://acidbarium.github.io/openEulsecondImg/eulsecondSecond4.png)

```bash
Parent process: My PID is 70845
Parent process: Child process ID is 70846
Child process:My PID is 70846
```
   ��������� 

- `fork()` ϵͳ���óɹ��󣬻����һ���ӽ��̡�  
- ������������Լ��Ľ��̺��Լ��ӽ��̵Ľ��̺ţ����ӽ��̽�����Լ��Ľ��̺š�  
- ��֤���˸��ӽ���֮��Ĺ�ϵ�������ӽ����ɸ����̴������������Ǿ��и��Զ����Ľ��̺š�

## �����̵ȴ��ӽ����˳�����

### 1.�޸Ĵ���

�޸�second.cpp�еĴ���

```bash
vi second.cpp
```

�޸�Ϊ����Ĵ���

```cpp
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t child_pid;
    child_pid = fork();
    if (child_pid < 0)
    {
        psecondror("Fork failed");
        return 1;
    }
    else if (child_pid == 0)
    {
        printf("Child process:My PID is %d \n", getpid());
    }
    else
    {
        printf("Parent process: Child process ID is %d \n", child_pid);
        int status;
        waitpid(child_pid, &status, 0);
        if (WIFEXITED(status))
        {
            printf("Parent process: Child exited with status %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}
```
��ͼ��ʾ

![alt text](https://acidbarium.github.io/openEulsecondImg/eulsecondSecond5.png)

### 2.���д���

�ٴα�����벢����

```bash
g++ second.cpp -o second
```

```bash
./second
```


�õ��������

![alt text](https://acidbarium.github.io/openEulsecondImg/eulsecondSecond6.png)

```bash
Parent process: Child process ID is 140429
Child process:My PID is 140429
Parent process: Child exited with status 0
```

�����ʾ��  

- �������ڵ��� `waitpid()` �����ȴ�״̬��ֱ���ӽ����˳���ż���ִ�к������롣  
- �ӽ��������˳����˳�״̬Ϊ 0�������Ҹ�����ͨ�� `WIFEXITED` �� `WEXITSTATUS` ����ӽ��̵��˳�״̬��  


## ��� fork() ���̴���ʵ��


### 1. ��д����

��д��������

```cpp
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    fork();
    fork();
    fork();
    printf("ciallo\n");
    return 0;
}
```
### 2. ������������ļ�

����һ���������������ļ�save.txt

```bash
touch save.txt
```


### 3. ���벢���г���

������벢�����еĽ�����뵽save.txt

```bash
g++ san.cpp -o san
```

```bash
./san > save.txt
```

��save.txt�����ֽ������ͼ��ʾ

![alt text](https://acidbarium.github.io/openEulsecondImg/eulsecondSecond7.png)

�������ÿ�ε��� `fork()` �󣬵�ǰ���̶��Ḵ�Ƴ�һ���µĽ��̡�  

- �״� `fork()` ��2 ������  
- �ٴ� `fork()` ��ÿ�������ָ��Ƴ�һ���ӽ��̣������ﵽ 4 ��  
- ������ `fork()` �������ﵽ \(2^3 = 8\) ������  
��ˣ�������� 8 �� `ciallo`����֤�˽��̸��Ƶı���Ч����

## ���̶�����ʵ��

### 1. ��д����

��д��������
```cpp
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int x = 1;
    pid_t p = fork();
    if (p < 0)
    {
        psecondror("fork fail");
        exit(1);
    }
    else if (p == 0)
        printf("Child has x = %d \n", ++x);
    else
        printf("Parent has x = %d\n", --x);

    return 0;
}
```

### 2. ���д���



```bash
Parent has x = 0
Child has x = 2
```
  
������������ӽ����� `fork()` ���ú�ӵ�и��Զ������ڴ�ռ䡣  

- �����̶Ա��� `x` ִ���Լ�������������Ϊ 0��  
- �ӽ��̶Ա��� `x` ִ������������������Ϊ 2��  
����֤�˽��̼�����������ŵ�����


## �ܽ�

ͨ�����ʵ����֤�� Linux ϵͳ�н��̴����Ļ���ԭ�����ص㣺

- ͨ�� getpid() �ܹ���ȡ���̺ţ�
- fork() ���óɹ���ᴴ��һ���ӽ��̣����ӽ��̸��Զ������У�
- �����̿���ͨ�� waitpid() �Ȼ��Ƶȴ��ӽ����˳����Ӷ�ʵ�ֽ���ͬ����
- ��� fork() ���ָ���������ӽ�������
- ������ӵ�ж������ڴ�ռ䣬�Ա������޸Ļ���Ӱ�졣