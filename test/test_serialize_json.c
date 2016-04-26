#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "IDL_BankJsonTransaction.dsc.h"
#include "IDL_BankJsonTransaction.dsc.LOG.c"

int test_serialize_json()
{
	BankJsonTransaction	trans ;
	char			buf[ 4096 + 1 ] ;
	char			*p = NULL ;
	int			len ;
	char			*base = NULL ;
	int			buf_size ;
	int			nret = 0 ;
	
	/* client code */
	DSCINIT_BankJsonTransaction( & trans );
	
	strcpy( trans.ResponseHeader.transaction_code , "P0101" );
	trans.ResponseHeader.trans_jnlsno = 12345678 ;
	trans.ResponseHeader.response_code = 0 ;
	trans.null_string[0] = DSCNULL ;
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
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[2].card_no , "\\\"\t\r\n\f\b" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[3].card_no , "\"你\"他\"" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[4].card_no , "我\"你\"他\"她" );
	trans.QueryTransactionDetails.TransactionDetails._TransactionDetail_count = 5 ;
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionTailDetail[0].message_text , "uu0040uu0049uu004Auu004F" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionTailDetail[1].message_text , "uu4049uu4a4f" );
	trans.OtherTransJnlsno[0]._trans_jnlsno = 20150417 ;
	trans.OtherTransJnlsno[1]._trans_jnlsno = 20150423 ;
	trans.OtherTransJnlsno[2]._trans_jnlsno = 20150429 ;
	trans.OtherTransJnlsno[3]._trans_jnlsno = 20150430 ;
	trans.OtherTransJnlsno[4]._trans_jnlsno = 20150501 ;
	trans.OtherTransJnlsno[5]._trans_jnlsno = 20150503 ;
	trans._OtherTransJnlsno_count = 6 ;
	trans.sign_flag = DSCTRUE ;
	
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

	base = NULL ;
	nret = DSCSERIALIZE_JSON_DUP_BankJsonTransaction( & trans , "GBK" , & base , NULL , NULL ) ;
	if( nret )
	{
		printf( "DSCSERIALIZE_JSON_DUP_BankJsonTransaction failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCSERIALIZE_JSON_DUP_BankJsonTransaction ok\n" );
	}
	
	printf( "DUP base[%d][%s]\n" , (int)strlen(base) , base );
	
	free( base );
	
	base = NULL ;
	buf_size = 100 ;
	len = 0 ;
	nret = DSCSERIALIZE_JSON_DUP_BankJsonTransaction( & trans , "GBK" , & base , & buf_size , & len ) ;
	if( nret )
	{
		printf( "DSCSERIALIZE_JSON_DUP_BankJsonTransaction failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCSERIALIZE_JSON_DUP_BankJsonTransaction ok , len[%d]\n" , len );
	}
	
	printf( "DUP base[%d][%d][%.*s]\n" , buf_size , len , len , base );
	
	memset( base , 0x00 , buf_size );
	base[0] = ' ' ;
	len = buf_size-1 - 1 ;
	nret = DSCSERIALIZE_JSON_DUP_COMPACT_BankJsonTransaction( & trans , "GBK" , & base , & buf_size , & len ) ;
	if( nret )
	{
		printf( "DSCSERIALIZE_JSON_DUP_COMPACT_BankJsonTransaction failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCSERIALIZE_JSON_DUP_COMPACT_BankJsonTransaction ok , len[%d]\n" , len );
	}
	
	printf( "DUP_COMPACT base[%d][%d][%.*s]\n" , buf_size , len , len , base );
	
	free( base );
	
	return 0;
}

int main()
{
	g_fasterjson_encoding = FASTERJSON_ENCODING_GB18030 ;
	
	return -test_serialize_json();
}
