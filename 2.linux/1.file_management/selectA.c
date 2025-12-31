#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,3);
	int fdw = open(argv[1],O_WRONLY);
	int fdr = open(argv[2],O_RDONLY);
	ERROR_CHECK(fdr,-1,"fdr open");
	ERROR_CHECK(fdw,0,"fdw open");

	puts("server is ready");

	char buff[1025] = {0};
	fd_set rdset; //为fd_set申请内存
	struct timeval timeout;
		while (1) {
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;
		//select 之前需要reset rdset
		FD_ZERO(&rdset);
		FD_SET(STDIN_FILENO,&rdset);
		FD_SET(fdr,&rdset);
		int flag = select(fdr+1,&rdset,NULL,NULL,&timeout);
		if (flag == 0) {
			puts("timeout!");
			continue;
		}

		/* 检测select是否return 如果return 那么fdr或者stdin就绪了 */
		if (FD_ISSET(fdr,&rdset)) {
			//pipe read side ready
			memset(buff,0,1024);
			int ret = read(fdr,buff,1024);
			if (ret == 0) break;
			printf("simon: %s\n",buff);
		}
		if (FD_ISSET(STDIN_FILENO,&rdset)) {
			memset(buff,0,1024);
			ssize_t rret = read(STDIN_FILENO,buff,1024); //从stdin读取最大为1024bytes长度data
			write(fdw,buff,rret);	
		}
	}

	close(fdw);
	close(fdr);
	return 0;
}
