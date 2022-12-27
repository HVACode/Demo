#include "../include/head.h"

// ./client ip port
int main(int argc,char *argv[])
{
	int ret=0;
	int sfd=socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in serverAddr;
	memset(&serverAddr,0,sizeof(serverAddr));

	serverAddr.sin_addr.s_addr=inet_addr(argv[1]);
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(atoi(argv[2]));

	connect(sfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

	char buf[1000]={0};
	int iDataLen=0;

	//1. 文件名
	recv(sfd,&iDataLen,4,0);
	recv(sfd,buf,iDataLen,0);

	printf("start recv file: %s ...\n",buf);
	

	//2. 文件大小
	off_t filesize = 0;
	off_t recvLen = 0;//累计接受数据量
	recv(sfd,&iDataLen,4,0);
	recv(sfd,&filesize,iDataLen,0);

	printf("file with size %ld bytes\n",filesize);

	float bar = 0;//进度条
	struct timeval begin,end;
	memset(&begin,0,sizeof(begin));
	memset(&end,0,sizeof(end));
	gettimeofday(&begin,NULL);

	time_t t1=0;
	time_t t2=0;

	int fd = open(buf,O_RDWR|O_CREAT,0666);
	while(1)
	{
		memset(buf,0,sizeof(buf));
		//数据长度
		ret=recv(sfd,&iDataLen,4,0);
		if(1000!=iDataLen)
		{
			printf("datalen: %d\n",iDataLen);
		}
		if(0==iDataLen)
		{
			break;
		}
		//数据内容
		ret=recv(sfd,buf,iDataLen,MSG_WAITALL);

		recvLen=ret+recvLen;
		bar=(float)recvLen/filesize*10;

		time(&t2);
		if(t2-t1>0.5){
			printf("bar: %5.2f\r",bar);
			fflush(stdout);
			t1=t2;
		}
		write(fd,buf,ret);

	}
	gettimeofday(&end,NULL);
    	printf("cost time=%ld us\n",(end.tv_sec-begin.tv_sec)*1000000+end.tv_usec-begin.tv_usec);
    	return 0;
}