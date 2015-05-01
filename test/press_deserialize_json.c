#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#include "IDL_BankJsonTransaction.dsc.h"
#include "IDL_BankJsonTransaction.dsc.LOG.c"

static long _GetFileSize(char *filename)
{
	struct stat stat_buf;
	int ret;

	ret=stat(filename,&stat_buf);
	
	if( ret == -1 )
		return -1;
	
	return stat_buf.st_size;
}

static int ReadEntireFile( char *filename , char *mode , char *buf , long *bufsize )
{
	FILE	*fp = NULL ;
	long	filesize ;
	long	lret ;
	
	if( filename == NULL )
		return -1;
	if( strcmp( filename , "" ) == 0 )
		return -1;
	
	filesize = _GetFileSize( filename ) ;
	if( filesize  < 0 )
		return -2;
	
	fp = fopen( filename , mode ) ;
	if( fp == NULL )
		return -3;
	
	if( filesize <= (*bufsize) )
	{
		lret = fread( buf , sizeof(char) , filesize , fp ) ;
		if( lret < filesize )
		{
			fclose( fp );
			return -4;
		}
		
		(*bufsize) = filesize ;
		
		fclose( fp );
		
		return 0;
	}
	else
	{
		lret = fread( buf , sizeof(char) , (*bufsize) , fp ) ;
		if( lret < (*bufsize) )
		{
			fclose( fp );
			return -4;
		}
		
		fclose( fp );
		
		return 1;
	}
}

static int ReadEntireFileSafely( char *filename , char *mode , char **pbuf , long *pbufsize )
{
	long	filesize ;
	
	int	nret ;
	
	filesize = _GetFileSize( filename );
	
	(*pbuf) = (char*)malloc( filesize + 1 ) ;
	if( (*pbuf) == NULL )
		return -1;
	memset( (*pbuf) , 0x00 , filesize + 1 );
	
	nret = ReadEntireFile( filename , mode , (*pbuf) , & filesize ) ;
	if( nret )
	{
		free( (*pbuf) );
		(*pbuf) = NULL ;
		return nret;
	}
	else
	{
		if( pbufsize )
			(*pbufsize) = filesize ;
		return 0;
	}
}

int main( int argc , char *argv[] )
{
	
	if( argc == 1 + 2 )
	{
		char			*json_buffer = NULL ;
		char			*json_buffer_bak = NULL ;
		int			json_len ;
		int			c , count ;
		BankJsonTransaction	trans ;
		int			nret = 0 ;
		
		nret = ReadEntireFileSafely( argv[1] , "rb" , & json_buffer_bak , NULL ) ;
		if( nret )
		{
			printf( "ReadEntireFileSafely[%s] failed[%d]\n" , argv[1] , nret );
			return nret;
		}
		json_buffer = strdup( json_buffer_bak ) ;
		if( json_buffer == NULL )
		{
			printf( "strdup failed , errno[%d]\n" , errno );
			return 1;
		}
		json_len = strlen(json_buffer) ;
		
		count = atol(argv[2]) ;
		if( count > 0 )
		{
			time_t	t1 , t2 ;
			
			time( & t1 );
			for( c = 0 ; c < count ; c++ )
			{
				memset( & trans , 0x00 , sizeof(BankJsonTransaction) );
				nret = DSCDESERIALIZE_JSON_BankJsonTransaction( NULL , json_buffer , & json_len , & trans ) ;
				if( nret )
				{
					printf( "DSCDESERIALIZE_JSON_BankJsonTransaction failed[%d]\n" , nret );
					return nret;
				}
			}
			time( & t2 );
			printf( "Elapse %d seconds\n" , (int)(t2-t1) );
			
			DSCLOG_BankJsonTransaction( & trans );
		}
		else
		{
			int	len ;
			
			count = -count ;
			len = 0 ;
			for( c = 0 ; c < count ; c++ )
			{
				len += strlen( json_buffer + c % 10 ) ;
			}
			printf( "len[%d]\n" , len );
		}
		free( json_buffer );
		free( json_buffer_bak );
	}
	else
	{
		printf( "USAGE : press_fastjson json_pathfilename press_count\n" );
		exit(7);
	}
	
	return 0;
}

#if 0
int press_serialize_json()
{
	BankJsonTransaction	trans ;
	char			buf[ 40960 + 1 ] ;
	int			len ;
	int			nret = 0 ;
	
	char			*p = NULL ;
	
	/* client code */
	DSCINIT_BankJsonTransaction( & trans );
	
	strcpy( trans.ResponseHeader.transaction_code , "P0101" );
	trans.ResponseHeader.trans_jnlsno = 12345678 ;
	trans.ResponseHeader.response_code = 0 ;
	strcpy( trans.ResponseHeader.response_desc , "OK" );
	strcpy( trans.QueryTransactionDetails.AddonMessages.AddonMessage[1].message_text , "Check channel passed" );
	strcpy( trans.QueryTransactionDetails.AddonMessages.AddonMessage[0].message_text , "Check account passed" );
	strcpy( trans.QueryTransactionDetails.TransactionDetailTitle.title_text , "DETAIL TITLE" );
	trans.QueryTransactionDetails.TransactionDetailTitle.page_no = 1 ;
	trans.QueryTransactionDetails.TransactionDetailTitle.page_size = 2 ;
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[0].trans_date , "2014-01-01" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[0].trans_time , "08:01:01" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[0].outlet_id , "1001" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[0].card_no , "603367123412341234" );
	trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[0].trans_amount = 100.00 ;
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[1].trans_date , "2014-02-02" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[1].trans_time , "08:02:02" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[1].outlet_id , "2002" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[1].card_no , "603367123412341234" );
	trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[1].trans_amount = 200.00 ;
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[2].card_no , "\\\"" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[3].card_no , "\"你\"他\"" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[4].card_no , "我\"你\"他\"她" );
	trans.QueryTransactionDetails.TransactionDetails._TransactionDetail_count = 5 ;
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionTailDetail[0].message_text , "uu0040uu0049uu004Auu004F" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionTailDetail[1].message_text , "uu4049uu4a4f" );
	
	DSCLOG_BankJsonTransaction( & trans );
	
	memset( buf , 0x00 , sizeof(buf) );
	len = sizeof(buf)-1 ;
	nret = DSCSERIALIZE_JSON_BankJsonTransaction( & trans , "GBK" , buf , & len ) ;
	if( nret )
	{
		printf( "DSCSERIALIZE_JSON_BankJsonTransaction failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCSERIALIZE_JSON_BankJsonTransaction ok , len[%d]\n" , len );
	}
	
	/* heihei */
	p = buf ;
	while(1)
	{
		p = strstr( p , "uu" ) ;
		if( p == NULL )
			break;
		*(p) = '\\' ;
		p += 2 ;
	}
	
	printf( "[%s]\n" , buf );
	
	/* ... client send communication struct to server ... */
	
	/* server code */
	memset( & trans , 0x00 , sizeof(BankJsonTransaction) );
	nret = DSCDESERIALIZE_JSON_BankJsonTransaction( NULL , buf , & len , & trans ) ;
	if( nret )
	{
		printf( "DSCDESERIALIZE_JSON_BankJsonTransaction failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCDESERIALIZE_JSON_BankJsonTransaction ok\n" );
	}
	
	DSCLOG_BankJsonTransaction( & trans );
	
	memset( buf , 0x00 , sizeof(buf) );
	len = sizeof(buf)-1 ;
	nret = DSCSERIALIZE_JSON_COMPACT_BankJsonTransaction( & trans , "GBK" , buf , & len ) ;
	if( nret )
	{
		printf( "DSCSERIALIZE_JSON_COMPACT_BankJsonTransaction failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCSERIALIZE_JSON_COMPACT_BankJsonTransaction ok , len[%d]\n" , len );
	}
	
	printf( "[%s]\n" , buf );
	
	/* ... client send communication struct to server ... */
	
	/* server code */
	memset( & trans , 0x00 , sizeof(BankJsonTransaction) );
	nret = DSCDESERIALIZE_JSON_COMPACT_BankJsonTransaction( NULL , buf , & len , & trans ) ;
	if( nret )
	{
		printf( "DSCDESERIALIZE_JSON_COMPACT_BankJsonTransaction failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCDESERIALIZE_JSON_COMPACT_BankJsonTransaction ok\n" );
	}
	
	DSCLOG_BankJsonTransaction( & trans );
	
	return 0;
}

int main()
{
	g_fasterjson_encoding = FASTERJSON_ENCODING_GB18030 ;
	
	return -test_serialize_json();
}
#endif

