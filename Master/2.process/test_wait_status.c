#include <myfunc.h>
int main(int argc,char*argv[]) {
	int waitstatus;
	if (fork()) {
		wait(&waitstatus);
		if(WIFEXITED(waitstatus)) { //是否正常退出
			printf("parent process exit status = %d\n",
					WEXITSTATUS(waitstatus));  //get 退出状态
		}else if (WIFSIGNALED(waitstatus)) { //是否异常退出
			printf("parent process bnormal quit ,terminal signal = %d\n"
					,WTERMSIG(waitstatus)); //被信号终止 
		}
		printf("This is parents ,pid = %d, ppid = %d\n",getpid(),getppid());
	} else {
		printf("This is son,pid = %d, ppid = %d\n",getpid(),getppid());
	}
	return 0;
}
