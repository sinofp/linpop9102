int list(Message *msg)
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