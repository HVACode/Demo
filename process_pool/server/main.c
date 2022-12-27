#include "../include/head.h"
#include "threadpool.h"
#include "taskqueue.h"

int sigPipe[2];
void sigfunc_INT(int signum)
{
    printf("SIGINT activated\n");
    write(sigPipe[1],&signum,sizeof(int));
}

/// @brief 
/// @param argc 
/// @param argv ./thread_pool ip port threadNum 
/// @return 
int main(int argc,char* argv[])
{
    pipe(sigPipe);
    if(fork()){
        signal(SIGINT,sigfunc_INT);
        wait(NULL);
        printf("process exit\n");
        exit(0);
    }
    
    /// @brief 任务监听线程的个数
    int numThreads=atoi(argv[3]);

    stThreadpool pool;
    memset(&pool,0,sizeof(pool));

    threadpoolInit(&pool,numThreads);

    threadpoolCreateThds(&pool);

    int sfd = 0;
    tcpInit(argv[1],argv[2],&sfd);

    // 监听SIGINT管道及套接字
    int epfd=epoll_create(1);
    epollAdd(epfd,sfd);
    epollAdd(epfd,sigPipe[0]);

    int iReadyNum=0;
    struct epoll_event evs[2];
    memset(evs,0,sizeof(evs));

    int nsfd = 0;
    while(1)
    {
        iReadyNum = epoll_wait(epfd,evs,2,-1);
        for(int i=0;i<iReadyNum;++i)
        {
            if(evs[i].data.fd == sfd){
                nsfd=accept(sfd,NULL,NULL);

                pstTaskNode task = (pstTaskNode)calloc(1,sizeof(stTaskNode));
                task->sockfd=nsfd;
                pthread_mutex_lock(&pool.taskQueue.threadMutex);
                TaskQueueInsert(&pool.taskQueue,task);
                pthread_cond_broadcast(&pool.taskQueue.threadCond);
                pthread_mutex_unlock(&pool.taskQueue.threadMutex);
            }
            else //收到SIGINT
            {
                for(int i=0;i<numThreads;++i)
                {
                    pthread_cancel(pool.thids[i]);
                }
                for(int i=0;i<numThreads;++i)
                {
                    pthread_join(pool.thids[i],NULL);
                }
                printf("Threads exited\n");
            }
        }
    }

    return 0;
}