/* ./test_write ./buff "hellloworld" */
#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,3);
	int fd=open(argv[1],O_RDWR | O_CREAT | O_APPEND,0666);
	ERROR_CHECK(fd,-1,"open");
	ssize_t sret=write(fd,argv[2],strlen(argv[2]));
	ERROR_CHECK(sret,-1,"write");
	printf("sret=%ld\n",sret);
	close(fd);
	return 0;
}
