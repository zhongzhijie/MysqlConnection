// SqlTest.cpp : 定义控制台应用程序的入口点。
//
/*********************************************************************
数据表格式如下：
id(int) name(char) score(int) sex(char) age(int) tel(char) city(char)

可以用bit位来表示打印哪一些内容，比如
01000000表示只打印id
01100000表示打印id和name，以此类推
**********************************************************************/

#include "stdafx.h"
#include "winsock.h"
#include "mysql.h"

#define INSERT_STR "insert into %s (name, score, sex, age, tel, city) values (%s, %d, %s, %d, %s, %s)"
#define SELECT_STR "select * from %s"
#define UPDATE_STR "update %s set score = %d, city = %s where name = %s"
#define DELETE_STR "delete from %s where name = %s OR id >= %d"

/* 根据比特位决定打印哪些列 */
#define ID_FLAG		64
#define NAME_FLAG	32
#define SCORE_FLAG	16
#define SEX_FLAG	8
#define AGE_FLAG	4
#define TEL_FLAG	2
#define CITY_FLAG	1

#define COLUM_COUNT	7
#define COLUM_NAME_MAXLEN	10

void printData(MYSQL *con, unsigned char bitFlag)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char columName[COLUM_COUNT][COLUM_NAME_MAXLEN] = { "id", "name", "score", "sex", "age", "tel", "city"};
	unsigned char columFlag[COLUM_COUNT] = {ID_FLAG, NAME_FLAG, SCORE_FLAG, SEX_FLAG, AGE_FLAG, TEL_FLAG, CITY_FLAG};
	
	for (unsigned int i = 0; i < COLUM_COUNT; ++i)
	{
		if (bitFlag & columFlag[i])
		{
			/* id sex 和age只打印列宽5 */
			if (0 == i || 3 == i || 4 == i)
				printf("%-5s", columName[i]);
			else
				printf("%-12s", columName[i]);
		}
	}
	printf("\r\n");

	/* restore the result of sql select */
	res = mysql_store_result(con);

	/* get row of result */
	while (row = mysql_fetch_row(res))
	{
		/* mysql_num_fields 如果改变，相应的COLUM_COUNT也应该修改 */
		for (unsigned int i = 0; i < mysql_num_fields(res); ++i)
		{
			if (columFlag[i] & bitFlag)
			{
				if (0 == i || 3 == i || 4 == i)
					printf("%-5s", row[i]);
				else
					printf("%-12s", row[i]);
			}
		}
		printf("\r\n");
	}

	/* free the result */
	mysql_free_result(res);
}

int main()
{
	MYSQL * con;
	/*MYSQL_RES *res;
	MYSQL_ROW row;*/

	char tmp[400];
	int rt;
	unsigned char printFlag = 0;

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

	printFlag |= ID_FLAG | NAME_FLAG | SCORE_FLAG | SEX_FLAG | AGE_FLAG | TEL_FLAG | CITY_FLAG;
	printData(con, printFlag);

	sprintf_s(tmp, UPDATE_STR, dbtable, 80, "'nowhere'", "'zzj'");
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
	printData(con, printFlag);

	sprintf_s(tmp, DELETE_STR, dbtable, "'name1'", 10);
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
	printData(con, printFlag);

	/* close the database */
	mysql_close(con);

    return 0;
}
