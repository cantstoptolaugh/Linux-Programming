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

struct msgq_data send_data = {1, "Hello, joohyeok\n"};

int main(int argc, char *argv[]) {

    strcpy(send_data.typing, argv[1]);

    int queue_id, length;

    char buff[BUFFSIZE];

    if((queue_id = msgget(QKEY, IPC_CREAT | 0666)) == -1) {
        perror ("message is not approached\n");
        exit(1);
    }

    if(msgsnd(queue_id, &send_data, strlen(send_data.typing), 0) == -1) {
        perror("message is not approached");
        exit(1);
    }
}