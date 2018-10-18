/* ************************************************************************
 *       Filename:  tcp.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2010年10月11日 07时19分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/
#include "myinclude.h"
#include "mytcp.h"
#include "user_interface.h"
#include "communication.h"


extern FINFO send_info[];
extern FINFO recv_buf[];


/*发送文件*/
static void send_file(int socket_fd, char *msg_buf)
{
	int t,msgnum,filenum,offset; //msgnum(包编号),filenum(文件序号),offset(偏移量)
	char user[20];
	char host[20];
	char send_buf[1024]="\0";    //发送文件内容缓冲区
	int len;
	int i;
	
	if(read(socket_fd, msg_buf, 1000)!=0)//读取IPMSG_GETFILEDATA包文
	{
		//解析包文，msgnum,filenum,这些有用的信号提取出来
		sscanf(msg_buf,"1:%d:%[^:]:%[^:]:96:%X:%d:%d:",&t,user,host,
				&msgnum,&filenum,&offset);
				
		//循环搜索要发送的文件		
		for(i=0; i<5; i++)
		{ 
		    //判断文件是否为要发送的文件
			if(send_info[i].msgnum==msgnum && send_info[i].filenum==filenum)
			{
				//打开文件
				int send_fd;
				if((send_fd=open(send_info[i].filename, O_RDONLY))<0)
					IPMSG_OUT_MSG_COLOR(perror("open");)
				
				//循环读取发送的文件，并将其写入套接字里	
				while((len = read(send_fd, send_buf, 1024))>0)
					write(socket_fd, send_buf, len);
					
				//文件发送完毕，关闭文件描述户	
				close(send_fd);	
				break;
			}
		}
		
		//判断指定发送的文件是否存在，因为接收方传送数据
		//时可能会出错
		if(5 == i)
		{	
			IPMSG_OUT_MSG_COLOR(printf("没找到要发送的文件\n\n");)
			write(1,"\rIPMSG:",7); //打印命令提示
			fflush(stdout);	
		}		
	}	
}


/*tcp服务器线程，用于发送文件*/
void *tcp_server(void *data)
{
	int tcpfd; //tcp套接字
	unsigned short port = 2425; //端口号
	//初始化本地网络地址
	struct sockaddr_in severaddr = {AF_INET};
	severaddr.sin_port = htons(port);
	severaddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY为0

	//获取套接字
	if((tcpfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		IPMSG_OUT_MSG_COLOR(perror("tcpsocket");)
	//绑定套接字	
	if(bind(tcpfd, (struct sockaddr *)&severaddr, sizeof(severaddr)))
		IPMSG_OUT_MSG_COLOR(perror("tcpbind");)
	//监听客户端	
	if(listen(tcpfd, 0)<0)
		IPMSG_OUT_MSG_COLOR(perror("listen");)
			
	while(1)
	{
		pid_t pid;
		char buf[1000];
		int cfd;
		struct sockaddr_in cliaddr = {AF_INET};
		socklen_t addrlen = sizeof(cliaddr);

		//取出客户连接队列中的一个连接，没有连接，则等待
		cfd = accept(tcpfd, (struct sockaddr *)&cliaddr, &addrlen);
		
		//创建进程，发送文件
		pid = fork();
		if(pid<0)
			IPMSG_OUT_MSG_COLOR(perror("fork");)
		if(pid==0)
		{
			send_file(cfd, buf);//发送文件
			close(cfd);         //子进程关闭客户套接口
			exit(0);            //退出进程
		}
		close(cfd);             //父进程关闭客户套接口
	}
	return NULL;
}


/*tcp客户端函数,用于接收文件*/
void tcp_client(int num, char *path)
{
	char buf[1024]="";
	char pathbuf[50];             //保存文件存放路径
	int t = time((time_t *)NULL); //获取包编号
	int tcpfd;                    //tcp客户端套接字
	int fd;                       //保存下面打开文件描述符
	long filesize=0;              //文件大小计数器  
	struct sockaddr_in seraddr = {AF_INET};  //初始化网络地址
	seraddr.sin_port = htons(2425);
	seraddr.sin_addr.s_addr = (in_addr_t)recv_buf[num].ip;
	
	//将IPMSG_GETFILEDATA包文写入到buf
	int len = sprintf(buf,"1:%d:%s:%s:%ld:%x:%x:%x", t,user(),
			  host(),IPMSG_GETFILEDATA,recv_buf[num].msgnum,
			  recv_buf[num].filenum,recv_buf[num].offset);
	
	//获取客户端套接字
	if((tcpfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		IPMSG_OUT_MSG_COLOR(perror("tcpsocket");)
		
	//连接到目标机器的服务器
	if(connect(tcpfd, (struct sockaddr *)&seraddr, sizeof(seraddr)))
		IPMSG_OUT_MSG_COLOR(perror("connect");)
	
	//发送报文，由于是tcp协议，所以可以用write代替sendto	
	write(tcpfd, buf, len);
	
	//打开文件，接收文件
	strcpy(pathbuf,path);//没有这句，下面一句的执行会出现段错误
	strcat(pathbuf, recv_buf[num].filename); //按指定路径打开文件
	if( (fd=open(pathbuf, O_WRONLY|O_CREAT|O_APPEND,777))<0 ) //创建、追加方式打开
		IPMSG_OUT_MSG_COLOR(perror("fd_open");)
	while( (len=read(tcpfd, buf, 1024))>0 ) //从套接字接收文件
	{
		filesize += len;	   //记录文件大小 
		write(fd, buf, len);   //写入文件
		if(len < 1024)         //判断文件接收完
			close(tcpfd);      //关闭tcpfd，否则循环不会结束，read阻塞
	}

	//判断文件是否接收成功，打印提示
	if(recv_buf[num].filesize == filesize) 
		printf("%s 文件接收成功!\n", recv_buf[num].filename);
	else
		IPMSG_OUT_MSG_COLOR(
		printf("%s 文件接收失败!\n", recv_buf[num].filename);)
		
	close(fd);
}



