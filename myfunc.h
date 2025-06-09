#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <errno.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <sys/types.h>
#include <grp.h>

#include <pthread.h>
#include <sys/mman.h>
#define ARGS_CHECK(argc,num) {if(argc!=num){error(1,0,"args error!");}}
#define ERROR_CHECK(ret,num,message) {if(ret==num){perror(message);return -1;}}


#include <arpa/inet.h>

#include <sys/socket.h>
#include <netinet/ip.h>

#include <netdb.h>
