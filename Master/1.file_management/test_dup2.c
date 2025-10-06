#include <myfunc.h>
int main(int argc,char*argv[]) {
	//用dup2实现stdout 重定向
	ARGS_CHECK(argc,2);
	int fd = open(argv[1],O_RDWR | O_CREAT,0664);
	ERROR_CHECK(fd,-1,"open");

	printf("\n");
	//TODO为什么要加上\n才有用

	int backupfd = 10;
	dup2(STDOUT_FILENO,backupfd);	//备份stdout
	dup2(fd,STDOUT_FILENO); //实现重定向

	printf("输出重定向\n");
	dup2(backupfd,STDOUT_FILENO); //关闭重定向
	printf("关闭重定向\n");

	close(fd);
	return 0;
}
