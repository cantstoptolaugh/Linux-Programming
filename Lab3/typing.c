#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define PASSWORD 12

int main(void) {
    int fd;
    int nread, count = 0, err_count = 0;
    char ch, text[] = "Hello this is typing test. Text this samely.";
    struct termios init_attr, new_attr;

    time_t start, end;

    fd = open(ttyname(fileno(stdin)), O_RDWR);

    tcgetattr(fd, &init_attr);

    new_attr = init_attr;
    new_attr.c_cflag &= ~ICANON;
    new_attr.c_cflag &= ~ECHO;
    new_attr.c_cc[VMIN] = 1;
    new_attr.c_cc[VTIME] = 0;

    if(tcsetattr(fd, TCSANOW, &new_attr) != 0) {
        fprintf(stderr, "there is error on this terminal\n");
    }

    printf("please text this sentence samely\n\n", text);

    time(&start);

    while((nread = read(fd, &ch, 1)) > 0 && ch!= '\n') {
        if(ch == text[count++])
            write(fd, &ch, 1);
        else {
            write(fd, "*", 1);
            err_count++;
        }
    }
    time(&end);

    printf("%f", ((float)(end -start)));
    printf("\n error counting is %d\n", err_count);
    tcsetattr(fd, TCSANOW, &init_attr);
    close(fd);
}