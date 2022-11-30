#include <stdio.h>
#include <dirent.h>
#include <errno.h>		//errno

int main( )
{
	char strFolderPath[] = { "test_Folder" };
	
	int nResult = rmdir( strFolderPath );

	if( nResult == 0 )
	{
		printf( "폴더 삭제 성공" );
	}
	else if( nResult == -1 )
	{
		perror( "폴더 삭제 실패 - 폴더 안에 파일이 있거나 사용중\n" );
		printf( "errorno : %d", errno );
	}

	return 0;
}