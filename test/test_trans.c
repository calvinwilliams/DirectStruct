#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "IDL_CommHead.dsc.h"
#include "IDL_BankTransaction.dsc.h"

#include "IDL_CommHead.dsc.LOG.c"
#include "IDL_BankTransaction.dsc.LOG.c"

static void dump( char *buf , int len )
{
	int		i , j ;
	
	for( i = 0 ; i < len / 16 + 1 ; i++ )
	{
		printf( "0x%010X" , i );
		
		printf( " " );
		
		for( j = 0 ; j < 16 ; j++ )
		{
			if( i*16+j<len )
			{
				printf( " %02x" , (unsigned char)*(buf+i*16+j) );
			}
			else
			{
				printf( "   " );
			}
		}
		
		printf( " " );
		
		for( j = 0 ; j < 16 ; j++ )
		{
			if( i*16+j<len )
			{
				if( isprint(*(buf+i*16+j)) )
					putchar( *(buf+i*16+j) );
				else
					putchar( '.' );
			}
			else
			{
				putchar( ' ' );
			}
		}
		
		putchar( '\n' );
	}
	
	return;
}

int test()
{
	CommHead		head ;
	BankTransaction		trans ;
	char			buf[ DSCSERIALIZE_COMPACT_BUFSIZE_CommHead + DSCSERIALIZE_COMPRESS_BUFSIZE_BankTransaction + 1 ] ;
	int			len , total_len ;
	
	int			nret = 0 ;
	
	/* client code */
	DSCINIT_BankTransaction( & trans );
	
	strcpy( trans.ResponseHeader.transaction_code , "P0101" );
	trans.ResponseHeader.trans_jnlsno = 12345678 ;
	trans.ResponseHeader.response_code = 0 ;
	strcpy( trans.ResponseHeader.response_desc , "OK" );
	
	strcpy( trans.QueryTransactionDetails.AddonMessages[1].message_text , "Check channel passed" );
	strcpy( trans.QueryTransactionDetails.AddonMessages[0].message_text , "Check account passed" );
	
	strcpy( trans.QueryTransactionDetails.TransactionDetailTitle.title_text , "DETAIL TITLE" );
	trans.QueryTransactionDetails.TransactionDetailTitle.page_no = 1 ;
	trans.QueryTransactionDetails.TransactionDetailTitle.page_size = 2 ;
	
	strcpy( trans.QueryTransactionDetails.TransactionDetails[0].trans_date , "2014-01-01" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails[0].trans_time , "08:01:01" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails[0].outlet_id , "1001" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails[0].card_no , "603367123412341234" );
	trans.QueryTransactionDetails.TransactionDetails[0].trans_amount = 100.00 ;
	
	strcpy( trans.QueryTransactionDetails.TransactionDetails[1].trans_date , "2014-02-02" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails[1].trans_time , "08:02:02" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails[1].outlet_id , "2002" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails[1].card_no , "603367123412341234" );
	trans.QueryTransactionDetails.TransactionDetails[1].trans_amount = 200.00 ;
	
	DSCLOG_BankTransaction( & trans );
	
	memset( buf , 0x00 , sizeof(buf) );
	total_len = 0 ;
	
	nret = DSCSERIALIZE_COMPRESS_BankTransaction( & trans , buf + DSCSERIALIZE_COMPACT_BUFSIZE_CommHead , & len ) ;
	if( nret )
	{
		printf( "DSCSERIALIZE_COMPRESS_BankTransaction failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCSERIALIZE_COMPRESS_BankTransaction ok , len[%d]\n" , len );
	}
	total_len += len ;
	
	head.length = len ;
	nret = DSCSERIALIZE_COMPACT_CommHead( & head , buf , & len ) ;
	if( nret )
	{
		printf( "DSCSERIALIZE_COMPACT_CommHead failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCSERIALIZE_COMPACT_CommHead ok , len[%d]\n" , len );
	}
	total_len += len ;
	
	dump( buf , total_len );
	
	/* ... client send communication struct to server ... */
	
	/* server code */
	memset( & head , 0x00 , sizeof(CommHead) );
	nret = DSCDESERIALIZE_COMPACT_CommHead( buf , & len , & head ) ;
	if( nret )
	{
		printf( "DSCDESERIALIZE_COMPACT_CommHead failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCDESERIALIZE_COMPACT_CommHead ok\n" );
	}
	
	DSCLOG_CommHead( & head );
	
	memset( & trans , 0x00 , sizeof(BankTransaction) );
	nret = DSCDESERIALIZE_COMPRESS_BankTransaction( buf + DSCSERIALIZE_COMPACT_BUFSIZE_CommHead , & len , & trans ) ;
	if( nret )
	{
		printf( "DSCDESERIALIZE_COMPRESS_BankTransaction failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCDESERIALIZE_COMPRESS_BankTransaction ok\n" );
	}
	
	DSCLOG_BankTransaction( & trans );
	
	return 0;
}

int main()
{
	return -test();
}
