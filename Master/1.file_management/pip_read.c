#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,2);
	int fdr = open(argv[1],O_RDONLY);
	ERROR_CHECK(fdr,-1,"fdr open");

	puts("read side is opened");
	char buff[1024] = {0};
	ssize_t ret = read(fdr,buff,1024);
	printf("ret = %ld buffer = %s",ret,buff);

	close(fdr);
	return 0;
}
