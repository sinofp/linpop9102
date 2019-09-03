int login(Message *msg)
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