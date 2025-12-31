#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,2);
	FILE* f = fopen(argv[1],"r=+");
	ERROR_CHECK(f,NULL,"fopen");
	int ret=write(3,"hello file stream",10);
	printf("ret = %d\n",ret);

	fclose(f);
	return 0;
}
