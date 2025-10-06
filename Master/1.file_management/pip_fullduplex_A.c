#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,3);
	int fdr = open(argv[1],O_RDONLY);
	int fdw = open(argv[2],O_WRONLY);
	ERROR_CHECK(fdr,-1,"fdr open");
	ERROR_CHECK(fdw,-1,"fdw open");

	puts("alan is ready");
	char buff[1024] = {0};
	while (1) {
		//write
		memset(buff,0,1024);
		ssize_t rret = read(STDIN_FILENO,buff,1024); //从stdin读取最大为1024bytes长度data
		write(fdw,buff,rret);
		//read
		read(fdr,buff,1024);
		printf("simon: %s\n",buff);
	}
	close(fdw);
	close(fdr);
	return 0;
}
