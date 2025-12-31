#include <myfunc.h>
int main() {
	char p[1024]={0};
	char* ret=getcwd(p,sizeof(p));
	puts(p);
	ERROR_CHECK(ret,NULL,"getcwd");
	return 0;
}
