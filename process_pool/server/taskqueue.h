#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__
#include "../include/head.h"

typedef struct taskNode
{
	int sockfd;
	struct taskNode *next;
} stTaskNode, *pstTaskNode;

typedef struct taskQueue
{
	int size;
	pstTaskNode head;
	pstTaskNode tail;
	pthread_cond_t threadCond;
	pthread_mutex_t threadMutex;
} stTaskQueue, *pstTaskQueue;

// 任务队列初始化
int TaskQueueInit(pstTaskQueue pQue);

// 新任务进入队列
int TaskQueueInsert(pstTaskQueue pQue, pstTaskNode pNewTask);

// 从队列首获取一个任务
int GetTask(pstTaskQueue pQue, pstTaskNode *pGetTask);

#endif