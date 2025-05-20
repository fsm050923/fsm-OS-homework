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
