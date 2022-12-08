#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 100
#define THREAD_COUNT 4

int buffer [BUFFER_SIZE];
int in = -1;
int out = -1;
int num = 0;

sem_t buffer_slot;
sem_t items;
sem_t mutex;

void *consumer(void *arg);
void *producer(void *arg);

int main(void) {
    sem_init(&buffer_slot, 0, BUFFER_SIZE);
    sem_init(&items, 0, 0);
    sem_init(&mutex, 0, 1);
    pthread_t threads[THREAD_COUNT];

    for(int i =0; i<THREAD_COUNT; i++) {
        if (i%2==0) {
            pthread_create(&threads[i], NULL, consumer, NULL);
        }

        else {
            pthread_create(&threads[i], NULL, producer, NULL);
        }
    }

    for(int i =0; i< THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
        printf("\n result : %d \n\n", num);
        pthread_exit(NULL);
    }

    sem_destroy(&buffer_slot);
    sem_destroy(&items);
    return 0;
}

void *consumer (void *arg) {
    int data;

    for(int i=0;i<BUFFER_SIZE*10;i++) {
        sem_wait(&items);
        sem_wait(&mutex);

        out++;
        num--;

        out %= BUFFER_SIZE;
        data = buffer[out];

        sem_post(&mutex);
        sem_post(&buffer_slot);

        printf("Consumer consumes index : %d \n", data);
    }
}

void *producer(void *arg) {
    int i, data;

    for(int i=0;i<BUFFER_SIZE*10;i++) {

        printf("Producer consumes index : %d \n", i);

        sem_wait(&buffer_slot);
        sem_wait(&mutex);

        num++;
        in++;

        in %= BUFFER_SIZE;
        buffer[in] = i;

        sem_post(&mutex);
        sem_post(&items);

        
    }
}
