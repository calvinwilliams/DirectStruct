/* It had generated by DirectStruct v1.9.1 */
#include "IDL_BankTransaction.dsc.h"

#ifndef FUNCNAME_DSCLOG_BankTransaction
#define FUNCNAME_DSCLOG_BankTransaction	DSCLOG_BankTransaction
#endif

#ifndef PREFIX_DSCLOG_BankTransaction
#define PREFIX_DSCLOG_BankTransaction	printf( 
#endif

#ifndef NEWLINE_DSCLOG_BankTransaction
#define NEWLINE_DSCLOG_BankTransaction	"\n"
#endif

int FUNCNAME_DSCLOG_BankTransaction( BankTransaction *pst )
{
	int	index[10] = { 0 } ; index[0]++; index[0] = 0 ;
	PREFIX_DSCLOG_BankTransaction "BankTransaction.version[%d]" NEWLINE_DSCLOG_BankTransaction , pst->version );
		PREFIX_DSCLOG_BankTransaction "BankTransaction.ResponseHeader.transaction_code[%s]" NEWLINE_DSCLOG_BankTransaction , pst->ResponseHeader.transaction_code );
		PREFIX_DSCLOG_BankTransaction "BankTransaction.ResponseHeader.trans_jnlsno[%d]" NEWLINE_DSCLOG_BankTransaction , pst->ResponseHeader.trans_jnlsno );
		PREFIX_DSCLOG_BankTransaction "BankTransaction.ResponseHeader.response_code[%d]" NEWLINE_DSCLOG_BankTransaction , pst->ResponseHeader.response_code );
		PREFIX_DSCLOG_BankTransaction "BankTransaction.ResponseHeader.response_desc[%s]" NEWLINE_DSCLOG_BankTransaction , pst->ResponseHeader.response_desc );
				PREFIX_DSCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.AddonMessages._AddonMessage_count[%d]" NEWLINE_DSCLOG_BankTransaction , pst->QueryTransactionDetails.AddonMessages._AddonMessage_count );
				for( index[3] = 0 ; index[3] < pst->QueryTransactionDetails.AddonMessages._AddonMessage_count ; index[3]++ )
				{
				PREFIX_DSCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.AddonMessages.AddonMessage[index[3]].message_text[%s]" NEWLINE_DSCLOG_BankTransaction , pst->QueryTransactionDetails.AddonMessages.AddonMessage[index[3]].message_text );
				}
			PREFIX_DSCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetailTitle.title_text[%s]" NEWLINE_DSCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetailTitle.title_text );
			PREFIX_DSCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetailTitle.page_no[%hd]" NEWLINE_DSCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetailTitle.page_no );
			PREFIX_DSCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetailTitle.page_size[%hd]" NEWLINE_DSCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetailTitle.page_size );
				PREFIX_DSCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetails._TransactionDetail_count[%d]" NEWLINE_DSCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetails._TransactionDetail_count );
				for( index[3] = 0 ; index[3] < pst->QueryTransactionDetails.TransactionDetails._TransactionDetail_count ; index[3]++ )
				{
				PREFIX_DSCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetails.TransactionDetail[index[3]].trans_date[%s]" NEWLINE_DSCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetails.TransactionDetail[index[3]].trans_date );
				PREFIX_DSCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetails.TransactionDetail[index[3]].trans_time[%s]" NEWLINE_DSCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetails.TransactionDetail[index[3]].trans_time );
				PREFIX_DSCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetails.TransactionDetail[index[3]].outlet_id[%s]" NEWLINE_DSCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetails.TransactionDetail[index[3]].outlet_id );
				PREFIX_DSCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetails.TransactionDetail[index[3]].card_no[%s]" NEWLINE_DSCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetails.TransactionDetail[index[3]].card_no );
				PREFIX_DSCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetails.TransactionDetail[index[3]].trans_amount[%f]" NEWLINE_DSCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetails.TransactionDetail[index[3]].trans_amount );
				}
	return 0;
}
