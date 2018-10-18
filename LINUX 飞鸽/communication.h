/* ************************************************************************
 *       Filename:  communication.h
 *    Description:  
 *        Version:  1.0
 *        Created:  2010年10月10日 11时12分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/

#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "myinclude.h"

#define PORT 2425  //该程序的端口号

/*下面几个函数都是让其他文件可以使用某个内
部连接的变量，在其他文件对这些变量赋值*/
int udp_fd(void);
char *user(void);
char *host(void);

/*系统初始化函数*/
void sysinit(char *user, char *host);

//接收消息线程，接收其他客户端发送的UDP数据
void *recv_msg_thread(void *arg);

#endif	//_COMMUNICATION_H_




