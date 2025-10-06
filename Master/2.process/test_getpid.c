#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>

int main(int argc,char* argv[]) {
	printf("pid = %d\n",getpid());
	printf("ppid = %d\n",getppid());

	return 0;
}
