# 此文件由makeobj.sh自动生成
############################################################
# 项目名 : 
# 模块名 : 
# 备  注 : 
############################################################

###### 源文件配置区
#@ c_FILE
c_FILE		=	\
			IDL_AllTypes.dsc.c		\
			IDL_AllTypes2.dsc.c		\
			IDL_CommHead.dsc.c		\
			IDL_ResponseHeader.dsc.c	\
			IDL_BankTransaction.dsc.c	\
			IDL_BankXmlTransaction.dsc.c	\
			IDL_BankJsonTransaction.dsc.c	\
			test_netorder.c			\
			test_serialize_compact.c	\
			test_serialize_compress.c	\
			test_serialize_compress_press.c	\
			test_trans.c			\
			test_serialize_xml.c		\
			test_serialize_json.c		\
			test_serialize_json_utf8.c	\
			press_deserialize_json.c	\

###### 目标文件、安装目录配置区
BIN		=	test_netorder			\
			test_serialize_compact		\
			test_serialize_compress		\
			test_serialize_compress_press	\
			test_trans			\

###### 编译选项
CFLAGS		=	-I. \
			-I$(HOME)/include/fasterxml	\
			-I$(HOME)/include/fasterjson	\

###### 链接选项
LFLAGS		=	-L. \

###### 额外宏定义区
CLEAN_ADDITION	=	\
			IDL_AllTypes.dsc.* \
			IDL_AllTypes2.dsc.* \
			IDL_CommHead.dsc.* \
			IDL_BankTransaction.dsc.* \
			IDL_BankXmlTransaction.dsc.* \
			IDL_BankJsonTransaction.dsc.* \
			IDL_testable.dsc.* \
			test_serialize_json		\
			test_serialize_json_utf8	\
			press_deserialize_json		\
			test_serialize_xml		\
			test_testable.ecE test_testable.c test_testable.o \
			test_testable			\

PREPRO		=	prepro

###### 加载mktpl模板库
#@ make_all
#@ make_clean
#@ make_install
#@ make_install_BININST
#@ make_install_HDERINST
#@ make_install_DFTHDERINST

include $(MKTPL2_HOME)/makeobj_$(MKTPL2_OS).inc

prepro		:
	dsc -f IDL_AllTypes.dsc -c-compact -c-compress -c-LOG
	dsc -f IDL_AllTypes2.dsc -c-compact -c-compress -c-LOG
	dsc -f IDL_CommHead.dsc -c-compact -c-LOG
	dsc -f IDL_BankTransaction.dsc -c-compress -c-LOG
	dsc -f IDL_BankXmlTransaction.dsc -c-xml -c-xml-compact -c-LOG
	dsc -f IDL_BankJsonTransaction.dsc -c-json -c-json-compact -c-LOG
	dsc -f IDL_testable.dsc -sql-pgsql -ec-pgsql -c-LOG

###### 目标文件依赖关系
test_netorder	:	test_netorder.o IDL_AllTypes.dsc.o
	$(CC) -o $@ test_netorder.o IDL_AllTypes.dsc.o $(LFLAGS)

test_serialize_compact	:	test_serialize_compact.o IDL_AllTypes.dsc.o
	$(CC) -o $@ test_serialize_compact.o IDL_AllTypes.dsc.o $(LFLAGS)

test_serialize_compress	:	test_serialize_compress.o IDL_AllTypes.dsc.o
	$(CC) -o $@ test_serialize_compress.o IDL_AllTypes.dsc.o $(LFLAGS)

test_serialize_compress_press	:	test_serialize_compress_press.o IDL_AllTypes2.dsc.o
	$(CC) -o $@ test_serialize_compress_press.o IDL_AllTypes2.dsc.o $(LFLAGS)

test_trans	:	test_trans.o IDL_CommHead.dsc.o IDL_BankTransaction.dsc.o
	$(CC) -o $@ test_trans.o IDL_CommHead.dsc.o IDL_BankTransaction.dsc.o $(LFLAGS)

IDL_BankXmlTransaction.dsc.o	:	IDL_BankXmlTransaction.dsc.c
	$(CC) -I$(HOME)/include/fasterxml -I. -c IDL_BankXmlTransaction.dsc.c

test_serialize_xml	:	test_serialize_xml.o IDL_BankXmlTransaction.dsc.o
	$(CC) -o $@ test_serialize_xml.o IDL_BankXmlTransaction.dsc.o $(LFLAGS) -L$(HOME)/lib -lfasterxml

test_serialize_json	:	test_serialize_json.o IDL_BankJsonTransaction.dsc.o
	$(CC) -o $@ test_serialize_json.o IDL_BankJsonTransaction.dsc.o $(LFLAGS) -L$(HOME)/lib -lfasterjson

test_serialize_json_utf8	:	test_serialize_json_utf8.o IDL_BankJsonTransaction.dsc.o
	$(CC) -o $@ test_serialize_json_utf8.o IDL_BankJsonTransaction.dsc.o $(LFLAGS) -L$(HOME)/lib -lfasterjson

press_deserialize_json	:	press_deserialize_json.o IDL_BankJsonTransaction.dsc.o
	$(CC) -o $@ press_deserialize_json.o IDL_BankJsonTransaction.dsc.o $(LFLAGS) -L$(HOME)/lib -lfasterjson

ECP	=	ecpg

test_testable.o	:	test_testable.ec
	$(CC) -I/root/local/postgresql/include -E -x c test_testable.ec > test_testable.ecE
	$(ECP) -h test_testable.ecE -o test_testable.c
	sed -i 's/NULL/0/g' test_testable.c
	$(CC) -I/root/local/postgresql/include -c test_testable.c

IDL_testable.dsc.ESQL.o	:	IDL_testable.dsc.ESQL.ec
	$(CC) -I/root/local/postgresql/include -E -x c IDL_testable.dsc.ESQL.ec > IDL_testable.dsc.ESQL.ecE
	$(ECP) -h IDL_testable.dsc.ESQL.ecE -o IDL_testable.dsc.ESQL.c
	sed -i 's/NULL/0/g' IDL_testable.dsc.ESQL.c
	$(CC) -I/root/local/postgresql/include -c IDL_testable.dsc.ESQL.c

test_testable	:	IDL_testable.dsc.ESQL.o test_testable.o
	$(CC) -o $@ IDL_testable.dsc.ESQL.o test_testable.o $(LFLAGS) -L/root/local/postgresql/lib -lecpg

