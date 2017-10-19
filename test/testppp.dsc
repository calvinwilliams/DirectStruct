STRUCT OrgCustBaseInfoQueryRequest MESSAGE hzbank 
{
	INCLUDE RequestHeader.dsc
	STRUCT RequestBody
	{
		INCLUDE CustIdentZone.dsc
	}
}
