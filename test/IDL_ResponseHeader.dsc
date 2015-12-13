	STRUCT	ResponseHeader
	{
		STRING	32	transaction_code
		INT	4	trans_jnlsno
		INT	4	response_code		DEFAULT		0
				H "#define RESPONSE_CODE_PASSWORD_NOTMATCH	-12345"
		STRING	1024	response_desc		DEFAULT		"OK"
	}
