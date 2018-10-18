/* ************************************************************************
 *       Filename:  user_interface.h
 *    Description:  
 *        Version:  1.0
 *        Created:  2010年10月10日 11时10分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/
#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

//文件属性结构体
typedef struct file_info{
	int msgnum;
	int filenum;
	int offset;
	long filesize;
	long filectime;
	long filemode;
	int ip;
	char filename[100];
}FINFO;

//命令函数指针
typedef void (*FUN)(int argc, char *argv[]);

//命令结构体
typedef struct cmd
{
	char *name;		//命令名称
	FUN fun;		//命令处理函数
}CMD;

//人机交互线程，处理用户输入的命令
void *user_interface(void *arg);

#endif	



