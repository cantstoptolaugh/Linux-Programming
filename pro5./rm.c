rm test.c file

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
