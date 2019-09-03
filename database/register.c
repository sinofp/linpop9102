int register(Message *msg) //传入结构体指针&msg
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