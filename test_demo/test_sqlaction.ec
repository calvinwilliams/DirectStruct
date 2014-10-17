#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "IDL_userinfo.dsc.h"
#include "IDL_userinfo.dsc.ESQL.eh"
#include "IDL_userinfo.dsc.LOG.c"

#if ( defined _ORACLE )
EXEC SQL BEGIN DECLARE SECTION ;
	char	DBUSER[ 128 + 1 ] ;
	char	DBPASS[ 128 + 1 ] ;
EXEC SQL END DECLARE SECTION ;
#endif

int test_sqlaction()
{
	userinfo		u ;
	
#if ( defined _PQSQL )
	EXEC SQL
		CONNECT 	"calvin@127.0.0.1:18432"
		USER		"calvin"
		IDENTIFIED BY	"calvin" ;
#elif ( defined _ORACLE )
	strcpy( DBUSER , "hzbsbdb" );
	strcpy( DBPASS , "hzbsbdb" );
	EXEC SQL
		CONNECT 	:DBUSER
		IDENTIFIED BY	:DBPASS ;
#endif
	if( SQLCODE )
	{
		printf( "CONNECT failed[%d][%s]\n" , SQLCODE , SQLSTATE );
		return 1;
	}
	else
	{
		printf( "CONNECT ok\n" );
	}
	
#if ( defined _PQSQL )
	EXEC SQL
		BEGIN WORK ;
	if( SQLCODE )
	{
		printf( "BEGIN WORK failed[%d][%s]\n" , SQLCODE , SQLSTATE );
		return 1;
	}
	else
	{
		printf( "BEGIN WORK ok\n" );
	}
#endif
	
	memset( & u , 0x00 , sizeof(userinfo) );
	u.user_id = 1001 ;
	strcpy( u.user_name , "calvin" );
	strcpy( u.email , "calvinwilliams.c@gmail.com" );
	DSCSQLACTION_INSERT_INTO_userinfo( & u );
	if( SQLCODE )
	{
		printf( "DSCSQLACTION_INSERT_INTO_userinfo failed , SQLCODE[%d][%s]\n" , SQLCODE , SQLSTATE );
		goto E;
	}
	else
	{
		printf( "DSCSQLACTION_INSERT_INTO_userinfo ok\n" );
	}
	
	memset( & u , 0x00 , sizeof(userinfo) );
	u.user_id = 1002 ;
	strcpy( u.user_name , "calvin2" );
	strcpy( u.email , "calvin2williams.c@gmail.com" );
	DSCSQLACTION_INSERT_INTO_userinfo( & u );
	if( SQLCODE )
	{
		printf( "DSCSQLACTION_INSERT_INTO_userinfo failed , SQLCODE[%d][%s]\n" , SQLCODE , SQLSTATE );
		goto E;
	}
	else
	{
		printf( "DSCSQLACTION_INSERT_INTO_userinfo ok\n" );
	}
	
	memset( & u , 0x00 , sizeof(userinfo) );
	u.user_id = 1003 ;
	strcpy( u.user_name , "calvin3" );
	strcpy( u.email , "calvin3williams.c@gmail.com" );
	DSCSQLACTION_INSERT_INTO_userinfo( & u );
	if( SQLCODE )
	{
		printf( "DSCSQLACTION_INSERT_INTO_userinfo failed , SQLCODE[%d][%s]\n" , SQLCODE , SQLSTATE );
		goto E;
	}
	else
	{
		printf( "DSCSQLACTION_INSERT_INTO_userinfo ok\n" );
	}
	
	memset( & u , 0x00 , sizeof(userinfo) );
	u.user_id = 1001 ;
	strcpy( u.user_name , "calvinn" );
	strcpy( u.email , "calvinnwilliams.c@gmail.com" );
	DSCSQLACTION_UPDATE_userinfo_SET_A_WHERE_user_id_E( & u );
	if( SQLCODE )
	{
		printf( "DSCSQLACTION_UPDATE_userinfo_SET_A_WHERE_user_id_E failed , SQLCODE[%d][%s]\n" , SQLCODE , SQLSTATE );
		goto E;
	}
	else
	{
		printf( "DSCSQLACTION_UPDATE_userinfo_SET_A_WHERE_user_id_E ok\n" );
	}
	
	memset( & u , 0x00 , sizeof(userinfo) );
	strcpy( u.email , "calvinnnwilliams.c@gmail.com" );
	DSCSQLACTION_UPDATE_userinfo_SET_email( & u );
	if( SQLCODE )
	{
		printf( "DSCSQLACTION_UPDATE_userinfo_SET_email failed , SQLCODE[%d][%s]\n" , SQLCODE , SQLSTATE );
		goto E;
	}
	else
	{
		printf( "DSCSQLACTION_UPDATE_userinfo_SET_email ok\n" );
	}
	
	memset( & u , 0x00 , sizeof(userinfo) );
	u.user_id = 1003 ;
	DSCSQLACTION_DELETE_FROM_userinfo_WHERE_user_id_GE( & u );
	if( SQLCODE )
	{
		printf( "SCSQLACTION_DELETE_FROM_userinfo_WHERE_user_id_GE failed , SQLCODE[%d][%s]\n" , SQLCODE , SQLSTATE );
		goto E;
	}
	else
	{
		printf( "DSCSQLACTION_DELETE_FROM_userinfo_WHERE_user_id_GE ok\n" );
	}
	
	memset( & u , 0x00 , sizeof(userinfo) );
	u.user_id = 1001 ;
	DSCSQLACTION_SELECT_user_name_email_FROM_userinfo_WHERE_user_id_E( & u );
	if( SQLCODE )
	{
		printf( "DSCSQLACTION_SELECT_user_name_email_FROM_userinfo_WHERE_user_id_E failed , SQLCODE[%d][%s]\n" , SQLCODE , SQLSTATE );
		goto E;
	}
	else
	{
		printf( "DSCSQLACTION_SELECT_user_name_email_FROM_userinfo_WHERE_user_id_E ok\n" );
	}
	
	DSCLOG_userinfo( & u );
	
	memset( & u , 0x00 , sizeof(userinfo) );
	u.user_id = 1001 ;
	DSCSQLACTION_OPEN_CURSOR_mycursor3_SELECT_A_FROM_userinfo_WHERE_user_id_GE_ORDER_BY_user_id_ASC( & u );
	if( SQLCODE )
	{
		printf( "DSCSQLACTION_OPEN_CURSOR_mycursor3_SELECT_A_FROM_userinfo_WHERE_user_id_GE_ORDER_BY_user_id_ASC failed , SQLCODE[%d][%s]\n" , SQLCODE , SQLSTATE );
		goto E;
	}
	else
	{
		printf( "DSCSQLACTION_OPEN_CURSOR_mycursor3_SELECT_A_FROM_userinfo_WHERE_user_id_GE_ORDER_BY_user_id_ASC ok\n" );
	}
	
	while(1)
	{
		memset( & u , 0x00 , sizeof(userinfo) );
		DSCSQLACTION_FETCH_CURSOR_mycursor3( & u );
		if( SQLCODE == SQLNOTFOUND )
		{
			break;
		}
		else if( SQLCODE )
		{
			printf( "DSCSQLACTION_FETCH_CURSOR_mycursor3 failed , SQLCODE[%d][%s]\n" , SQLCODE , SQLSTATE );
			goto E;
		}
		else
		{
			printf( "DSCSQLACTION_FETCH_CURSOR_mycursor3 ok\n" );
		}
			
		DSCLOG_userinfo( & u );
	}
	
	DSCSQLACTION_CLOSE_CURSOR_mycursor3();
	printf( "DSCSQLACTION_CLOSE_CURSOR_mycursor3 ok\n" );
	
E :
	
	if( SQLCODE )
	{
		EXEC SQL
			ROLLBACK WORK ;
		if( SQLCODE )
		{
			printf( "ROLLBACK WORK failed[%d][%s]\n" , SQLCODE , SQLSTATE );
		}
		else
		{
			printf( "ROLLBACK WORK ok\n" );
		}
	}
	else
	{
		EXEC SQL
			COMMIT WORK ;
		if( SQLCODE )
		{
			printf( "COMMIT WORK failed[%d][%s]\n" , SQLCODE , SQLSTATE );
		}
		else
		{
			printf( "COMMIT WORK ok\n" );
		}
	}
	
#if ( defined _PQSQL )
	EXEC SQL
		DISCONNECT ;
	if( SQLCODE )
	{
		printf( "DISCONNECT failed[%d][%s]\n" , SQLCODE , SQLSTATE );
		return 1;
	}
	else
	{
		printf( "DISCONNECT ok\n" );
	}
#endif
	
	return 0;
}

int main()
{
	return -test_sqlaction();
}

