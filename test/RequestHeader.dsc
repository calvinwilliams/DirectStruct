STRUCT  RequestHeader	MESSAGE	HZBANK2	# ���󹫹�ͷ
{
	STRUCT	SysReqHeader	 MESSAGE HZBANK3	#ϵͳ����ͷ
	{
		STRING	20	TransactionRef		#���ײο���
		STRING	20	ProjectCode		#��Ŀ��
		STRING	10	BranchNo		#���׻���
		STRING	4	SourceChannel		#��Դ����
		STRING	4	Language		#��������
		STRING	10	VersionNo		#���װ汾
		STRING	16	TerminalNo		#�����ն�
		STRING	2	SystemType		#ϵͳ����
		STRING	12	OperatorId		#����Ա���
		STRING	128	SessionToken		#��֤��ʶ
		STRING	10	SenderNode		#�ڵ���
		STRING	10	SenderServer		#���������
		STRING	20	SenderJournalNo		#���ͷ���ˮ��(����8�ֽ�+���8�ֽ�)
		STRING	10	SenderDate		#��������
		STRING	10	SenderTime		#����ʱ��
		STRING	10	TransferNode		#�ڵ���
		STRING	10	TransferServer		#���������
		STRING	20	TransferJournalNo	#ת������ˮ��(����8�ֽ�+���8�ֽ�)
		STRING	10	TransferDate		#��������
		STRING 	10	TransferTime		#����ʱ��
	}
	STRUCT	TxnReqHeader		#ҵ������ͷ
	{
		STRING	4	MessageType		#��Ϣ����
		STRING	10	TransactionCode		#���ױ���	
		INT	4	SecondStamp		#�������
		STRING	12	AuthorOperatorId	#��Ȩ����Ա���	
		STRING	512	AuthorFingerprint	#��Ȩ����Աָ��
		STRING	48	AuthorPassword		#��Ȩ����Ա����
		INT	4	RelatedCycleNo		#�Է����ں�
		STRING	10	RelatedAccDate		#�Է���������
		STRING	12	CheckOperatorId		#���˲���Ա���
		STRING	256	Reserve			#������
	}
}

