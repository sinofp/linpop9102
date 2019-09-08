#include "msg.h"
#include <mysql/mysql.h>

//#ifndef __DATABASE_H__
//#define __DATABASE_H__
   extern MYSQL mysql;
   extern MYSQL_RES *result;
   extern MYSQL_ROW  row;
   int db_register(Message *msg);
   int db_login(Message *msg);
   int db_list(Message *msg);
   int db_addf(Message *msg);
   int db_delf(Message *msg);
   int db_mvf(Message *msg);
//#endif