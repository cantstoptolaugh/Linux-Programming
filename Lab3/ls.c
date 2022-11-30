#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    DIR *pdir;
    struct dirent *pde;

    int i = 0;

    if (argc < 2) {
        fprintf(stderr, "File : ls directory name\n");
        exit(1);
    }

    if((pdir = opendir(argv[1])) < 0) {
        perror("open directory");
        exit(1);
    }

    while((pde = readdir(pdir)) != NULL) {
        printf("%20s ", pde->d_name);

        if (++i % 3 == 0)
        printf("\n");
    }
    
    printf("\n");
    closedir(pdir);
}
