#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include "../include/head.h"
#include "taskqueue.h"

typedef struct threadpool
{
	int iThreadNum;
	pthread_t *thids;
	stTaskQueue taskQueue;
} stThreadpool, *pstThreadpool;

typedef struct protocol_train
{
	int iLength;//有效数据的长度
	char data[1000];//有效数据
} stProtoTrain;

// 线程池初始化
int threadpoolInit(pstThreadpool pthreadpool, int ithreadNum);

// 创建线程池线程
int threadpoolCreateThds(pstThreadpool pthreadpool);

// 创建套接字listen
int tcpInit(char *ip, char *port, int *sfd);

// 添加监听描述符
int epollAdd(int epfd, int sfd);

// 发送文件
int sendFile(const char *filename, int socketfd);

#endif