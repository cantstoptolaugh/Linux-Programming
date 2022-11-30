#include<stdio.h>
#include<unistd.h>

int main(void){

    char buf[BUFSIZ];
    int bufsize;

    getcwd(buf,bufsize);

    printf("buf :: %s , size :: %d \n",buf,bufsize);

    return 0;
}