#include <myfunc.h>

int main(int argc,char** argv) {
	ARGS_CHECK(argc,3);
	chmod(argv[1],argv[2]);
}
