STRUCT	BankJsonTransaction
{
	INT	1	version				DEFAULT		1
	INCLUDE IDL_ResponseHeader.dsc
	STRUCT QueryTransactionDetails
	{
		STRUCT AddonMessages
		{
			STRUCT AddonMessage ARRAY 3
			{
				STRING	64	message_text
			}
		}
		STRUCT TransactionDetailTitle
		{
			STRING	64	title_text
			INT	2	page_no
			INT	2	page_size
		}
		STRUCT TransactionDetails
		{
			STRUCT TransactionDetail ARRAY 10
			{
				STRING	10	trans_date
				STRING	10	trans_time
				STRING	6	outlet_id
				STRING	20	card_no
				FLOAT	4	trans_amount
			}
			STRUCT TransactionTailDetail ARRAY 3
			{
				STRING	64	message_text
			}
		}
	}
	STRUCT OtherTransJnlsno ARRAY 10
	{
		INT	4	_trans_jnlsno
	}
	BOOL	1	sign_flag
}
