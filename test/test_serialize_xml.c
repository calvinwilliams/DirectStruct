#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "IDL_BankXmlTransaction.dsc.h"
#include "IDL_BankXmlTransaction.dsc.LOG.c"

int test_serialize_xml()
{
	BankXmlTransaction	trans ;
	char		buf[ 40960 + 1 ] ;
	int		len ;
	int		nret = 0 ;
	
	/* client code */
	DSCINIT_BankXmlTransaction( & trans );
	
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
	
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[2].card_no , "<" );
	
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[3].card_no , "&" );
	
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[4].card_no , ">" );
	
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[5].card_no , "<&>" );
	
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[6].card_no , "<你&他>" );
	
	strcpy( trans.QueryTransactionDetails.TransactionDetails.TransactionDetail[7].card_no , "我<你&他>她" );
	
	DSCLOG_BankXmlTransaction( & trans );
	
	memset( buf , 0x00 , sizeof(buf) );
	len = sizeof(buf)-1 ;
	nret = DSCSERIALIZE_XML_BankXmlTransaction( & trans , "GBK" , buf , & len ) ;
	if( nret )
	{
		printf( "DSCSERIALIZE_XML_BankXmlTransaction failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCSERIALIZE_XML_BankXmlTransaction ok , len[%d]\n" , len );
	}
	
	printf( "[%s]\n" , buf );
	
	/* ... client send communication struct to server ... */
	
	/* server code */
	memset( & trans , 0x00 , sizeof(BankXmlTransaction) );
	nret = DSCDESERIALIZE_XML_BankXmlTransaction( NULL , buf , & len , & trans ) ;
	if( nret )
	{
		printf( "DSCDESERIALIZE_XML_BankXmlTransaction failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCDESERIALIZE_XML_BankXmlTransaction ok\n" );
	}
	
	DSCLOG_BankXmlTransaction( & trans );
	
	return 0;
}

int main()
{
	return -test_serialize_xml();
}
