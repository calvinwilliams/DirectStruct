STRUCT	BankJsonTransaction
{
	INT	1	version				DEFAULT		1
	INCLUDE IDL_ResponseHeader.dsc
	STRING	4	null_string
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
					H "#define MAX_PAGE_SIZE	10"
		}
		STRUCT TransactionDetails
		{
			STRUCT TransactionDetail ARRAY 10
			{
				STRING	10	trans_date
				STRING	10	trans_time
				STRING	6	outlet_id
						H '#define CENTER_OUTLET_ID	"1001"'
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
