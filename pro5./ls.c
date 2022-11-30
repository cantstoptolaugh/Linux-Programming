#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void do_ls(char name[]){
    
    DIR *dirptr; //현재 디렉토리를 저장해 줄 DIR 포인터 변수 선언
    struct dirent *direntp; //파일의 정보를 저장해 줄 dirent 포인터 변수 선언

    dirptr = opendir(name); //성공하면 DIR을, 실패하면 NULL을 반환

    if (dirptr == NULL){ 
        fprintf(stderr, "Fail to open %s", name); //open에 실패하면 에러를 출력
    }
    else{
        //다음 dirent를 받아 옴
        direntp = readdir(dirptr);
        while (direntp != NULL){
            printf("%s\n", direntp -> d_name); //dirent에서 이름 데려오기
            direntp = readdir(dirptr);
        }
        closedir(dirptr); //완료 후 DIR을 close 해준다.
    }
}

int main(int argc, char* argv[]){ //argc, argv 사용 할 필요는 없지만 어차피 다음에 사용하기 때문에 미리보기
    do_ls("."); //현재 디렉토리 이름 넘겨주기

    return 0;
}