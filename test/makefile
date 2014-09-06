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
			IDL_CommHead.dsc.c		\
			IDL_ResponseHeader.dsc.c	\
			IDL_BankTransaction.dsc.c	\
			test_netorder.c			\
			test_serialize_compact.c	\
			test_serialize_compress.c	\
			test_serialize_compress_press.c	\
			test_trans.c			\

###### 目标文件、安装目录配置区
BIN		=	test_netorder			\
			test_serialize_compact		\
			test_serialize_compress		\
			test_serialize_compress_press	\
			test_trans

###### 编译选项
CFLAGS		=	-I. \

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

test_serialize_compress_press	:	test_serialize_compress_press.o IDL_AllTypes.dsc.o
	$(CC) -o $@ test_serialize_compress_press.o IDL_AllTypes.dsc.o $(LFLAGS)

test_trans	:	test_trans.o IDL_CommHead.dsc.o IDL_BankTransaction.dsc.o
	$(CC) -o $@ test_trans.o IDL_CommHead.dsc.o IDL_BankTransaction.dsc.o $(LFLAGS)

