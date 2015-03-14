#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "IDL_userinfo.dsc.h"
#include "IDL_userinfo.dsc.ESQL.eh"
#include "IDL_userinfo.dsc.LOG.c"

int test_client()
{
	userinfo		u ;
	
	char			json_buffer[ 1024 + 1 ] ;
	int			json_len ;
	
	FILE			*fp = NULL ;
	
	int			nret ;
	
	DSCDBCONN( "127.0.0.1" , 18432 , "calvin" , "calvin" , "calvin" );
	if( SQLCODE )
	{
		printf( "CONNECT failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return 1;
	}
	else
	{
		printf( "CONNECT ok\n" );
	}
	
	DSCINITV_userinfo();
	
	EXEC SQL
		SELECT	*
		INTO	DBVLLIST_userinfo
		FROM	userinfo
		WHERE	user_id = 1001 ;
	if( SQLCODE )
	{
		printf( "SELECT failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return 1;
	}
	else
	{
		printf( "SELECT ok\n" );
	}
	
	DSCDBDISCONN();
	
	DSCVTOS_userinfo( & u );
	
	memset( json_buffer , 0x00 , sizeof(json_buffer) );
	nret = DSCSERIALIZE_JSON_userinfo( & u , "GBK" , json_buffer , & json_len ) ;
	if( nret )
	{
		printf( "DSCSERIALIZE_JSON_userinfo failed[%d] , errno[%d]\n" , nret , errno );
		return -1;
	}
	
	fp = fopen( "test_db2json.json" , "w" ) ;
	if( fp == NULL )
	{
		printf( "fopen failed , errno[%d]\n" , errno );
		return -1;
	}
	
	fwrite( json_buffer , 1 , json_len , fp );
	
	fclose( fp );
	
	memset( json_buffer , 0x00 , sizeof(json_buffer) );
	nret = DSCSERIALIZE_JSON_COMPACT_userinfo( & u , "GBK" , json_buffer , & json_len ) ;
	if( nret )
	{
		printf( "DSCSERIALIZE_JSON_COMPACT_userinfo failed[%d] , errno[%d]\n" , nret , errno );
		return -1;
	}
	
	fp = fopen( "test_db2json_compact.json" , "w" ) ;
	if( fp == NULL )
	{
		printf( "fopen failed , errno[%d]\n" , errno );
		return -1;
	}
	
	fwrite( json_buffer , 1 , json_len , fp );
	
	fclose( fp );
	
	return 0;
}

int main()
{
	return -test_client();
}

