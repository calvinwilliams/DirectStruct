/* It had generated by DirectStruct v1.6.3 */
#include "IDL_AllTypes2.dsc.h"

int DSCINIT_AllTypes2( AllTypes2 *pst )
{
	int	index[10] = { 0 } ; index[0] = 0 ;
	memset( pst , 0x00 , sizeof(AllTypes2) );
	return 0;
}

int DSCNETORDER_AllTypes2( AllTypes2 *pst )
{
	int	index[10] = { 0 } ; index[0] = 0 ;
	pst->n4=(int)htonl((unsigned int)(pst->n4));
	__HTONLL(pst->n8);
	pst->u4=htonl(pst->u4);
	__HTONLL(pst->u8);
	__HTONF(pst->f4);
	__HTONLF(pst->f8);
	return 0;
}

int DSCHOSTORDER_AllTypes2( AllTypes2 *pst )
{
	int	index[10] = { 0 } ; index[0] = 0 ;
	pst->n4=(int)ntohl((unsigned int)(pst->n4));
	__NTOHLL(pst->n8);
	pst->u4=ntohl(pst->u4);
	__NTOHLL(pst->u8);
	__NTOHF(pst->f4);
	__NTOHLF(pst->f8);
	return 0;
}

int DSCSERIALIZE_COMPACT_AllTypes2( AllTypes2 *pst , char *buf , int *p_len )
{
	char	*ptr = buf ;
	int	len = 0 ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	
	/* n4 */
	{ int i; i=(int)htonl((unsigned int)(pst->n4)); memcpy( ptr , (char*)&i , 4 ); }
	len+=4; ptr+=4;
	/* n8 */
	{ longlong ll=pst->n8; __HTONLL(ll); memcpy( ptr , (char*)&ll , 8 ); }
	len+=8; ptr+=8;
	/* u4 */
	{ int i; i=htonl(pst->u4); memcpy( ptr , (char*)&i , 4 ); }
	len+=4; ptr+=4;
	/* u8 */
	{ longlong ll=pst->u8; __HTONLL(ll); memcpy( ptr , (char*)&ll , 8 ); }
	len+=8; ptr+=8;
	/* f4 */
	{ float f=pst->f4; __HTONF(f); memcpy( ptr , (char*)&f , 4 ); }
	len+=4; ptr+=4;
	/* f8 */
	{ double d=pst->f8; __HTONLF(d); memcpy( ptr , (char*)&d , 8 ); }
	len+=8; ptr+=8;
	/* str32 */
	memcpy( ptr , pst->str32 , 32 );
	len+=32; ptr+=32;
	/* str1024 */
	memcpy( ptr , pst->str1024 , 1024 );
	len+=1024; ptr+=1024;
	
	if( p_len ) (*p_len) = len ;
	
	return 0;
}

int DSCDESERIALIZE_COMPACT_AllTypes2( char *buf , int *p_len , AllTypes2 *pst )
{
	char	*ptr = buf ;
	int	len = 0 ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	
	/* n4 */
	memcpy( (char*)&(pst->n4) , ptr , 4 ); pst->n4=(int)htonl((unsigned int)(pst->n4));
	len+=4; ptr+=4;
	/* n8 */
	memcpy( (char*)&(pst->n8) , ptr , 8 ); __HTONLL(pst->n8);
	len+=8; ptr+=8;
	/* u4 */
	memcpy( (char*)&(pst->u4) , ptr , 4 ); pst->u4=htonl(pst->u4);
	len+=4; ptr+=4;
	/* u8 */
	memcpy( (char*)&(pst->u8) , ptr , 8 ); __HTONLL(pst->u8);
	len+=8; ptr+=8;
	/* f4 */
	memcpy( (char*)&(pst->f4) , ptr , 4 ); __NTOHF(pst->f4);
	len+=4; ptr+=4;
	/* f8 */
	memcpy( (char*)&(pst->f8) , ptr , 8 ); __NTOHLF(pst->f8);
	len+=8; ptr+=8;
	/* str32 */
	memcpy( pst->str32 , ptr , 32 );
	len+=32; ptr+=32;
	/* str1024 */
	memcpy( pst->str1024 , ptr , 1024 );
	len+=1024; ptr+=1024;
	
	if( p_len ) (*p_len) = len ;
	
	return 0;
}

int DSCSERIALIZE_COMPRESS_AllTypes2( AllTypes2 *pst , char *buf , int *p_len )
{
	char	*ptr = buf ;
	int	len = 0 ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	/* n4 */
	COMPRESS_INT4( pst->n4 , ptr , len )
	/* n8 */
	COMPRESS_INT8( pst->n8 , ptr , len )
	/* u4 */
	COMPRESS_UINT4( pst->u4 , ptr , len )
	/* u8 */
	COMPRESS_UINT8( pst->u8 , ptr , len )
	/* f4 */
	{ float f=pst->f4; __HTONF(f); memcpy( ptr , (char*)&f , 4 ); }
	len+=4; ptr+=4;
	/* f8 */
	{ double d=pst->f8; __HTONLF(d); memcpy( ptr , (char*)&d , 8 ); }
	len+=8; ptr+=8;
	/* str32 */
	{
	unsigned int size=strlen(pst->str32);
	COMPRESS_UINT4( size , ptr , len )
	memcpy( ptr , pst->str32 , size );
	len+=size; ptr+=size;
	}
	/* str1024 */
	{
	unsigned int size=strlen(pst->str1024);
	COMPRESS_UINT4( size , ptr , len )
	memcpy( ptr , pst->str1024 , size );
	len+=size; ptr+=size;
	}
	
	if( p_len ) (*p_len) = len ;
	
	return 0;
}

int DSCDESERIALIZE_COMPRESS_AllTypes2( char *buf , int *p_len , AllTypes2 *pst )
{
	char	*ptr = buf ;
	int	len = 0 ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	/* n4 */
	UNCOMPRESS_INT4( ptr , len , pst->n4 )
	/* n8 */
	UNCOMPRESS_INT8( ptr , len , pst->n8 )
	/* u4 */
	UNCOMPRESS_UINT4( ptr , len , pst->u4 )
	/* u8 */
	UNCOMPRESS_UINT8( ptr , len , pst->u8 )
	/* f4 */
	memcpy( (char*)&(pst->f4) , ptr , 4 ); __NTOHF(pst->f4);
	len+=4; ptr+=4;
	/* f8 */
	memcpy( (char*)&(pst->f8) , ptr , 8 ); __NTOHLF(pst->f8);
	len+=8; ptr+=8;
	/* str32 */
	{
	unsigned int size ;
	UNCOMPRESS_UINT4( ptr , len , size )
	memcpy( pst->str32 , ptr , size );
	len+=size; ptr+=size;
	}
	/* str1024 */
	{
	unsigned int size ;
	UNCOMPRESS_UINT4( ptr , len , size )
	memcpy( pst->str1024 , ptr , size );
	len+=size; ptr+=size;
	}
	
	if( p_len ) (*p_len) = len ;
	
	return 0;
}
