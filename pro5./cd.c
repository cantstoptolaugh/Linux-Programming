#include <stdio.h>
#include <unistd.h>

void main(int ac, char*av) {
        char *path;
        if(ac>1) path = av;
        
        else if ((path=(char*)getenv("HOME")) == NULL) path = ".";

        if(chdir(path) < 0) {
                printf("error");
        }
}