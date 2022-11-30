#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFSIZE 16
#define QKEY (key_t)0111

struct msgq_data {
    long typing;
    char message[BUFFSIZE];
};

struct msgq_data recv_data;

int main(int argc, char *argv[]) {

    int queue_id, length;

    if((queue_id = msgget(QKEY, IPC_CREAT | 0666)) == -1) {
        perror ("message is not approached\n");
        exit(1);
    }

    if((length=msgget(queue_id, &recv_data, BUFFSIZE, 0, 0)) == -1) {
        perror ("message is not approached\n");
        exit(1);
    }

    printf("next message is : %s \n", recv_data);

    if(msgctl(queue_id, IPC_RMID, 0) == -1) {
        perror ("message control is fail\n");
        exit(1);
    }
}