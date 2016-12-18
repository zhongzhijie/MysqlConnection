// SqlTest.cpp : 定义控制台应用程序的入口点。
//
/*********************************************************************


**********************************************************************/

#include "stdafx.h"
#include "winsock.h"
#include "mysql.h"

#define INSERT_STR "insert into %s (name, score, sex, age, tel, city) values (%s, %d, %s, %d, %s, %s)"
#define SELECT_STR "select * from %s"

int main()
{
	MYSQL * con;
	MYSQL_RES *res;
	MYSQL_ROW row;

	char tmp[400];
	int rt;

	/* database configuration */
	char dbuser[16] = "root";			//user name of database
	char dbpasswd[16] = "";				//password of database
	char dbip[16] = "localhost";		//host name of database server
	char dbname[16] = "students";		//database name
	char dbtable[16] = "studentinfo";	//table name
	char *query = NULL;					//pointer to string which is to be exected

	con = mysql_init((MYSQL*)0);

	/* connect to database */
	if (NULL != con && mysql_real_connect(con, dbip, dbuser, dbpasswd, dbname, 3306, NULL, 0))
	{
		printf("connect to database %s succeed!\r\n", dbname);
		con->reconnect = 1;

		query = "set names \'GBK\'";
		rt = mysql_real_query(con, query, strlen(query));

		if (rt)
		{
			printf("Error making query: %s\r\n", mysql_error(con));
		}
		else
		{
			printf("query %s succeed!\r\n", query);
		}
	}
	else
	{
		MessageBoxA(NULL, "unable to connect to database!", "Warning", NULL);
	}
	
	/* make up a sql string */
	sprintf_s(tmp, INSERT_STR, dbtable, "'name1'", 98, "'男'", 25, "'15678987654'", "'beijing'");

	/* execute the sql string */
	rt = mysql_real_query(con, tmp, strlen(tmp));
	if (rt)
	{
		printf("executed <%s> error: %s\r\n", tmp, mysql_error(con));
	}
	else
	{
		printf("executed <%s> succeed!\r\n", tmp);
	}

	sprintf_s(tmp, SELECT_STR, dbtable);
	rt = mysql_real_query(con, tmp, strlen(tmp));
	if (rt)
	{
		printf("executed <%s> error: %s\r\n", tmp, mysql_error(con));
	}
	else
	{
		printf("executed <%s> succeed!\r\n", tmp);
	}

	/* restore the result of sql select */
	res = mysql_store_result(con);

	printf("%-4s%-12s%-12s%-12s%-12s%-12s%-12s\r\n", "id", "name", "score", "sex", "age", "tel", "city");
	/* get row of result */
	while (row = mysql_fetch_row(res))
	{
		printf("%-4s", row[0]);
		for (unsigned int i = 1; i < mysql_num_fields(res); ++i)
		{
			printf("%-12s", row[i]);
		}
		printf("\r\n");
	}

	/* free the result */
	mysql_free_result(res);
	/* close the database */
	mysql_close(con);

    return 0;
}
