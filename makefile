# ���ļ���makedir.sh�Զ�����
############################################################
# ��Ŀ�� :
# ģ���� :
# �� ע :
############################################################

###### ��Ŀ¼������
DIROBJS		= \
			src \
			test \

###### ����mktplģ���
#@ FILESYSTEM
#@ dir_all
#@ dir_make
#@ dir_clean
#@ dir_install
#@ dir_uninstall
include $(MKTPLDIR)/makedir_$(MKTPLOS).inc

