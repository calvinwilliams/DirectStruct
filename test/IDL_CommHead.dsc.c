#include "IDL_CommHead.dsc.h"

int DSCINIT_CommHead( CommHead *pst )
{
	int	index[10] = { 0 } ; index[0] = 0 ;
	memset( pst , 0x00 , sizeof(CommHead) );
	return 0;
}

int DSCNETORDER_CommHead( CommHead *pst )
{
	int	index[10] = { 0 } ; index[0] = 0 ;
	pst->length=(int)htonl((unsigned int)(pst->length));
	return 0;
}

int DSCHOSTORDER_CommHead( CommHead *pst )
{
	int	index[10] = { 0 } ; index[0] = 0 ;
	pst->length=(int)ntohl((unsigned int)(pst->length));
	return 0;
}

int DSCSERIALIZE_COMPACT_CommHead( CommHead *pst , char *buf , int *p_len )
{
	char	*ptr = buf ;
	int	len = 0 ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	
	
	/* length */
	{ int i; i=(int)htonl((unsigned int)(pst->length)); memcpy( ptr , (char*)&i , 4 ); }
	len+=4; ptr+=4;
	
	if( p_len ) (*p_len) = len ;
	
	return 0;
}

int DSCDESERIALIZE_COMPACT_CommHead( char *buf , int *p_len , CommHead *pst )
{
	char	*ptr = buf ;
	int	len = 0 ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	
	
	/* length */
	memcpy( (char*)&(pst->length) , ptr , 4 ); pst->length=(int)htonl((unsigned int)(pst->length));
	len+=4; ptr+=4;
	
	if( p_len ) (*p_len) = len ;
	
	return 0;
}

int DSCSERIALIZE_COMPRESS_CommHead( CommHead *pst , char *buf , int *p_len )
{
	char	*ptr = buf ;
	int	len = 0 ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	
	/* length */
	COMPRESS_INT4( pst->length , ptr , len )
	
	if( p_len ) (*p_len) = len ;
	
	return 0;
}

int DSCDESERIALIZE_COMPRESS_CommHead( char *buf , int *p_len , CommHead *pst )
{
	char	*ptr = buf ;
	int	len = 0 ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	
	/* length */
	UNCOMPRESS_INT4( ptr , len , pst->length )
	
	if( p_len ) (*p_len) = len ;
	
	return 0;
}
