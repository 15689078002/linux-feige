/* ************************************************************************
 *       Filename:  myinclude.h
 *    Description:  
 *        Version:  1.0
 *        Created:  2010年10月10日 14时27分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/
#ifndef _MYINCLUDE_H_
#define _MYINCLUDE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ipmsg.h"


#define IPMSG_OUT_MSG_COLOR(x)   { \
write(1, "\033[31m", 5);\
x \
write(1, "\033[32m", 5);}

#endif//_MYINCLUDE_H_



