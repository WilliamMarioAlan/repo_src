#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,2);
	int oldfd = open(argv[1],O_RDWR);
	ERROR_CHECK(oldfd,-1,"open");

	int newfd = dup(oldfd);

	printf("oldfd = %d\n",oldfd);
	printf("newfd = %d\n",newfd);
	write(oldfd,"hello",5);
	write(newfd,"world",5);
	

	return 0;
}
