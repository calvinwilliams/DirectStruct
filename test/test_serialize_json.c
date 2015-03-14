#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "IDL_BankJsonTransaction.dsc.h"
#include "IDL_BankJsonTransaction.dsc.LOG.c"

int test_serialize_json()
{
	BankJsonTransaction	trans ;
	char			buf[ 40960 + 1 ] ;
	int			len ;
	int			nret = 0 ;
	
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
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[2].card_no , "\"" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[3].card_no , "\"��\"��\"" );
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[4].card_no , "��\"��\"��\"��" );
	
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
	
	return 0;
}

int main()
{
	return -test_serialize_json();
}
