# 此文件由makeobj.sh自动生成
############################################################
# 项目名 : 
# 模块名 : 
# 备  注 : 
############################################################

###### 源文件配置区
#@ c_FILE
c_FILE		=					\
			IDL_userinfo.dsc.c		\
			test_client.c			\
			test_server.c			\
			test_db2json.c			\
			test_sqlaction.c		\

###### 目标文件、安装目录配置区
BIN		=					\
			test_client			\
			test_server			\
			test_db2json			\
			test_sqlaction			\

###### 编译选项
CFLAGS		=	-I. 				\
			-I$(PGHOME)/include		\
			-I$(HOME)/include/fasterxml	\
			-I$(HOME)/include/fasterjson	\

###### 链接选项
LFLAGS		=	-L. 				\
			-L$(PGHOME)/lib			\
			-lecpg				\
			-L$(HOME)/lib			\
			-lfasterxml			\
			-lfasterjson			\

#			-L/root/local/postgresql/lib	\

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
test_client	:	IDL_userinfo.dsc.o test_client.o
	$(CC) -o $@ IDL_userinfo.dsc.o test_client.o $(LFLAGS)

ECP	=	ecpg -h -D_PQSQL

IDL_userinfo.dsc.ESQL.o	:	IDL_userinfo.dsc.ESQL.ec
	$(CC) $(CFLAGS) -E -x c IDL_userinfo.dsc.ESQL.ec > IDL_userinfo.dsc.ESQL.ecE
	$(ECP) IDL_userinfo.dsc.ESQL.ecE -o IDL_userinfo.dsc.ESQL.c
	sed -i 's/NULL/0/g' IDL_userinfo.dsc.ESQL.c
	$(CC) $(CFLAGS) -c IDL_userinfo.dsc.ESQL.c

test_server.o	:	test_server.ec
	$(CC) $(CFLAGS) -E -x c test_server.ec > test_server.ecE
	$(ECP) test_server.ecE -o test_server.c
	sed -i 's/NULL/0/g' test_server.c
	$(CC) $(CFLAGS) -c test_server.c

test_server	:	IDL_userinfo.dsc.o IDL_userinfo.dsc.ESQL.o test_server.o
	$(CC) -o $@ IDL_userinfo.dsc.o IDL_userinfo.dsc.ESQL.o test_server.o $(LFLAGS)

test_db2json.o	:	test_db2json.ec
	$(CC) $(CFLAGS) -E -x c test_db2json.ec > test_db2json.ecE
	$(ECP) test_db2json.ecE -o test_db2json.c
	sed -i 's/NULL/0/g' test_db2json.c
	$(CC) $(CFLAGS) -c test_db2json.c

test_db2json	:	IDL_userinfo.dsc.o IDL_userinfo.dsc.ESQL.o test_db2json.o
	$(CC) -o $@ IDL_userinfo.dsc.o IDL_userinfo.dsc.ESQL.o test_db2json.o $(LFLAGS)

test_sqlaction.o	:	test_sqlaction.ec
	$(CC) $(CFLAGS) -E -x c test_sqlaction.ec > test_sqlaction.ecE
	$(ECP) test_sqlaction.ecE -o test_sqlaction.c
	sed -i 's/NULL/0/g' test_sqlaction.c
	$(CC) $(CFLAGS) -c test_sqlaction.c

test_sqlaction	:	IDL_userinfo.dsc.o IDL_userinfo.dsc.ESQL.o test_sqlaction.o
	$(CC) -o $@ IDL_userinfo.dsc.o IDL_userinfo.dsc.ESQL.o test_sqlaction.o $(LFLAGS)

CLEAN_ADDITION	=	IDL_userinfo.dsc.ESQL.ecE IDL_userinfo.dsc.ESQL.c IDL_userinfo.dsc.ESQL.o	\
			test_server.ecE test_server.c test_server.o		\
			test_db2json.ecE test_db2json.c test_db2json.o		\
			test_sqlaction.ecE test_sqlaction.c test_sqlaction.o	\

