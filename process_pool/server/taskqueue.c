#include "taskqueue.h"
int TaskQueueInit(pstTaskQueue pQue)
{
	pQue->head=NULL;
	pQue->tail=NULL;
	pQue->size=0;
	pthread_cond_init(&pQue->threadCond,NULL);
	pthread_mutex_init(&pQue->threadMutex,NULL);

	return 0;
}

int TaskQueueInsert(pstTaskQueue pQue, pstTaskNode pNewTask)
{
	if(pQue->tail==NULL)
	{
		pQue->head=pNewTask;
		pQue->tail=pNewTask;
	}
	else
	{
		pQue->tail->next=pNewTask;
		pQue->tail=pNewTask;
	}
	++pQue->size;

	return 0;	
}

int GetTask(pstTaskQueue pQue, pstTaskNode *pGetTask)
{
	if(0==pQue->size)
	{
		return -1;
	}

	*pGetTask=pQue->head;
	pQue->head=pQue->head->next;
	--pQue->size;
	if(0==pQue->size)
	{
		pQue->tail=NULL;
	}

	return 0;
}
