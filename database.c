#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <mysql/mysql.h>
#include "msg.h"
#include "database.h"

int db_register(Message *msg) //传入结构体指针&msg
{
    char sqlStr[1024]={0};

    sprintf(sqlStr, "insert into user(u_name, u_password) values('%s', '%s')", msg->sendName, msg->content);
    printf("sqlStr=%s\n", sqlStr);  
    if(mysql_query(&mysql, sqlStr)!=0){ //发送请求，接受结果
        printf("%s\n",mysql_error(&mysql));
	    return -1; //查询失败
    }
}

int db_login(Message *msg)
{
    char sqlStr[1024]={0};

    sprintf(sqlStr, "select u_password from user where u_name='%s'", msg->sendName);
    printf("sqlStr=%s\n", sqlStr);

    if(mysql_query(&mysql, sqlStr)!=0){
        printf("%s\n",mysql_error(&mysql));
	    return -1;
    }

    result = mysql_store_result(&mysql); //存储查询结果集
	if(result == NULL){
		printf("%s\n",mysql_error(&mysql));
		return -1;
	}
    if(row=mysql_fetch_row(result)){ //从结果集中取下一行
        strcpy(msg->content, row[0]);
    } 
    else{
        memset(msg->content, 0, sizeof(msg->content)); //用户不存在
    }
    mysql_free_result(result);
}

int db_list(Message *msg)
{
    char sqlStr[1024]={0};

    sprintf(sqlStr, "select f_name1, f_type1 from friend where f_name2='%s' union select f_name2, f_type2 from friend where f_name1='%s'", msg->sendName, msg->sendName);
    if(mysql_query(&mysql, sqlStr)!=0){
        printf("%s\n",mysql_error(&mysql));
	    return -1;
    }
            
    result = mysql_store_result(&mysql);
    if(result == NULL){
        printf("%s\n",mysql_error(&mysql));
		return -1;
	}

    char tmp[100]={0};
    memset(msg->content, 0, sizeof(msg->content));
    for(int i=0; i<result->row_count; i++){
        row=mysql_fetch_row(result);
        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "%s,%s|", row[0], row[1]);//好友姓名，好友在该用户列表中的类型
        strcat(msg->content, tmp);
    }
    mysql_free_result(result);
}

int db_addf(Message *msg)
{
    char sqlStr[1024]={0};
    sprintf(sqlStr, "insert into friend(f_name1, f_name2) values('%s', '%s')", msg->sendName, msg->recvName);
    //puts(sqlStr);
    if(mysql_query(&mysql, sqlStr)!=0){
        printf("%s\n",mysql_error(&mysql));
	    return -1;
    }
}

int db_delf(Message *msg)
{
    char sqlStr[1024]={0};

    sprintf(sqlStr, "delete from friend where f_name1='%s' and f_name2='%s'" ,msg->sendName, msg->recvName);
    if(mysql_query(&mysql, sqlStr)!=0){
        printf("%s\n",mysql_error(&mysql));
	    return -1;
    }

    sprintf(sqlStr, "delete from friend where f_name2='%s' and f_name1='%s'", msg->sendName, msg->recvName);
    if(mysql_query(&mysql, sqlStr)!=0){
        printf("%s\n",mysql_error(&mysql));
	    return -1;
    }
}

int db_mvf(Message *msg)
{
    char sqlStr[1024]={0};

    sprintf(sqlStr, "update friend set f_type2=%s where f_name1='%s' and f_name2='%s'", msg->content, msg->sendName, msg->recvName);
    if(mysql_query(&mysql, sqlStr)!=0){
        printf("%s\n",mysql_error(&mysql));
	    return -1;
    }

    sprintf(sqlStr, "update friend set f_type1=%s where f_name2='%s' and f_name1='%s'", msg->content, msg->sendName, msg->recvName);
    if(mysql_query(&mysql, sqlStr)!=0){
        printf("%s\n",mysql_error(&mysql));
	    return -1;
    }
}