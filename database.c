#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <mysql/mysql.h>
#include "msg.h"
#include "database.h"

extern MYSQL mysql;
extern MYSQL_RES *result;
extern MYSQL_ROW  row;

void db_register(Message *msg) //传入结构体指针&msg
{
    char sqlStr[1024]={0};
    char uName[32]={0};
    char pWord[32]={0};

    sscanf(msg->content, "%[^|]|%s", uName, pWord);
    sprintf(sqlStr, "insert into user(u_name, u_password) values('%s', '%s')", uName, pWord);
    printf("sqlStr=%s\n", sqlStr);  
    if(mysql_query(&mysql, sqlStr)!=0){ //发送请求，接受结果
        printf("%s\n",mysql_error(&mysql));
	    return -1; //查询失败
    }
    else{
        //注册成功
        memset(sqlStr, 0, sizeof(sqlStr));
        sprintf(sqlStr, "select last_insert_id() from user");
        puts(sqlStr);
        if(mysql_query(&mysql, sqlStr)!=0){ //发送请求，接受结果
            printf("%s\n",mysql_error(&mysql));
	        return -1; //查询失败
        }
        else{
            //查询ID
            result = mysql_store_result(&mysql); //存储查询结果集
	        if(result == NULL){
		        printf("%s\n",mysql_error(&mysql));
		        return -1;
	        }
            if(row=mysql_fetch_row(result)){ //从结果集中取一行，每个数组表示一列
                //转换为四位数字字符串
                for(int i=0; i<4-strlen(row[0]); i++){
                    msg->content[i]='0';
                }
                strcpy(msg->content+4-strlen(row[0]), row[0]);
            }
        }
    }
}

void db_login(Message *msg)
{
    char uID[32]={0};
    for(int i=0; i<4; i++){
        uID[i]=msg->content[i];
    }
    char sqlStr[1024]={0};


    sprintf(sqlStr, "select u_password from user where u_id=%s", uID);
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
        mysql_free_result(result);
        strcpy(msg->content, row[0]);
    } 
    else{
        memset(msg->content, 0, sizeof(msg->content));
    }
}

void db_list(Message *msg)
{
    char uID[32]={0};
    for(int i=0; i<4; i++){
        uID[i]=msg->content[i];
    }
    char sqlStr[1024]={0};


    sprintf(sqlStr, "select f_id1, f_type1, f_type2 from friend where f_id2=%s union select f_id2, f_type2, f_type1 from friend where f_id1=%s", uID, uID);
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
        sprintf(tmp, "%s,%s,%s|", row[0], row[1], row[2]);//好友id，好友在该用户列表中的类型，该用户在好友列表中的类型
        strcat(msg->content, tmp);  
    }
    mysql_free_result(result);
}

void db_addf(Message *msg)
{
    char sqlStr[1024]={0};

    sprintf(sqlStr, "insert into friend(f_id1, f_id2) values(%s, %s)", msg->sendName, msg->recvName);
    puts(sqlStr);
    if(mysql_query(&mysql, sqlStr)!=0){
        printf("%s\n",mysql_error(&mysql));
	    return -1;
    }
}

void db_delf(Message *msg)
{
    char sqlStr[1024]={0};

    sprintf(sqlStr, "delete from friend where f_id1=%s and f_id2=%s union delete from friend where f_id2=%s and f_id1=%s", msg->sendName, msg->recvName);
    puts(sqlStr);
    if(mysql_query(&mysql, sqlStr)!=0){
        printf("%s\n",mysql_error(&mysql));
	    return -1;
    }
}