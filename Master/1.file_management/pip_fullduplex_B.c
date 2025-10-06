#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,3);
	int fdw = open(argv[1],O_WRONLY);
	int fdr = open(argv[2],O_RDONLY);
	ERROR_CHECK(fdr,-1,"fdr open");
	ERROR_CHECK(fdw,-1,"fdw open");

	puts("simon is ready");
	char buff[1024] = {0};
	while (1) {
		//read
		read(fdr,buff,1024);
		printf("alan: %s\n",buff);

		//write
		memset(buff,0,1024);
		ssize_t rret = read(STDIN_FILENO,buff,1024); //从stdin读取最大为1024bytes长度data
		write(fdw,buff,rret);

	}
	close(fdw);
	close(fdr);
	return 0;
}
