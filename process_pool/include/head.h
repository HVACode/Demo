#ifndef __HEAD_H__
#define __HEAD_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/time.h>

#define error_check(falsereturn, returnval, funcname, errnum)                  \
	{                                                                      \
		if (falsereturn == returnval)                                  \
		{                                                              \
			printf("%s failed: %s\n", funcname, strerror(errnum)); \
		}                                                              \
	}

#endif
