#include "threadpool.h"

int threadpoolInit(pstThreadpool pthreadpool, int ithreadNum)
{
	pthreadpool->iThreadNum=ithreadNum;
	pthreadpool->thids=(pthread_t*)calloc(ithreadNum,sizeof(pthread_t));
	TaskQueueInit(&pthreadpool->taskQueue);
	return 0;
}

// 线程终止的清理函数
void cleanfunc(void *pArgs)
{
	pthread_mutex_t* Mutex=(pthread_mutex_t*)pArgs;
	pthread_mutex_unlock(Mutex);
}

// 线程入口
void* threadfunc(void *pArgs)
{
	pstTaskQueue pQue=(pstTaskQueue)pArgs;

	pstTaskNode pTask=NULL;
	
	int getTaskRes=1;

	while(1)
	{
		pthread_mutex_lock(&pQue->threadMutex);
		pthread_cleanup_push(cleanfunc,&pQue->threadMutex);
		if(0==pQue->size)
		{
			pthread_cond_wait(&pQue->threadCond,&pQue->threadMutex);
		}
		getTaskRes=GetTask(pQue,&pTask);

		pthread_mutex_unlock(&pQue->threadMutex);

		if(0==getTaskRes){
			sendFile("The_Holy_Bible_Res.txt",pTask->sockfd);
			free(pTask);
			pTask=NULL;
		}

		pthread_cleanup_pop(1);

	} 
	
	printf("thread terminated ... \n");
	pthread_exit(NULL);
}

//
int threadpoolCreateThds(pstThreadpool pthreadpool)
{
	for(int i=0;i<pthreadpool->iThreadNum;++i)
	{
		pthread_create(&pthreadpool->thids[i],NULL,threadfunc,&pthreadpool->taskQueue);
	}

	return 0;
}

int tcpInit(char *ip, char *port, int* sfd)
{
	*sfd=socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in serverAddr;
	memset(&serverAddr,0,sizeof(serverAddr));

	serverAddr.sin_addr.s_addr=inet_addr(ip);
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(atoi(port));

	socklen_t reuse=1;
	setsockopt(*sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));

	bind(*sfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

	listen(*sfd,10);

	return 0;
}

// 添加监听描述符
int epollAdd(int epfd, int sfd)
{
	struct epoll_event event;
	memset(&event,0,sizeof(event));

	event.events=EPOLLIN;
	event.data.fd=sfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);

	return 0;
}


void sigfunc_PIPE(int signum)
{
	printf("SIGPIPE activated\n");
}


int sendFile(const char* filename,int socketfd)
{
	signal(SIGPIPE,sigfunc_PIPE);

	int ret=0;//调用结果

	int fd = open(filename,O_RDWR);

	stProtoTrain train;
	memset(&train,0,sizeof(train));

	//1. 先发送文件名
	train.iLength=strlen(filename);//整形的数据长度成员，4字节
	strcpy(train.data,filename);
	send(socketfd,&train,4+train.iLength,0);

	//2. 发送文件大小
	struct stat filestat;
	memset(&filestat,0,sizeof(filestat));
	fstat(fd,&filestat);

	train.iLength=sizeof(filestat.st_size);
	memcpy(&train.data,&filestat.st_size,train.iLength);
	send(socketfd,&train,4+train.iLength,0);

	printf("file size: %ld bytes confirmed\n",filestat.st_size);

	

	//3. 发送文件内容
	int ret1=0;
	while(1)
	{
		printf("while send\n");

		ret = read(fd,train.data,sizeof(train.data));
		train.iLength=ret;
		
		ret1 = send(socketfd,&train,4+train.iLength,0);
		
		if(0==ret// zero indicates end of file
			|| -1 ==ret1) // error
		{
			printf("done\n");
			break;
		}
	}

	close(fd);
	return 0;

}