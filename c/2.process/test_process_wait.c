#include <myfunc.h>
int main(int argc,char*argv[]) {
	if (fork()) {
		printf("This is parents ,pid = %d, ppid = %d\n",getpid(),getppid());
		wait(NULL);
		/* sleep(1); */
	} else {
		printf("This is son,pid = %d, ppid = %d\n",getpid(),getppid());
	}
	return 0;
}
