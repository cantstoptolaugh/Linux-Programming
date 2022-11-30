#include <stdio.h>
#include <signal.h>
#include <stdlib.h>


void handler(int sig){
  //SIGINT라는 신호가 오면 유저한테 물어본다. 
  if (sig == SIGINT) {
    printf("you said ctrl+c");
    exit(0) ;
  }
  else if (sig == SIGQUIT) {
    printf("you said ctrl+z");
  }
}

int main(){
  signal(SIGINT, handler);
  signal(SIGQUIT, handler);
  while(1);
}
