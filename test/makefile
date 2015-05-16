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
CLEAN_ADDITION	=

###### 加载mktpl模板库
#@ make_all
#@ make_clean
#@ make_install
#@ make_install_BININST
#@ make_install_HDERINST
#@ make_install_DFTHDERINST

include $(MKTPLDIR)/makeobj_$(MKTPLOS).inc

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
	$(CC) -I$(HOME)/exinc/fasterxml -I. -c IDL_BankXmlTransaction.dsc.c

test_serialize_xml	:	test_serialize_xml.o IDL_BankXmlTransaction.dsc.o
	$(CC) -o $@ test_serialize_xml.o IDL_BankXmlTransaction.dsc.o $(LFLAGS) -L$(HOME)/exlib -lfasterxml

test_serialize_json	:	test_serialize_json.o IDL_BankJsonTransaction.dsc.o
	$(CC) -o $@ test_serialize_json.o IDL_BankJsonTransaction.dsc.o $(LFLAGS) -L$(HOME)/exlib -lfasterjson

test_serialize_json_utf8	:	test_serialize_json_utf8.o IDL_BankJsonTransaction.dsc.o
	$(CC) -o $@ test_serialize_json_utf8.o IDL_BankJsonTransaction.dsc.o $(LFLAGS) -L$(HOME)/exlib -lfasterjson

press_deserialize_json	:	press_deserialize_json.o IDL_BankJsonTransaction.dsc.o
	$(CC) -o $@ press_deserialize_json.o IDL_BankJsonTransaction.dsc.o $(LFLAGS) -L$(HOME)/exlib -lfasterjson

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

CLEAN_ADDITION	=	test_testable.ecE test_testable.c test_testable.o \
			IDL_testable.dsc.ESQL.ecE IDL_testable.dsc.ESQL.c IDL_testable.dsc.ESQL.o \

