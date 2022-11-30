mv test.c file
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>


int main(){
	char str[256];	    // 사용자 입력 문자열
	char strSave[256]; // 임시저장용
	char *pipeFront = NULL;
	char *pipeMiddle = NULL;
	char *pipeEnd = NULL;
	char *cmd1[4]; //앞쪽 명령어
	char *cmd2[5]; //중간 명령어
	char *cmd3[5]; //마지막 명령어 
	
	int ampersandIdx; // 백그라운드 사용 시 입력 인수에서 '&'를 삭제하기 위한 변수
	int i ;
	char *pch;//문장나눌때 임시 저장
	
	int fd1[2]; // 파이프 처리
	int fd2[2]; // 파이프 처리 (파이프가 두 개일 경우)
	
	pid_t pid;

	//signal 처리
	sigset_t block;	    // 막아놓을 시그널 목록
	sigemptyset(&block);
	sigaddset(&block, SIGINT);   //목록에 해당 ^C시그널 추가
	sigaddset(&block, SIGQUIT);   //^\시그널 추가
	sigprocmask(SIG_BLOCK, &block, NULL);

	while(1){

		printf("sys test sh$ ");
		
		//프롬프트가 변수 초기화
		for(i=0 ; i < 256 ; i++){    
			str[i]='\0';
		}
		
		fgets(str,sizeof(str),stdin);
		str[strlen(str)-1] ='\0';
