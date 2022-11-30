#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>


int main() {
    pid_t pid;

    printf("call fork \n");

    pid = fork();

    if(pid == 0)
    printf("this is child process\n");

    else if(pid>0)
    printf("this is parent process\n");

    else
    printf("this is failed\n");
}