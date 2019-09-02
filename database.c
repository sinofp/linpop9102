#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <mysql/mysql.h>


typedef struct slist{
    int num;
    char **friend;
    char **type;
}list;

int user_register(char *buf)
{
    //"0001|123"
    MYSQL mysql; //数据库连接句柄，包含数据库连接状态
    MYSQL_RES *result; //返回行的查询结果
    MYSQL_ROW row;

    char uName[32]={0};
    char pWord[32]={0};
    //拆包函数
    sscanf(buf, "%[^|]|%s", uName, pWord);
    //int sscanf(const char *str, const char *format, ...)
    char sqlStr[1024]={0};

    mysql_init(&mysql); //初始化mysql结构，取地址
    if(mysql_real_connect(&mysql, "localhost", "root", NULL, "chatroom", 0, NULL, 0)==NULL){
        printf("%s\n",mysql_error(&mysql));
        return -1; //连接失败
    }

    sprintf(sqlStr, "insert into user(u_name, u_password) values('%s', '%s')", uName, pWord);
    printf("sqlStr=%s\n", sqlStr);  
    if(mysql_query(&mysql, sqlStr)!=0){ //发送请求，接受结果
        printf("%s\n",mysql_error(&mysql));
	    return -1; //查询失败
    }
    else{
        printf("注册成功\n");
        //用传入指针buf把注册自动生成的id传回去
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
                memset(buf, 0, sizeof(buf));
                strcpy(buf, row[0]);
                //通过传入指针buf，把注册时生成的登陆账户传回去
            } 
        }
        return 200;
    }
}


int user_login(char *buf)
{
    //"0001|123"
    MYSQL mysql; //数据库连接句柄，包含数据库连接状态
    MYSQL_RES *result; //返回行的查询结果
    MYSQL_ROW row;

    char uID[32]={0};
    char pWord[32]={0};
    
    sscanf(buf, "%[^|]|%s", uID, pWord);
    char sqlStr[1024]={0};


    sprintf(sqlStr, "select u_password from user where u_id=%s", uID);
    printf("sqlStr=%s\n", sqlStr);

    mysql_init(&mysql); //初始化mysql结构
    if(mysql_real_connect(&mysql, "localhost", "root", NULL, "chatroom", 0, NULL, 0)==NULL){
        printf("%s\n", mysql_error(&mysql));
        return -1;
    }
    //连接到mysql服务器
    //MYSQL *mysql_real_connect (MYSQL *mysql,const char *host,const char *user, const char *passwd, const char *db, unsigned int port,const char *unix_socket,unsigned long client_flag)

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
        if(strcmp(row[0], pWord)==0){ //传首地址
            printf("登录成功\n");
            //获取好友列表
            memset(sqlStr, 0, sizeof(sqlStr));
            sprintf(sqlStr, "select f_id1, f_type1 from friend where f_id2=%s union select f_id2, f_type2 from friend where f_id1=%s", uID, uID);
            puts(sqlStr);
            if(mysql_query(&mysql, sqlStr)!=0){
                printf("%s\n",mysql_error(&mysql));
	            return -1;
            }
            
            result = mysql_store_result(&mysql);
            if(result == NULL){
                printf("%s\n",mysql_error(&mysql));
		        return -1;
	        }
            //结构体存储查询结果
            list *flist;
            flist=(list*)malloc(sizeof(list));
            flist->num=result->row_count;
            flist->friend=(char**)malloc(flist->num * sizeof(char*));
            flist->type=(char**)malloc(flist->num * sizeof(char*));
            for(int i=0; i<flist->num; i++){
                row=mysql_fetch_row(result);
                flist->friend[i]=(char*)malloc(10);
                flist->type[i]=(char*)malloc(20);
                strcpy(flist->friend[i], row[0]);
                strcpy(flist->type[i], row[1]);
                puts(flist->friend[i]);
                puts(flist->type[i]);
            }
            mysql_free_result(result);
            //把flist指针传回去
            //？？？？？
            return 200;
        }
        else{
            printf("登录失败\n");
            return 400;
        }
    } 
    else{
        printf("用户不存在\n");
        return 404;
    }
}


int user_logout(char *buf)
{
    //"0001"
    MYSQL mysql; //数据库连接句柄，包含数据库连接状态
    MYSQL_RES *result; //返回行的查询结果
    MYSQL_ROW row;

    char uID[32]={0};
    char sqlStr[1024]={0};

    strcpy(uID, buf);

    mysql_init(&mysql); //初始化mysql结构
    if(mysql_real_connect(&mysql, "localhost", "root", NULL, "chatroom", 0, NULL, 0)==NULL){
        printf("%s\n", mysql_error(&mysql));
        return -1;
    }
    //连接到mysql服务器
    //MYSQL *mysql_real_connect (MYSQL *mysql,const char *host,const char *user, const char *passwd, const char *db, unsigned int port,const char *unix_socket,unsigned long client_flag)

    sprintf(sqlStr, "select f_id1 from friend where f_id2=%s union select f_id2 from friend where f_id1=%s", uID, uID);
    if(mysql_query(&mysql, sqlStr)!=0){
        printf("%s\n",mysql_error(&mysql));
	    return -1;
    }

    result = mysql_store_result(&mysql); //存储查询结果集
	if(result == NULL){
		printf("%s\n",mysql_error(&mysql));
		return -1;
	}

    list *flist;
    flist=(list*)malloc(sizeof(list));
    flist->num=result->row_count;
    flist->friend=(char**)malloc(flist->num * sizeof(char*));
    for(int i=0; i<flist->num; i++){
        row=mysql_fetch_row(result);
        flist->friend[i]=(char*)malloc(10);
        strcpy(flist->friend[i], row[0]);
        puts(flist->friend[i]);
    }
    //返回指针
}

