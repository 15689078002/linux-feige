/* ************************************************************************
 *       Filename:  main.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2010年10月09日 22时17分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/
#include "myinclude.h"
#include "communication.h"
#include "user_manager.h"
#include "user_interface.h"
#include "mytcp.h"

int main(int argc, char *argv[])
{
	pthread_t tid;	
	sysinit("zc", "root_zc");	
	//用户界面线程，处理用户输入的命令
	pthread_create(&tid, NULL, user_interface, NULL);	
	//接收消息线程，接收其他客户端发送的UDP数据
	pthread_create(&tid, NULL, recv_msg_thread, NULL);
	//tcp_server线程
	pthread_create(&tid, NULL, tcp_server, NULL);
	//主线程不能退出
	pthread_join(tid, NULL);	
	return 0;	
}


/* ************************************************************************
 *       Filename:  main.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2010年10月09日 22时17分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/
#include "myinclude.h"
#include "communication.h"
#include "user_manager.h"
#include "user_interface.h"
#include "mytcp.h"

int main(int argc, char *argv[])
{
	pthread_t tid;	
	sysinit("zc", "root_zc");	
	//用户界面线程，处理用户输入的命令
	pthread_create(&tid, NULL, user_interface, NULL);	
	//接收消息线程，接收其他客户端发送的UDP数据
	pthread_create(&tid, NULL, recv_msg_thread, NULL);
	//tcp_server线程
	pthread_create(&tid, NULL, tcp_server, NULL);
	//主线程不能退出
	pthread_join(tid, NULL);	
	return 0;	
}


/* ************************************************************************
 *       Filename:  main.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2010年10月09日 22时17分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/
#include "myinclude.h"
#include "communication.h"
#include "user_manager.h"
#include "user_interface.h"
#include "mytcp.h"

int main(int argc, char *argv[])
{
	pthread_t tid;	
	sysinit("zc", "root_zc");	
	//用户界面线程，处理用户输入的命令
	pthread_create(&tid, NULL, user_interface, NULL);	
	//接收消息线程，接收其他客户端发送的UDP数据
	pthread_create(&tid, NULL, recv_msg_thread, NULL);
	//tcp_server线程
	pthread_create(&tid, NULL, tcp_server, NULL);
	//主线程不能退出
	pthread_join(tid, NULL);	
	return 0;	
}


/* ************************************************************************
 *       Filename:  main.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2010年10月09日 22时17分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/
#include "myinclude.h"
#include "communication.h"
#include "user_manager.h"
#include "user_interface.h"
#include "mytcp.h"

int main(int argc, char *argv[])
{
	pthread_t tid;	
	sysinit("zc", "root_zc");	
	//用户界面线程，处理用户输入的命令
	pthread_create(&tid, NULL, user_interface, NULL);	
	//接收消息线程，接收其他客户端发送的UDP数据
	pthread_create(&tid, NULL, recv_msg_thread, NULL);
	//tcp_server线程
	pthread_create(&tid, NULL, tcp_server, NULL);
	//主线程不能退出
	pthread_join(tid, NULL);	
	return 0;	
}


