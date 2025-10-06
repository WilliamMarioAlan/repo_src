#include "myfunc.h"
int main() {
	printf("stdin = %d\n",fileno(stdin));
	printf("stdout = %d\n",fileno(stdout));
	printf("stderr = %d\n",fileno(stderr));
}
