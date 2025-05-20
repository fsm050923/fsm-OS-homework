# 进程通信实验报告

## 一 实验目标
###  1.通过熟悉Linux系统中的管道通信机制，加深对进程间通信机制的理解
### 2.观察并体验并发进程间通信和协作的效果
### 3.练习利??名管道进?进程间通信的编程和调试技术



## 二 demo-code阅读理解
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
 // ?进程=>?进程：?进程通过pipe2[1]进?写
 // ?进程<=?进程：?进程通过pipe1[0]读
 // 因此，在?进程中将pipe1[1]和pipe2[0]关闭
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
 // ?进程<=?进程:?进程从pipe2[0]读取?进程传过来的数
 // ?进程=>?进程:?进程将更新的值通过pipe1[1]写入，传给?进程
 // 因此，?进程会先关闭pipe1[0]和pipe2[1]端?
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

###  运行结果：
![alt text](image-1.png)
### 代码解读：

  #### 父子进程通信：
##### 1 父进程向子进程发送数字 x。

##### 2 子进程接收 x，递增后返回给父进程。

##### 3 父进程再接收递增后的 x，继续递增并发送。

##### 4 循环直到 x > 9。

#### 双向管道：

pipe1：父进程写 → 子进程读。

pipe2：子进程写 → 父进程读。

## 三 fork1.c 阅读理解
### 代码
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
### 运行结果
![alt text](image-2.png)
### 代码解析
#### 进程创建流程
(1) 初始进程（main）
i = 0：

fork() → 创建 子进程1。

父进程：

pid > 0 → 进入 else if(pid>0) 分支。

wait(NULL) 等待子进程结束。

打印 "PARENT: value = 5"（因为 value 初始是 5，父进程没有修改它）。

exit(0) → 父进程终止（关键！如果没有 exit(0)，父进程会继续循环）。

子进程1：

pid == 0 → 进入 if(pid==0) 分支。

value += 15 → value = 20。

打印 "Child: value = 20"。

i++ → i = 1（因为 i 是局部变量，子进程有自己的副本）。

继续循环。

i = 1（仅子进程1执行）：

fork() → 创建 子进程2。

子进程1（父进程）：

pid > 0 → 进入 else if(pid>0) 分支。

wait(NULL) 等待子进程2 结束。

打印 "PARENT: value = 20"（因为子进程1 之前修改了 value）。

exit(0) → 子进程1 终止。

子进程2：

pid == 0 → 进入 if(pid==0) 分支。

value += 15 → value = 35（因为子进程1 已经修改 value 为 20）。

打印 "Child: value = 35"。

i++ → i = 2，循环结束。


## 四 3并发进程
### 代码
```C
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

// 计算f(x)的函数
int calculate_fx(int x) {
    if (x == 1) return 1;
    return calculate_fx(x - 1) * x;
}

// 计算f(y)的函数
int calculate_fy(int y) {
    if (y == 1 || y == 2) return 1;
    return calculate_fy(y - 1) + calculate_fy(y - 2);
}

int main() {
    int x, y;
    printf("请输入x和y的值（用空格分隔）: ");
    scanf("%d %d", &x, &y);
    
    // 创建两个管道：一个用于fx，一个用于fy
    int pipe_fx[2], pipe_fy[2];
    if (pipe(pipe_fx) == -1 || pipe(pipe_fy) == -1) {
        perror("管道创建失败");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid_fx, pid_fy;
    
    // 创建计算f(x)的子进程
    pid_fx = fork();
    if (pid_fx == 0) {
        close(pipe_fx[0]); // 关闭读端
        int result = calculate_fx(x);
        write(pipe_fx[1], &result, sizeof(result));
        close(pipe_fx[1]);
        exit(0);
    } else if (pid_fx < 0) {
        perror("创建f(x)进程失败");
        exit(EXIT_FAILURE);
    }
    
    // 创建计算f(y)的子进程
    pid_fy = fork();
    if (pid_fy == 0) {
        close(pipe_fy[0]); // 关闭读端
        int result = calculate_fy(y);
        write(pipe_fy[1], &result, sizeof(result));
        close(pipe_fy[1]);
        exit(0);
    } else if (pid_fy < 0) {
        perror("创建f(y)进程失败");
        exit(EXIT_FAILURE);
    }
    
    // 主进程等待并读取结果
    close(pipe_fx[1]); // 关闭写端
    close(pipe_fy[1]); // 关闭写端
    
    int fx_result, fy_result;
    read(pipe_fx[0], &fx_result, sizeof(fx_result));
    read(pipe_fy[0], &fy_result, sizeof(fy_result));
    
    close(pipe_fx[0]);
    close(pipe_fy[0]);
    
    // 等待子进程结束
    waitpid(pid_fx, NULL, 0);
    waitpid(pid_fy, NULL, 0);
    
    // 计算最终结果
    int fxy_result = fx_result + fy_result;
    printf("f(x,y) = f(%d) + f(%d) = %d + %d = %d\n", x, y, fx_result, fy_result, fxy_result);
    
    return 0;
}
```
### 运行结果
![alt text](image.png)
