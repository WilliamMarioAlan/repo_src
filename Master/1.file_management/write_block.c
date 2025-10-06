#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,2);
	int fd1 = open(argv[1],O_RDWR);
	int fd2 = open(argv[1],O_RDWR);
	puts("pipe is opened");
	
	int cnt = 0;
	char buff[4096] = {0};
	fd_set rset,wset;
	while (1) {
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(fd1,&rset);
		FD_SET(fd2,&wset);
		int ret = select(fd2 + 1,&rset,&wset,NULL,NULL);
		if(FD_ISSET(fd1,&rset)) {
			puts("read side is ready");
			read(fd2,buff,2048);
			cnt--;
			printf("ret = %d cnt = %d\n",ret,cnt);
		}
		if (FD_ISSET(fd2,&wset)) {
			puts("write side is ready");
			write(fd1,buff,4096);
			cnt++;
			printf("ret = %d cnt = %d\n",ret,cnt);
		}
	}
	return 0;
}
