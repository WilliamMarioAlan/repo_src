#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,2);
	int fd=open(argv[1],O_RDONLY);
	ERROR_CHECK(fd,-1,"open");

	close(fd);
	return 0;
}
