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
:wq

