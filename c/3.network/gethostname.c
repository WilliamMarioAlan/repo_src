#include <myfunc.h>
int main(int argc,char*argv[]) {
	ARGS_CHECK(argc,2);
	struct hostent* entry = gethostbyname(argv[1]);
	if (entry == NULL) {
		herror("gethostbyname func error");
		return -1;
	}
	printf("official name = %s\n",entry->h_name);
	for (int i = 0;entry->h_aliases[i] != NULL;i++) {
		printf("\t alias%d = %s\n",i,entry->h_aliases[i]);
	}
	return 0;
}
