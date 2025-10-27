#include <myfunc.h>
int stringToInte(const char* str) {
	int len=strlen(str);
	int sum=0;
	for (int i=0;i<len;i++) {
		sum=sum*10+(str[i]-'0');
	}
	return sum;
}

int main(int argc,char*argv[]) {
	//改变文件大小 
	//Usage:
	//./test_truncate file 10
	ARGS_CHECK(argc,3);
	int fd=open(argv[1],O_RDWR);
	ERROR_CHECK(fd,-1,"open");

	int len=stringToInte(argv[2]);
	int ret=ftruncate(fd,len);
	ERROR_CHECK(ret,-1,"ftruncate");

	close(fd);
	return 0;
}
