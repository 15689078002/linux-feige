/* ************************************************************************
 *       Filename:  online_list.h
 *    Description:  
 *        Version:  1.0
 *        Created:  2010年10月10日 11时09分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/
#ifndef USER_MANAGER_H_
#define USER_MANAGER_H_

//用户信息结构体
typedef struct usr_info{
	char usr_name[40];
	char host_name[40];
	unsigned int ip;
	struct usr_info *next;
}USR,*pUSR;


//向用户列表中添加一个用户节点
void add_user(USR user_node);

//链表是按usr_name的值，从小到大排序的，
//此函数将新节点添加到链表的适当位置
void order_user(pUSR new_node);

//从用户链表中删除一个用户节点
void dele_user(USR user_node);

//搜索指定用户
pUSR find_user(char *username);

//打印用户列表
void list(void);

//释放链表
void free_link(void);

#endif



