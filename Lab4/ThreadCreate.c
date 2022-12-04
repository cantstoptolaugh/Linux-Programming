#include <pthread.h>
#include <stdio.h>

void *hello_thread(void *arg) {
    printf("this is thread : hello \n");
    return 0;
}

int main() {
    pthread_t tid;
    int status;

    status = pthread_create (&tid, NULL, hello_thread, NULL);

    if(status != 0)
        perror("Create thread");

    pthread_exit(NULL);
}