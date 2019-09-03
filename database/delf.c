int delf(Message *msg)
{
    char fID1[32]={0};
    char fID2[32]={0};
    char sqlStr[1024]={0};

    sscanf(msg->content, "%[^|]|%s", fID1, fID2);
    sprintf(sqlStr, "delete from friend where f_id1=%s and f_id2=%s", fID1, fID2);
    puts(sqlStr);
    if(mysql_query(&mysql, sqlStr)!=0){
        printf("%s\n",mysql_error(&mysql));
	    return -1;
    }
}