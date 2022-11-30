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

		/*************************************    명령어를 background로 실행할 경우  **********************************/
		if(strchr(str,'&')!=NULL) {
		    printf("------------------------------------------------------------------------------------------\n");

		    ampersandIdx = -1;
		    str[strlen(str)] = '\0';
		    i=0;
		    pch = strtok(str," ");	// 공백을 구분으로 구분

		    while(pch != NULL && i<3)
		    {
			    if(strcmp(pch,"&")==0)	// '&'를 찾으면 해당 idx를 저장
				    ampersandIdx = i;
			    cmd1[i] = pch;
			    pch = strtok(NULL," ");
			    i++;
		    }
		    cmd1[i] = '\0';
		    cmd1[ampersandIdx] = NULL;	// 사용자 입력 명령어문자열에서'&' 기호를 삭제

		    pid = fork();
		    if(cmd1[0]!=NULL) {
			if(pid == 0) {
				execvp(cmd1[0],cmd1);
				exit(0);
			}
			wait(NULL);
		    }
		    printf("------------------------------------------------------------------------------------------\n");
		    //backgroud로 실행할 경우 부모의 pid 값을 출력한다.
		    printf("[Process id %d]\n",pid); 

		}



		/******************************************   pipe를 사용하는 경우  *************************************** */

		else if(strchr(str,'|')!=NULL){
			// 파이프 기호 '|'를 기준으로 명령어를 구분한다. 
			pipeFront = strtok (str,"|");
			pipeMiddle = strtok (NULL, "|");
			pipeEnd = strtok (NULL, "|");

			strcat(pipeFront,"\0");
			strcat(pipeMiddle, "\0");
			if(pipeEnd != NULL) // pipe가 한 개일 수도 있으므로 pipeEnd가 NULL인지 검사
				strcat(pipeEnd,"\0");
			
			i=0;//초기화
			pch = strtok (pipeFront," ");
			while (pch != NULL && i<3)
			{
				cmd1[i]=pch;
				pch = strtok (NULL, " ");
				i++;
			}
			cmd1[i]='\0';
			
			i=0;//초기화
			pch = strtok (pipeMiddle," ");
			while (pch != NULL && i<3)
			{
				cmd2[i]=pch;
				pch = strtok (NULL, " ");
				i++;
			}
			cmd2[i]='\0';

			if(pipeEnd != NULL) { // pipe가 한 개일 수도 있으므로 pipeEnd가 NULL인지 검사
				i=0;
				pch = strtok(pipeEnd," ");
				while(pch != NULL && i<3)
				{
					cmd3[i]=pch;
					pch = strtok(NULL," ");
					i++;
				}
				cmd3[i]='\0';
			}

			if(pipe(fd1) == -1){	    // 파이프 생성 fd1[0] 읽기용, fd1[1] 쓰기용
				printf("fail to call pipe1\n");
				exit(1);
			}
			if(pipeEnd != NULL) {	    //파이프가 두 개일 경우 fd2[0] 읽기, fd2[1] 쓰기
				if(pipe(fd2) == -1) {
					printf("fail to call pipe2\n");
					exit(1);
				}
			}
			switch(fork())// pipeFront  명령어프로세스 생성
			{
				case -1 : perror("fork error"); break;
				case 0 :
					  dup2(fd1[1],1);  // 표준출력 파이프 연결
					 
					  // 파일 기술자 절약
					  close(fd1[0]);
					  close(fd2[0]);
					  close(fd2[1]);
					  close(fd1[1]);	  

					  execvp(cmd1[0], cmd1);
					  exit(0);  
			}
			switch(fork())// pipeMiddle 명령어 프로세스 생성
			{
				case -1 : perror ("fork"); break;
				case 0 :

					  dup2(fd1[0],0);  //  표준입력 파이프 연결
					  dup2(fd2[1],1);  //  표준출력 파이프 연결

					  // 파일 기술자 절약
					  close(fd1[1]);
					  close(fd2[0]);
					  close(fd1[0]);
					  close(fd2[1]);

					  execvp(cmd2[0], cmd2);
					  exit(0);  
			}
		    
			if(pipeEnd != NULL) {

			    switch(fork()) // pipeEnd 명령어 프로세스 생성
			    {
			    	case -1 : perror("fork"); break;
			    	case 0 :

					  dup2(fd2[0],0);  // 표준입력 파이프 연결

					  // 파일 기술자 절약
					  close(fd2[1]);
					  close(fd1[0]);
					  close(fd1[1]);
					  close(fd2[0]); 

					  execvp(cmd3[0], cmd3);
					  exit(0);
			    }
			}   
			
			
			close(fd1[0]);
			close(fd1[1]);
			close(fd2[0]);
			close(fd2[1]);
				       	
			while(wait(NULL) != -1);
		}
		/**************************************************   단일 명령어 수행  *************************************************/
		else{       // background, pipe가 아닌 일반 명령어인 경우 

			str[strlen(str)]='\0';
			if(strcmp(str,"exit") == 0) 
				exit(0);
			else 
				system(str);
		    }   

	}
	return 0;
}
