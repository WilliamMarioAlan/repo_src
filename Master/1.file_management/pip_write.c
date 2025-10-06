#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,2);
	int fdw = open(argv[1],O_WRONLY);
	ERROR_CHECK(fdw,-1,"fdw open");

	puts("write side is opened");

	write(fdw,"helloworld",5);
	while(1) {
		sleep(1);
	}


	close(fdw);
	return 0;
}
