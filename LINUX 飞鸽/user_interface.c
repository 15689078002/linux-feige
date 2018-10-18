/* ************************************************************************
 *       Filename:  user_interface.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2010年10月10日 11时10分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/

#include "myinclude.h"
#include "user_manager.h"
#include "communication.h"
#include "user_interface.h"
#include "mytcp.h"


FINFO send_info[5];         //存放发送文件属性信息
extern FINFO recv_buf[];    //存放接收文件属性信息


//帮助菜单
static char *help= "*************************************\n"\
			"* sentfile    :    发送文件                *\n"\
			"* recvfile    :    接收文件                *\n"\
			"* say         :    发送消息                *\n"\
			"* list        :    打印用户列表            *\n"\
			"* cls         :    清平                    *\n"\
			"* help        :    帮助                    *\n"\
			"* exit        :    退出                    *\n"\
			"********************************************\n"
			;



/*打印出上线用户表*/			
void list_fun(int argc, char *argv[])
{
	list();
	printf("\n");
}


/*帮助命令*/
void help_fun(int argc, char *argv[])
{
	//打印帮助菜单
	printf("%s", help);
	printf("\n");	
}


/*退出命令*/
void exit_fun(int argc, char *argv[])
{
	//释放链表的所有节点，还没通报下线
	free_link();
	exit(0);
}


/*清屏命令*/
void cls_fun(int argc, char *argv[])
{
	write(1, "\033[2J\033[0;0H", 10);
}


/*发送消息命令argv[1]为目标用户名
argv[2]为用户消息，缺点是只能发送
一条不带空格的消息*/
void say_fun(int argc, char *argv[])
{
	char buf[100]="";
	struct sockaddr_in addr = {AF_INET}; //存放目标网络地址
	int t = time((time_t *)NULL);        //获取包编号
	//在用户表中搜索指定用户，pUSR结构在user_manager.h 中定义
	pUSR temp = find_user(argv[1]);	
	
	//初始化网络地址
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = (in_addr_t)temp->ip;
	
	//组建消息包文
	int len = sprintf(buf,"1:%d:%s:%s:%ld:%s", \
					  t,user(),host(),IPMSG_SENDMSG,argv[2]);
	
	//发送	
	sendto(udp_fd(), buf, len, 0, (struct sockaddr*)&addr,sizeof(addr));
	printf("\n");
}


/*发送文件命令argv[1]为发送对象的用户名,argv[2]
为发送的消息，不能为空,argv[3]-argv[7]存放要发
送的文件名，本函数的缺点是:是只能一次发送5个文
件，对方没接收完，不能再发送，否则会将上次保存
在send_info中的信息覆盖*/
void sendfile_fun(int argc, char *argv[])
{
	//存放文件属性，此结构在/usr/include/bits/stat.h中定义，
	//本函数用了其st_mode,st_size, st_ctime成员
	struct stat attr_buf; 
	char buf[1000]="";                   //存放报文
	struct sockaddr_in addr = {AF_INET}; //存放目标网络地址
	int t = time((time_t *)NULL);        //获取包编号
	pUSR temp = find_user(argv[1]);      //找到要发送的对象
	if(temp == NULL)					 //用户是否在线
		return;
	int i = 3;   						 //i要初始化为3
	
	//组建部分包文（包括：版本号、包编号、用户名、主机名、
	//命令字、消息、消息后面的‘\0’）
	int len = sprintf(buf,"1:%d:%s:%s:%ld:%s%c", t,user(),
				host(),IPMSG_SENDMSG|IPMSG_FILEATTACHOPT
				|IPMSG_READCHECKOPT|IPMSG_SENDCHECKOPT
				|IPMSG_SECRETOPT,argv[2],0);
	//regflag为普通文件标志数，本函数没有检查文件是否存在的功能			
	int regflag = strlen(buf)+1;//不要忘了加1	
	
	//初始化网络地址
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = (in_addr_t)temp->ip;
	
	//每循环一次添加一个文件的属性到buf,send_info中，buf用于存放
	//包文，send_info为文件属性表，方便tcp服务索引要发送的指定文件
	for(i=3; i<argc; i++)
	{
		if(stat(argv[i], &attr_buf)<0)      //调用stat函数获取文件argv[i]
		IPMSG_OUT_MSG_COLOR(perror("stat");)//的属性,保存在attr_buf中	   
		if(S_ISREG(attr_buf.st_mode))       //argv[i]是否为普通文件
		{
		    //如果是普通文件，添加该文件相关属性到buf中，用'\a'隔开
			len += sprintf(buf+len,"%d:%s:%lx:%lx:%lx:%c", 
							i,argv[i],attr_buf.st_size,
							attr_buf.st_ctime,IPMSG_FILE_REGULAR,'\a'); 
			//将该包文的包编号(t)、本文件的文件序号(i)、文件名
			//(argv[i])存于send_info中的某个元素中，		
			send_info[i-3].msgnum = t;
			send_info[i-3].filenum = i;
			strcpy(send_info[i-3].filename, argv[i]);
		}
		else
		{
		    //否则打印出错信息
			IPMSG_OUT_MSG_COLOR(printf("%s属非普通文件 \n",argv[i]);)
			printf("\n");
		}
	}
	//判断所有要发送的文件中是否有普通文件，有则发送包文，否则不发送	
	if(len != regflag)
		sendto(udp_fd(), buf, len, 0, (struct sockaddr*)&addr,sizeof(addr));
	printf("\n");	
}


/*接收文件命令函数,本函数的缺本发送命令函数相关
argv[2]为接收文件名，argv[3]、argv[4]为可选项，
用于指定路径，其中argv[3]为“to”，不选，则为默认
路径，*/
void recvfile_fun(int argc, char *argv[])
{
	int i,j;
	
	//在接收文件列表中搜索指定接收的文件
	for(i=0; i<5; i++)
	{
		//按文件名搜索，（最好用包编号与文件序号搜索）
		//但是本程序的缺陷，使得无法这样搜索，如果要
		//这样做，会使用户命令很复杂
		if( !strcmp(recv_buf[i].filename, argv[1]) )
		{
			if(argc == 2)             //是使用默认路径
				argv[3] = "./down/";  //默认路径
				
			//连接服务器，接收文件	
			tcp_client(i, argv[3]); 
			
			//将已接收的文件信息中的包编号清零，标志文件已接收
			recv_buf[i].msgnum=0;
			
			//打印还未接收的文件信息
			printf("等待接收的文件:");
			for(j=0; j<5; j++)
			{
				if(recv_buf[j].msgnum != 0)
					printf("  %s",recv_buf[j].filename);
			}
			printf("\n\n");
			
			//打印命令提示符
			write(1,"\rIPMSG:",7);             
			fflush(stdout);
			//接收完退出循环，实际效果会退本函数	
			break;
		}
	}
	
	//判断指定接收的文件是否在接收文件列表中，
	//如果存在，i不会等于5
	if(5 == i) 
	{
	    //打印相关信息
		IPMSG_OUT_MSG_COLOR(printf("无法接收文件%s\n", argv[1]);)
		
		//打印还未接收的文件信息
		printf("等待接收的文件:");
		for(i=0; i<5; i++)
		{
			if(recv_buf[i].msgnum != 0)
				printf("  %s",recv_buf[i].filename);
		}
		printf("\n\n");
		
		//打印命令提示符
		write(1,"\rIPMSG:",7);             
		fflush(stdout);
	}
}


//命令数组：用来保存 命令名 和 处理函数名
CMD cmdlist[]={	
				{"say", say_fun},			// 添加命令
				{"sendfile",sendfile_fun},  //发送文件
				{"recvfile",recvfile_fun},  //接收文件
				{"list", list_fun},		    // 添加命令
				{"help", help_fun},		    // 帮助命令
				{"exit", exit_fun},		    // 退出
				{"cls", cls_fun}			// 清屏命令
			  };
			
			
/*分析、搜索、执行命令*/
int exec_cmd(char *cmd)
{
	char *argv[10] = {NULL}; //用于存放命令行参数    
	int argc = 0;            //记录命令行参数的个数 
	int i = 0;
	if(strlen(cmd)==0)       //判断是否有命令输入
		return 0;
		
	//以空格或'\t'切割命令行字符串，将切出的参数按顺序存放在argv数组中	
	argv[0] = cmd;
	while((argv[argc]=strtok(argv[argc], " \t"))!=NULL) argc++;

	//循环查找命令
	for (i=0;i<sizeof(cmdlist)/sizeof(CMD);i++)
	{
		//判断是否为要差找的命令
		if (strcmp(cmdlist[i].name, argv[0])==0)
		{
			//执行命令
			cmdlist[i].fun(argc, argv);
			return 0;
		}
	}	
	return -1;
}			


/*人机交互线程*/			
void *user_interface(void *arg)
{
	//设置标准输出设备的的前景色，设置为Green
	write(1, "\033[32m", 5); 

	
	while(1)
	{
		char buf[100]="";
		write(1,"\rIPMSG:",7);         //打印输入命令提示
		fgets(buf, sizeof(buf), stdin);//获取命令字符串，注意用sizeof，而不用常量
		buf[strlen(buf)-1]=0;          //去掉buf后面的'\n'
		if(exec_cmd(buf) < 0)          //调用函数解析、执行命令，并判断命令是否存在
		{
			IPMSG_OUT_MSG_COLOR(
			printf("找不到命令!\n");
			)
		}
	}
	return NULL;
}


