#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 3

void *hello_thread(void *args) {
    printf("Thread %d: Hello, World\n", args);
        return args;
}

int main() {
    pthread_t tid[NUM_THREADS];
    int i, status;

    for(i=0; i<NUM_THREADS; i++) {
        status = pthread_create (&tid[i], NULL, hello_thread, (void *)i);

        if(status != 0) {
            fprintf(stderr, "Create Thread %d : %d" ,i, status);
            exit(1);
        }
    }
    pthread_exit(NULL);
}