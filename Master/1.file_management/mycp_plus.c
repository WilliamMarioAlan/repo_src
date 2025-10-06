#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,3);
	int fd1=open(argv[1],O_RDWR);
	ERROR_CHECK(fd1,-1,"open");
	int fd2=open(argv[2],O_RDWR | O_CREAT | O_TRUNC ,0666);
	ERROR_CHECK(fd2,-1,"open");
	
	char buff[1024]={0};
	while(1) {
		memset(buff,0,sizeof(buff));
		ssize_t ret=read(fd1,buff,sizeof(buff));
		if (ret==0) break;
		write(fd2,buff,ret);
	}
	
	close(fd1);
	close(fd1);
	return 0;
}
