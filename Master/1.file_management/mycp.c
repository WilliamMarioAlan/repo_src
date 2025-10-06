#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,3);
	int fd1=open(argv[1],O_RDWR);
	ERROR_CHECK(fd1,-1,"open");
	int fd2=open(argv[2],O_RDWR | O_CREAT | O_TRUNC ,0666);
	ERROR_CHECK(fd2,-1,"open");
	
	char buff[512]={0};
	ssize_t ret=read(fd1,buff,sizeof(buff));
	ERROR_CHECK(ret,-1,"read");
	write(fd2,buff,ret);

	close(fd1);
	close(fd1);
	return 0;
}
