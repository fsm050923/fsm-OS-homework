#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int value=5;     //where?

int main(){
	
	int i;     //where?
	
	pid_t pid;
	
	for(i=0;i<2;i++){    // How many new processes and printfs£¿
		
	
		pid=fork();
	
		if(pid==0){
			value += 15;
			printf("Child: value = %d\n",value);
		}
		else if(pid>0){
			wait(NULL);
			printf("PARNET: value = %d\n",value);
			exit(0);   //Notice£ºWhat will happen with or without this line?
		}
	}
}

