STRUCT  RequestHeader	MESSAGE	HZBANK2	# 请求公共头
{
	STRUCT	SysReqHeader	 MESSAGE HZBANK3	#系统请求头
	{
		STRING	20	TransactionRef		#交易参考号
		STRING	20	ProjectCode		#项目名
		STRING	10	BranchNo		#交易机构
		STRING	4	SourceChannel		#来源渠道
		STRING	4	Language		#交易语言
		STRING	10	VersionNo		#交易版本
		STRING	16	TerminalNo		#交易终端
		STRING	2	SystemType		#系统类型
		STRING	12	OperatorId		#操作员编号
		STRING	128	SessionToken		#认证标识
		STRING	10	SenderNode		#节点编号
		STRING	10	SenderServer		#服务器编号
		STRING	20	SenderJournalNo		#发送方流水号(日期8字节+序号8字节)
		STRING	10	SenderDate		#交易日期
		STRING	10	SenderTime		#交易时间
		STRING	10	TransferNode		#节点编号
		STRING	10	TransferServer		#服务器编号
		STRING	20	TransferJournalNo	#转发方流水号(日期8字节+序号8字节)
		STRING	10	TransferDate		#交易日期
		STRING 	10	TransferTime		#交易时间
	}
	STRUCT	TxnReqHeader		#业务请求头
	{
		STRING	4	MessageType		#消息类型
		STRING	10	TransactionCode		#交易编码	
		INT	4	SecondStamp		#交易秒戳
		STRING	12	AuthorOperatorId	#授权操作员编号	
		STRING	512	AuthorFingerprint	#授权操作员指纹
		STRING	48	AuthorPassword		#授权操作员密码
		INT	4	RelatedCycleNo		#对方周期号
		STRING	10	RelatedAccDate		#对方交易日期
		STRING	12	CheckOperatorId		#复核操作员编号
		STRING	256	Reserve			#保留域
	}
}

