int addf(Message *msg)
{
    char fID1[32]={0};
    char fID2[32]={0};
    char fType1[32]={0};
    char fType2[32]={0};
    char sqlStr[1024]={0};

    sscanf(msg->content, "%[^|]|%[^|]|%[^|]|%s", fID1, fID2, fType1, fType2);
    sprintf(sqlStr, "insert into friend(f_id1, f_id2, f_type1, f_type2) values(%s, %s, '%s', '%s')", fID1, fID2, fType1, fType2);
    puts(sqlStr);
    if(mysql_query(&mysql, sqlStr)!=0){
        printf("%s\n",mysql_error(&mysql));
	    return -1;
    }
}