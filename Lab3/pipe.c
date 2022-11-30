#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MASSAGESIZE 16

int main() {
    char buff[MASSAGESIZE];
    int p[2], i;
    int pid;

    if(pipe(p)==-1) {
        perror("pipe call fail");
        exit(1);
    }

    pid = fork();

    if(pid == 0) {
        close(p[0]);

        for(i = 0; i < 2; i++) {
            sprintf(buff, "hi, my name is joohyeok ! #%d", i+1);
            write(p[1], buff, MASSAGESIZE);
        }
    }

    else if (pid>0) {
        close(p[1]);
        for(i = 0; i < 2; i++) {
            read(p[0], buff, MASSAGESIZE);
            printf("%s\n", buff);
        }
    }
    
    else
        perror("fork fail");
}