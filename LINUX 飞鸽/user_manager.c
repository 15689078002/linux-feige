/* ************************************************************************
 *       Filename:  online_list.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2010年10月09日 22时21分20秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/
 
#include "myinclude.h"
#include "user_manager.h"

static pUSR stc_head = NULL;
 

//向用户列表中添加一个用户节点
void add_user(USR user_node)
{
	pUSR p=stc_head, new_node=NULL;
	
	while(p != NULL)
	{
		if(p->ip == user_node.ip)	//用户是否已经在链表中
			return;
		p = p->next;	
	}
	new_node=(USR *)malloc(sizeof(USR));
	strcpy(new_node->usr_name, user_node.usr_name);
	strcpy(new_node->host_name, user_node.host_name);
	new_node->ip = user_node.ip;	
	new_node->next=NULL;
	
	order_user(new_node);
}

//链表是按usr_name的值，从小到大排序的，
//此函数将新节点添加到链表的适当位置
void order_user(pUSR new_node)
{
	pUSR cur_node = stc_head;
	pUSR last_node = stc_head;
	
	if(NULL == stc_head)
	{
		stc_head = new_node;
	}
	else if(strcmp(new_node->usr_name, stc_head->usr_name)<0)
	{
		new_node->next = stc_head;
		stc_head = new_node;
	}
	else
	{
		while((strcmp(new_node->usr_name, cur_node->usr_name)>0)
					&& (cur_node->next!=NULL))
		{
			last_node = cur_node;
			cur_node = cur_node->next;
		}
		if(cur_node->next != NULL)
		{
			last_node->next = new_node;
			new_node->next = cur_node;
		}
		else
		{
			cur_node->next = new_node;
			new_node->next = NULL;
		}
	}
}


//从用户列表中删除一个用户节点
void dele_user(USR user_node)
{
	USR *p1=stc_head,*p2=NULL;
	if(stc_head==NULL)
		return ;
	else
	{
		while((strcmp(p1->usr_name,user_node.usr_name)!=0)&&(p1->next!=NULL))
		{
			p2=p1;
			p1=p1->next;
		}
		if(strcmp(p1->usr_name,user_node.usr_name)==0)
		{
			if(p1==stc_head)
				stc_head=p1->next;
			else 
				p2->next=p1->next;
			free(p1);
		}
	}
}
//从用户链表中删除一个用户节点
/*void dele_user(USR user_node)
{
	pUSR cur_node = stc_head;
	pUSR last_node = NULL;
	
	if(NULL == stc_head)
	{
		printf("list have been empty!");
	}
	else if(!strcmp(stc_head->usr_name, user_node.usr_name))
	{
		stc_head = stc_head->next;
		free(cur_node);
	}
	else
	{
		while(cur_node != NULL)
		{
			last_node = cur_node;
			if(!strcmp(cur_node->usr_name, user_node.usr_name))
			{
				last_node->next = cur_node->next;
				free(cur_node);
			}
			cur_node = cur_node->next;
		}
	}
}*/

//find 指定用户
pUSR find_user(char *username)
{
	pUSR cur_node = stc_head;
	//下面两个条件表达是不能调换，否则当用户不在线时，会出现段错误
	while( (cur_node!=NULL) && (strcmp(cur_node->usr_name, username)) )
		cur_node = cur_node->next;
	if(NULL == cur_node)
	{
		IPMSG_OUT_MSG_COLOR(printf("此用户不在线\n");)
		printf("\n");
    }
	return cur_node;
}

//打印用户列表
void list(void)
{
	pUSR p=stc_head;
	IPMSG_OUT_MSG_COLOR(
	printf("%10s\t%10s\t%s\n", "Username","Hostname","IP Address");
	)
	while(p!=NULL)
	{
		struct in_addr addr = {p->ip};
		IPMSG_OUT_MSG_COLOR(
		printf("%10s\t%10s\t%s\n",p->usr_name,p->host_name,inet_ntoa(addr));
		)
		p=p->next;
	}	
}

//释放链表
void free_link(void)
{
	pUSR cur_node = stc_head;
	pUSR last_node = stc_head;	
	
	while(cur_node != NULL)
	{
		cur_node = cur_node->next;
		free(last_node);
		last_node = cur_node;
	}
}
