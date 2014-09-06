#include "dsc.h"

/*
 * dsc - DirectSrtuct compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

int GenerateCCode_h( struct CommandParameter *pcmdparam , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_h )
{
	struct StructInfo	*pmsginfo = NULL ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_struct )
	{
		if( depth == 0 )
		{
			fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "\n" );
			fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "typedef struct\n" );
		}
		else
		{
			fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "struct\n" );
		}
		fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "{\n" );
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			fprintabs( fp_dsc_h , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_h , "	char	%s ;\n" , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_h , "	short	%s ;\n" , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_h , "	int	%s ;\n" , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_h , "	longlong %s ;\n" , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "UINT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_h , "	unsigned char	%s ;\n" , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_h , "	unsigned short	%s ;\n" , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_h , "	unsigned int	%s ;\n" , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_h , "	unsigned longlong %s ;\n" , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "FLOAT" ) )
			{
				if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_h , "	float	%s ;\n" , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_h , "	double	%s ;\n" , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "CHAR" ) )
			{
				fprintf( fp_dsc_h , "	char	%s ;\n" , pfldinfo->field_name );
			}
			else if( STRCMP( pfldinfo->field_type , == , "UCHAR" ) )
			{
				fprintf( fp_dsc_h , "	unsigned char %s ;\n" , pfldinfo->field_name );
			}
			else if( STRCMP( pfldinfo->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_h , "	char	%s[ %d + 1 ] ;\n" , pfldinfo->field_name , pfldinfo->field_len );
			}
		}
		
		if( pmsginfo->sub_struct_list )
		{
			nret = GenerateCCode_h( pcmdparam , depth + 1 , pmsginfo->sub_struct_list , fp_dsc_h ) ;
			if( nret )
				return nret;
		}
		
		if( depth == 0 )
		{
			fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "} %s ;\n" , pmsginfo->struct_name );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "} %s ;\n" , pmsginfo->struct_name );
			}
			else
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "} %s [ %d ] ;\n" , pmsginfo->struct_name , pmsginfo->array_size );
			}
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCINIT( struct CommandParameter *pcmdparam , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pmsginfo = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_struct )
	{
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pmsginfo->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname , up_pathname );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pmsginfo->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pmsginfo->struct_name , depth );
			}
		}
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			if( STRCMP( pfldinfo->init_default , == , "" ) )
				continue;
			
			fprintabs( fp_dsc_c , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "FLOAT" ) )
			{
				if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "CHAR" ) )
			{
				fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
			}
			else if( STRCMP( pfldinfo->field_type , == , "UCHAR" ) )
			{
				fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
			}
			else if( STRCMP( pfldinfo->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_c , "	strcpy( %s%s , %s );\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
			}
		}
		
		if( pmsginfo->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCINIT( pcmdparam , depth + 1 , pmsginfo->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCNETORDER( struct CommandParameter *pcmdparam , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pmsginfo = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_struct )
	{
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pmsginfo->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pmsginfo->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pmsginfo->struct_name , depth );
			}
		}
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			fprintabs( fp_dsc_c , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	%s%s=(short)htons((unsigned short)(%s%s));\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	%s%s=(int)htonl((unsigned int)(%s%s));\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	%s%s=(longlong)HTONLL((unsigned longlong)(%s%s));\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "UINT" ) )
			{
				if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	%s%s=htons(%s%s);\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	%s%s=htonl(%s%s);\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	%s%s=HTONLL(%s%s);\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
			}
		}
		
		if( pmsginfo->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCNETORDER( pcmdparam , depth + 1 , pmsginfo->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCHOSTORDER( struct CommandParameter *pcmdparam , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pmsginfo = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_struct )
	{
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pmsginfo->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pmsginfo->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pmsginfo->struct_name , depth );
			}
		}
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			fprintabs( fp_dsc_c , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	%s%s=(short)ntohs((unsigned short)(%s%s));\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	%s%s=(int)ntohl((unsigned int)(%s%s));\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	%s%s=(longlong)NTOHLL((unsigned longlong)(%s%s));\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "UINT" ) )
			{
				if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	%s%s=ntohs(%s%s);\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	%s%s=ntohl(%s%s);\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	%s%s=NTOHLL(%s%s);\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
			}
		}
		
		if( pmsginfo->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCHOSTORDER( pcmdparam , depth + 1 , pmsginfo->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_COMPACT( struct CommandParameter *pcmdparam , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pmsginfo = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_struct )
	{
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pmsginfo->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pmsginfo->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pmsginfo->struct_name , depth );
			}
		}
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			fprintabs( fp_dsc_c , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	{ short s; s=(short)htons((unsigned short)(%s%s)); memcpy( ptr , (char*)&s , 2 ); }\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	{ int i; i=(int)htonl((unsigned int)(%s%s)); memcpy( ptr , (char*)&i , 4 ); }\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	{ longlong ll; ll=(longlong)HTONLL((unsigned longlong)(%s%s)); memcpy( ptr , (char*)&ll , 8 ); }\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "UINT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	(*ptr) = (char)%s%s ;\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	{ short s; s=htons(%s%s); memcpy( ptr , (char*)&s , 2 ); }\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	{ int i; i=htonl(%s%s); memcpy( ptr , (char*)&i , 4 ); }\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	{ longlong ll; ll=HTONLL(%s%s); memcpy( ptr , (char*)&ll , 8 ); }\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "FLOAT" ) )
			{
				if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( ptr , (char*)&(%s%s) , 4 );\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( ptr , (char*)&(%s%s) , 8 );\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "CHAR" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "UCHAR" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	(*ptr) = (char)%s%s ;\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_c , "	\n" );
				fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
				fprintf( fp_dsc_c , "	memcpy( ptr , %s%s , %d );\n" , pathname,pfldinfo->field_name , pfldinfo->field_len );
				fprintf( fp_dsc_c , "	len+=%d; ptr+=%d;\n" , pfldinfo->field_len , pfldinfo->field_len );
			}
		}
		
		if( pmsginfo->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_COMPACT( pcmdparam , depth + 1 , pmsginfo->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_COMPACT( struct CommandParameter *pcmdparam , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pmsginfo = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_struct )
	{
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pmsginfo->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pmsginfo->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pmsginfo->struct_name , depth );
			}
		}
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			fprintabs( fp_dsc_c , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	%s%s = (*ptr) ;\n" , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 2 ); %s%s=(short)htons((unsigned short)(%s%s));\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 4 ); %s%s=(int)htonl((unsigned int)(%s%s));\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); %s%s=(longlong)HTONLL((unsigned longlong)(%s%s));\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "UINT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	%s%s = (unsigned char)(*ptr) ;\n" , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 2 ); %s%s=htons(%s%s);\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 4 ); %s%s=htonl(%s%s);\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); %s%s=HTONLL(%s%s);\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "FLOAT" ) )
			{
				if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 4 );\n" , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 );\n" , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "CHAR" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	%s%s = (char)(*ptr) ;\n" , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "UCHAR" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	%s%s = (unsigned char)(*ptr) ;\n" , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_c , "	\n" );
				fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
				fprintf( fp_dsc_c , "	memcpy( %s%s , ptr , %d );\n" , pathname , pfldinfo->field_name , pfldinfo->field_len );
				fprintf( fp_dsc_c , "	len+=%d; ptr+=%d;\n" , pfldinfo->field_len , pfldinfo->field_len );
			}
		}
		
		if( pmsginfo->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_COMPACT( pcmdparam , depth + 1 , pmsginfo->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_COMPRESS( struct CommandParameter *pcmdparam , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pmsginfo = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_struct )
	{
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pmsginfo->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pmsginfo->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pmsginfo->struct_name , depth );
			}
		}
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			fprintabs( fp_dsc_c , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	{ short s; s=(short)htons((unsigned short)(%s%s)); memcpy( ptr , (char*)&s , 2 ); }\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	COMPRESS_INT4( %s%s , ptr , len )\n" , pathname,pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	COMPRESS_INT8( %s%s , ptr , len )\n" , pathname,pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "UINT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	(*ptr) = (char)%s%s ;\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	{ short s; s=htons(%s%s); memcpy( ptr , (char*)&s , 2 ); }\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	COMPRESS_UINT4( %s%s , ptr , len )\n" , pathname,pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	COMPRESS_UINT8( %s%s , ptr , len )\n" , pathname,pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "FLOAT" ) )
			{
				if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( ptr , (char*)&(%s%s) , 4 );\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( ptr , (char*)&(%s%s) , 8 );\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "CHAR" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "UCHAR" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	(*ptr) = (char)%s%s ;\n" , pathname,pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_c , "	\n" );
				fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
				fprintf( fp_dsc_c , "	{\n" );
				fprintf( fp_dsc_c , "	unsigned int size=strlen(%s%s);\n" , pathname,pfldinfo->field_name );
				fprintf( fp_dsc_c , "	COMPRESS_UINT4( size , ptr , len )\n" );
				fprintf( fp_dsc_c , "	memcpy( ptr , %s%s , size );\n" , pathname,pfldinfo->field_name );
				fprintf( fp_dsc_c , "	len+=size; ptr+=size;\n" );
				fprintf( fp_dsc_c , "	}\n" );
			}
		}
		
		if( pmsginfo->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_COMPRESS( pcmdparam , depth + 1 , pmsginfo->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_COMPRESS( struct CommandParameter *pcmdparam , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pmsginfo = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_struct )
	{
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pmsginfo->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pmsginfo->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pmsginfo->struct_name , depth );
			}
		}
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			fprintabs( fp_dsc_c , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	%s%s = (*ptr) ;\n" , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 2 ); %s%s=(short)htons((unsigned short)(%s%s));\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	UNCOMPRESS_INT4( ptr , len , %s%s )\n" , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	UNCOMPRESS_INT8( ptr , len , %s%s )\n" , pathname , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "UINT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	%s%s = (unsigned char)(*ptr) ;\n" , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 2 ); %s%s=htons(%s%s);\n" , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	UNCOMPRESS_UINT4( ptr , len , %s%s )\n" , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	UNCOMPRESS_UINT8( ptr , len , %s%s )\n" , pathname , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "FLOAT" ) )
			{
				if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 4 );\n" , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 );\n" , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "CHAR" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	%s%s = (char)(*ptr) ;\n" , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "UCHAR" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_c , "	\n" );
					fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	%s%s = (unsigned char)(*ptr) ;\n" , pathname , pfldinfo->field_name );
					fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_c , "	\n" );
				fprintf( fp_dsc_c , "	/* %s */\n" , pfldinfo->field_name );
				fprintf( fp_dsc_c , "	{\n" );
				fprintf( fp_dsc_c , "	unsigned int size ;\n" );
				fprintf( fp_dsc_c , "	UNCOMPRESS_UINT4( ptr , len , size )\n" );
				fprintf( fp_dsc_c , "	memcpy( %s%s , ptr , size );\n" , pathname , pfldinfo->field_name );
				fprintf( fp_dsc_c , "	len+=size; ptr+=size;\n" );
				fprintf( fp_dsc_c , "	}\n" );
			}
		}
		
		if( pmsginfo->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_COMPRESS( pcmdparam , depth + 1 , pmsginfo->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_LOG_c( struct CommandParameter *pcmdparam , int depth , struct StructInfo *grandfather_pmsginfo , struct StructInfo *first_pmsginfo , FILE *fp_dsc_LOG_c , char *LOG_up_pathname , char *up_pathname )
{
	struct StructInfo	*pmsginfo = NULL ;
	char			LOG_pathname[ 1024 + 1 ] ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_struct )
	{
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pmsginfo->array_size , depth );
			fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( LOG_pathname , LOG_up_pathname );
			strcpy( pathname , up_pathname );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				SNPRINTF( LOG_pathname , sizeof(LOG_pathname)-1 , "%s%s." , LOG_up_pathname , pmsginfo->struct_name );
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pmsginfo->struct_name );
			}
			else
			{
				SNPRINTF( LOG_pathname , sizeof(LOG_pathname)-1 , "%s%s[index[%d]]." , LOG_up_pathname , pmsginfo->struct_name , depth );
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pmsginfo->struct_name , depth );
			}
		}
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			fprintabs( fp_dsc_LOG_c , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%d]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->struct_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%hd]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->struct_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%d]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->struct_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[\" LONGLONG_FORMAT_SPEC \"d]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->struct_name , pathname , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "UINT" ) )
			{
				if( pfldinfo->field_len == 1 )
				{
					fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%u]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->struct_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 2 )
				{
					fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%hu]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->struct_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%u]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->struct_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[\" LONGLONG_FORMAT_SPEC \"u]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->struct_name , pathname , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "FLOAT" ) )
			{
				if( pfldinfo->field_len == 4 )
				{
					fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%f]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->struct_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_len == 8 )
				{
					fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%lf]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->struct_name , pathname , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "CHAR" ) )
			{
				fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%c]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->struct_name , pathname , pfldinfo->field_name );
			}
			else if( STRCMP( pfldinfo->field_type , == , "UCHAR" ) )
			{
				fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%c]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->struct_name , pathname , pfldinfo->field_name );
			}
			else if( STRCMP( pfldinfo->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%s]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->struct_name , pathname , pfldinfo->field_name );
			}
		}
		
		if( pmsginfo->sub_struct_list )
		{
			nret = GenerateCCode_LOG_c( pcmdparam , depth + 1 , grandfather_pmsginfo , pmsginfo->sub_struct_list , fp_dsc_LOG_c , LOG_pathname , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pmsginfo->array_size > 0 )
		{
			fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	}\n" );
		}
	}
	
	return 0;
}

int GenerateCCode( struct CommandParameter *pcmdparam , struct StructInfo *pmsginfo , FILE *fp_dsc_h , FILE *fp_dsc_c , FILE *fp_dsc_LOG_c )
{
	char		LOG_pathname[ 64 + 1 ] ;
	int		nret = 0 ;
	
	/* Generate *.dsc.h */
	fprintf( fp_dsc_h , "#ifndef _H_%s_\n" , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_h , "#define _H_%s_\n" , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#include <stdio.h>\n" );
	fprintf( fp_dsc_h , "#include <stdlib.h>\n" );
	fprintf( fp_dsc_h , "#include <string.h>\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#if ( defined __unix ) || ( defined __linux__ )\n" );
	fprintf( fp_dsc_h , "#ifndef _WINDLL_FUNC\n" );
	fprintf( fp_dsc_h , "#define _WINDLL_FUNC\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#elif ( defined _WIN32 )\n" );
	fprintf( fp_dsc_h , "#ifndef _WINDLL_FUNC\n" );
	fprintf( fp_dsc_h , "#define _WINDLL_FUNC		_declspec(dllexport)\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#if ( defined __unix ) | ( defined __linux__ )\n" );
	fprintf( fp_dsc_h , "#include <arpa/inet.h>\n" );
	fprintf( fp_dsc_h , "#elif ( defined _WIN32 )\n" );
	fprintf( fp_dsc_h , "#include \"winsock.h\"\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#if ( defined __unix ) | ( defined __linux__ )\n" );
	fprintf( fp_dsc_h , "#ifndef longlong\n" );
	fprintf( fp_dsc_h , "#define longlong	long long\n" );
	fprintf( fp_dsc_h , "#define ulonglong	unsigned long long\n" );
	fprintf( fp_dsc_h , "#define LL(_ll_)	_ll_##LL\n" );
	fprintf( fp_dsc_h , "#define ULL(_ll_)	_ll_##ULL\n" );
	fprintf( fp_dsc_h , "#define LONGLONG_FORMAT_SPEC	\"%%ll\"\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#elif ( defined _WIN32 )\n" );
	fprintf( fp_dsc_h , "#ifndef longlong\n" );
	fprintf( fp_dsc_h , "#define longlong	__int64\n" );
	fprintf( fp_dsc_h , "#define ulonglong	unsigned __int64\n" );
	fprintf( fp_dsc_h , "#define LL(_ll_)	_ll_\n" );
	fprintf( fp_dsc_h , "#define ULL(_ll_)	_ll_\n" );
	fprintf( fp_dsc_h , "#define LONGLONG_FORMAT_SPEC	\"%%I64\"\n" );
	fprintf( fp_dsc_h , "#define __BIG_ENDIAN	4321\n" );
	fprintf( fp_dsc_h , "#define __LITTLE_ENDIAN	1234\n" );
	fprintf( fp_dsc_h , "#define __BYTE_ORDER	__LITTLE_ENDIAN\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#ifndef HTONLL\n" );
	fprintf( fp_dsc_h , "#define HTONLL(_ulonglong_)	(__BYTE_ORDER==__BIG_ENDIAN?(_ulonglong_):( (((ulonglong)htonl((unsigned int)(((_ulonglong_)<<32)>>32)))<<32) | ((ulonglong)htonl((unsigned int)((_ulonglong_)>>32))) ) )\n" );
	fprintf( fp_dsc_h , "#define NTOHLL(_ulonglong_)	(__BYTE_ORDER==__BIG_ENDIAN?(_ulonglong_):( (((ulonglong)ntohl((unsigned int)(((_ulonglong_)<<32)>>32)))<<32) | ((ulonglong)ntohl((unsigned int)((_ulonglong_)>>32))) ) )\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#ifndef COMPRESS_INT4\n" );
	fprintf( fp_dsc_h , "#define COMPRESS_INT4(_int4_,_buf_,_bufoffset_)					\\\n" );
	fprintf( fp_dsc_h , "	{									\\\n" );
	fprintf( fp_dsc_h , "		unsigned char	_flag_ = 0 ;					\\\n" );
	fprintf( fp_dsc_h , "		unsigned int	_h_ ;						\\\n" );
	fprintf( fp_dsc_h , "		unsigned int	_n_ ;						\\\n" );
	fprintf( fp_dsc_h , "		if( _int4_ < 0 )						\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_h_ = -_int4_ ;						\\\n" );
	fprintf( fp_dsc_h , "			_n_ = htonl( _h_ ) ;					\\\n" );
	fprintf( fp_dsc_h , "			_flag_ |= 0x10 ;					\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else								\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_h_ = _int4_ ;						\\\n" );
	fprintf( fp_dsc_h , "			_n_ = htonl( _h_ ) ;					\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		if( _h_ & 0xFF000000 )						\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = _flag_ | 4 ;					\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_) , 4 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=5; _buf_+=5 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & 0x00FF0000 )					\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = _flag_ | 3 ;					\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+1 , 3 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=4; _buf_+=4 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & 0x0000FF00 )					\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = _flag_ | 2 ;					\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+2 , 2 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=3; _buf_+=3 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else								\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = _flag_ | 1 ;					\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+3 , 1 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=2; _buf_+=2 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "	}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#ifndef UNCOMPRESS_INT4\n" );
	fprintf( fp_dsc_h , "#define UNCOMPRESS_INT4(_buf_,_bufoffset_,_int4_)				\\\n" );
	fprintf( fp_dsc_h , "	{									\\\n" );
	fprintf( fp_dsc_h , "		unsigned char	_flag_ = 0 ;					\\\n" );
	fprintf( fp_dsc_h , "		_int4_ = 0 ;							\\\n" );
	fprintf( fp_dsc_h , "		_flag_ = _buf_[0] & 0xF0 ;					\\\n" );
	fprintf( fp_dsc_h , "		_buf_[0] &= 0x0F ;						\\\n" );
	fprintf( fp_dsc_h , "		memcpy( ((unsigned char*)&_int4_) + (4-_buf_[0]) , _buf_+1 , _buf_[0] );\\\n" );
	fprintf( fp_dsc_h , "		_int4_ = ntohl( _int4_ ) ;					\\\n" );
	fprintf( fp_dsc_h , "		if( _flag_ & 0x10 )						\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_int4_ = -_int4_ ;					\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		_bufoffset_+=1+_buf_[0]; _buf_+=1+_buf_[0];			\\\n" );
	fprintf( fp_dsc_h , "	}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#ifndef COMPRESS_UINT4\n" );
	fprintf( fp_dsc_h , "#define COMPRESS_UINT4(_uint4_,_buf_,_bufoffset_)				\\\n" );
	fprintf( fp_dsc_h , "	{									\\\n" );
	fprintf( fp_dsc_h , "		unsigned int	_h_ ;						\\\n" );
	fprintf( fp_dsc_h , "		unsigned int	_n_ ;						\\\n" );
	fprintf( fp_dsc_h , "		_h_ = _uint4_ ;							\\\n" );
	fprintf( fp_dsc_h , "		_n_ = htonl( _h_ ) ;						\\\n" );
	fprintf( fp_dsc_h , "		if( _h_ & 0xFF000000 )						\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = 4 ;						\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_) , 4 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=5; _buf_+=5 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & 0x00FF0000 )					\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = 3 ;						\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+1 , 3 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=4; _buf_+=4 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & 0x0000FF00 )					\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = 2 ;						\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+2 , 2 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=3; _buf_+=3 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else								\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = 1 ;						\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+3 , 1 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=2; _buf_+=2 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "	}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#ifndef UNCOMPRESS_UINT4\n" );
	fprintf( fp_dsc_h , "#define UNCOMPRESS_UINT4(_buf_,_bufoffset_,_uint4_)				\\\n" );
	fprintf( fp_dsc_h , "	{									\\\n" );
	fprintf( fp_dsc_h , "		_uint4_ = 0 ;							\\\n" );
	fprintf( fp_dsc_h , "		_buf_[0] &= 0x0F ;						\\\n" );
	fprintf( fp_dsc_h , "		memcpy( ((unsigned char*)&_uint4_) + (4-_buf_[0]) , _buf_+1 , _buf_[0] );\\\n" );
	fprintf( fp_dsc_h , "		_uint4_ = ntohl( _uint4_ ) ;					\\\n" );
	fprintf( fp_dsc_h , "		_bufoffset_+=1+_buf_[0]; _buf_+=1+_buf_[0];			\\\n" );
	fprintf( fp_dsc_h , "	}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#ifndef COMPRESS_INT8\n" );
	fprintf( fp_dsc_h , "#define COMPRESS_INT8(_int8_,_buf_,_bufoffset_)					\\\n" );
	fprintf( fp_dsc_h , "	{									\\\n" );
	fprintf( fp_dsc_h , "		unsigned char	_flag_ = 0 ;					\\\n" );
	fprintf( fp_dsc_h , "		unsigned longlong	_h_ ;					\\\n" );
	fprintf( fp_dsc_h , "		unsigned longlong	_n_ ;					\\\n" );
	fprintf( fp_dsc_h , "		if( _int8_ < 0 )						\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_h_ = -_int8_ ;						\\\n" );
	fprintf( fp_dsc_h , "			_n_ = HTONLL( _h_ ) ;					\\\n" );
	fprintf( fp_dsc_h , "			_flag_ |= 0x10 ;					\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else								\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_h_ = _int8_ ;						\\\n" );
	fprintf( fp_dsc_h , "			_n_ = HTONLL( _h_ ) ;					\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		if( _h_ & ULL(0xFF00000000000000) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = _flag_ | 8 ;					\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_) , 8 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=9; _buf_+=9 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & ULL(0x00FF000000000000) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = _flag_ | 7 ;					\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+1 , 7 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=8; _buf_+=8 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & ULL(0x0000FF0000000000) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = _flag_ | 6 ;					\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+2 , 6 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=7; _buf_+=7 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & ULL(0x000000FF00000000) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = _flag_ | 5 ;					\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+3 , 5 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=6; _buf_+=6 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & ULL(0x00000000FF000000) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = _flag_ | 4 ;					\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+4 , 4 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=5; _buf_+=5 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & ULL(0x0000000000FF0000) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = _flag_ | 3 ;					\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+5 , 3 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=4; _buf_+=4 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & ULL(0x000000000000FF00) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = _flag_ | 2 ;					\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+6 , 2 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=3; _buf_+=3 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else								\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = _flag_ | 1 ;					\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+7 , 1 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=2; _buf_+=2 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "	}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#ifndef UNCOMPRESS_INT8\n" );
	fprintf( fp_dsc_h , "#define UNCOMPRESS_INT8(_buf_,_bufoffset_,_int8_)				\\\n" );
	fprintf( fp_dsc_h , "	{									\\\n" );
	fprintf( fp_dsc_h , "		unsigned char	_flag_ = 0 ;					\\\n" );
	fprintf( fp_dsc_h , "		_int8_ = 0 ;							\\\n" );
	fprintf( fp_dsc_h , "		_flag_ = _buf_[0] & 0xF0 ;					\\\n" );
	fprintf( fp_dsc_h , "		_buf_[0] &= 0x0F ;						\\\n" );
	fprintf( fp_dsc_h , "		memcpy( ((unsigned char*)&_int8_) + (8-_buf_[0]) , _buf_+1 , _buf_[0] );\\\n" );
	fprintf( fp_dsc_h , "		_int8_ = NTOHLL( _int8_ ) ;					\\\n" );
	fprintf( fp_dsc_h , "		if( _flag_ & 0x10 )						\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_int8_ = -_int8_ ;					\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		_bufoffset_+=1+_buf_[0]; _buf_+=1+_buf_[0];			\\\n" );
	fprintf( fp_dsc_h , "	}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#ifndef COMPRESS_UINT8\n" );
	fprintf( fp_dsc_h , "#define COMPRESS_UINT8(_uint8_,_buf_,_bufoffset_)				\\\n" );
	fprintf( fp_dsc_h , "	{									\\\n" );
	fprintf( fp_dsc_h , "		unsigned longlong	_h_ ;					\\\n" );
	fprintf( fp_dsc_h , "		unsigned longlong	_n_ ;					\\\n" );
	fprintf( fp_dsc_h , "		_h_ = _uint8_ ;							\\\n" );
	fprintf( fp_dsc_h , "		_n_ = HTONLL( _h_ ) ;						\\\n" );
	fprintf( fp_dsc_h , "		if( _h_ & ULL(0xFF00000000000000) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = 8 ;						\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_) , 8 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=9; _buf_+=9 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & ULL(0x00FF000000000000) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = 7 ;						\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+1 , 7 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=8; _buf_+=8 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & ULL(0x0000FF0000000000) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = 6 ;						\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+2 , 6 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=7; _buf_+=7 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & ULL(0x000000FF00000000) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = 5 ;						\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+3 , 5 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=6; _buf_+=6 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & ULL(0x00000000FF000000) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = 4 ;						\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+4 , 4 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=5; _buf_+=5 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & ULL(0x0000000000FF0000) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = 3 ;						\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+5 , 3 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=4; _buf_+=4 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else if( _h_ & ULL(0x000000000000FF00) )				\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = 2 ;						\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+6 , 2 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=3; _buf_+=3 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "		else								\\\n" );
	fprintf( fp_dsc_h , "		{								\\\n" );
	fprintf( fp_dsc_h , "			_buf_[0] = 1 ;						\\\n" );
	fprintf( fp_dsc_h , "			memcpy( _buf_+1 , ((unsigned char *)&_n_)+7 , 1 );	\\\n" );
	fprintf( fp_dsc_h , "			_bufoffset_+=2; _buf_+=2 ;				\\\n" );
	fprintf( fp_dsc_h , "		}								\\\n" );
	fprintf( fp_dsc_h , "	}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#ifndef UNCOMPRESS_UINT8\n" );
	fprintf( fp_dsc_h , "#define UNCOMPRESS_UINT8(_buf_,_bufoffset_,_uint8_)				\\\n" );
	fprintf( fp_dsc_h , "	{									\\\n" );
	fprintf( fp_dsc_h , "		_uint8_ = 0 ;							\\\n" );
	fprintf( fp_dsc_h , "		_buf_[0] &= 0x0F ;						\\\n" );
	fprintf( fp_dsc_h , "		memcpy( ((unsigned char*)&_uint8_) + (8-_buf_[0]) , _buf_+1 , _buf_[0] );\\\n" );
	fprintf( fp_dsc_h , "		_uint8_ = NTOHLL( _uint8_ ) ;					\\\n" );
	fprintf( fp_dsc_h , "		_bufoffset_+=1+_buf_[0]; _buf_+=1+_buf_[0];			\\\n" );
	fprintf( fp_dsc_h , "	}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#define DSCSERIALIZE_COMPACT_BUFSIZE_%s	%d\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_length );
	fprintf( fp_dsc_h , "#define DSCSERIALIZE_COMPRESS_BUFSIZE_%s	%d\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_length + pmsginfo->next_struct->field_count );
	nret = GenerateCCode_h( pcmdparam , 0 , pmsginfo->next_struct , fp_dsc_h ) ;
	if( nret )
		return nret;
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCINIT_%s( %s *pst );\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCNETORDER_%s( %s *pst );\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCHOSTORDER_%s( %s *pst );\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_COMPACT_%s( %s *pst , char *buf , int *p_len );\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_COMPACT_%s( char *buf , int *p_len , %s *pst );\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_COMPRESS_%s( %s *pst , char *buf , int *p_len );\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_COMPRESS_%s( char *buf , int *p_len , %s *pst );\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	
	/* Generate *.dsc.c */
	fprintf( fp_dsc_c , "#include \"%s.h\"\n" , pcmdparam->pathfilename );
	
	/* Generate DSCINIT_*() */
	fprintf( fp_dsc_c , "\n" );
	fprintf( fp_dsc_c , "int DSCINIT_%s( %s *pst )\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_c , "{\n" );
	fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
	fprintf( fp_dsc_c , "	memset( pst , 0x00 , sizeof(%s) );\n" , pmsginfo->next_struct->struct_name );
	nret = GenerateCCode_c_DSCINIT( pcmdparam , 0 , pmsginfo->next_struct , fp_dsc_c , "pst->" ) ;
	if( nret )
		return nret;
	fprintf( fp_dsc_c , "	return 0;\n" );
	fprintf( fp_dsc_c , "}\n" );
	
	/* Generate DSCNETORDER_*() */
	fprintf( fp_dsc_c , "\n" );
	fprintf( fp_dsc_c , "int DSCNETORDER_%s( %s *pst )\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_c , "{\n" );
	fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
	nret = GenerateCCode_c_DSCNETORDER( pcmdparam , 0 , pmsginfo->next_struct , fp_dsc_c , "pst->" ) ;
	if( nret )
		return nret;
	fprintf( fp_dsc_c , "	return 0;\n" );
	fprintf( fp_dsc_c , "}\n" );
	
	/* Generate DSCHOSTORDER_*() */
	fprintf( fp_dsc_c , "\n" );
	fprintf( fp_dsc_c , "int DSCHOSTORDER_%s( %s *pst )\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_c , "{\n" );
	fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
	nret = GenerateCCode_c_DSCHOSTORDER( pcmdparam , 0 , pmsginfo->next_struct , fp_dsc_c , "pst->" ) ;
	if( nret )
		return nret;
	fprintf( fp_dsc_c , "	return 0;\n" );
	fprintf( fp_dsc_c , "}\n" );
	
	/* Generate DSCSERIALIZE_COMPACT_*() */
	fprintf( fp_dsc_c , "\n" );
	fprintf( fp_dsc_c , "int DSCSERIALIZE_COMPACT_%s( %s *pst , char *buf , int *p_len )\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_c , "{\n" );
	fprintf( fp_dsc_c , "	char	*ptr = buf ;\n" );
	fprintf( fp_dsc_c , "	int	len = 0 ;\n" );
	fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
	fprintf( fp_dsc_c , "	\n" );
	nret = GenerateCCode_c_DSCSERIALIZE_COMPACT( pcmdparam , 0 , pmsginfo->next_struct , fp_dsc_c , "pst->" ) ;
	if( nret )
		return nret;
	fprintf( fp_dsc_c , "	\n" );
	fprintf( fp_dsc_c , "	if( p_len ) (*p_len) = len ;\n" );
	fprintf( fp_dsc_c , "	\n" );
	fprintf( fp_dsc_c , "	return 0;\n" );
	fprintf( fp_dsc_c , "}\n" );
	
	/* Generate DSCDESERIALIZE_COMPACT_*() */
	fprintf( fp_dsc_c , "\n" );
	fprintf( fp_dsc_c , "int DSCDESERIALIZE_COMPACT_%s( char *buf , int *p_len , %s *pst )\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_c , "{\n" );
	fprintf( fp_dsc_c , "	char	*ptr = buf ;\n" );
	fprintf( fp_dsc_c , "	int	len = 0 ;\n" );
	fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
	fprintf( fp_dsc_c , "	\n" );
	nret = GenerateCCode_c_DSCDESERIALIZE_COMPACT( pcmdparam , 0 , pmsginfo->next_struct , fp_dsc_c , "pst->" ) ;
	if( nret )
		return nret;
	fprintf( fp_dsc_c , "	\n" );
	fprintf( fp_dsc_c , "	if( p_len ) (*p_len) = len ;\n" );
	fprintf( fp_dsc_c , "	\n" );
	fprintf( fp_dsc_c , "	return 0;\n" );
	fprintf( fp_dsc_c , "}\n" );
	
	/* Generate DSCSERIALIZE_COMPRESS_*() */
	fprintf( fp_dsc_c , "\n" );
	fprintf( fp_dsc_c , "int DSCSERIALIZE_COMPRESS_%s( %s *pst , char *buf , int *p_len )\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_c , "{\n" );
	fprintf( fp_dsc_c , "	char	*ptr = buf ;\n" );
	fprintf( fp_dsc_c , "	int	len = 0 ;\n" );
	fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
	nret = GenerateCCode_c_DSCSERIALIZE_COMPRESS( pcmdparam , 0 , pmsginfo->next_struct , fp_dsc_c , "pst->" ) ;
	if( nret )
		return nret;
	fprintf( fp_dsc_c , "	\n" );
	fprintf( fp_dsc_c , "	if( p_len ) (*p_len) = len ;\n" );
	fprintf( fp_dsc_c , "	\n" );
	fprintf( fp_dsc_c , "	return 0;\n" );
	fprintf( fp_dsc_c , "}\n" );
	
	/* Generate DSCDESERIALIZE_COMPRESS_*() */
	fprintf( fp_dsc_c , "\n" );
	fprintf( fp_dsc_c , "int DSCDESERIALIZE_COMPRESS_%s( char *buf , int *p_len , %s *pst )\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_c , "{\n" );
	fprintf( fp_dsc_c , "	char	*ptr = buf ;\n" );
	fprintf( fp_dsc_c , "	int	len = 0 ;\n" );
	fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
	nret = GenerateCCode_c_DSCDESERIALIZE_COMPRESS( pcmdparam , 0 , pmsginfo->next_struct , fp_dsc_c , "pst->" ) ;
	if( nret )
		return nret;
	fprintf( fp_dsc_c , "	\n" );
	fprintf( fp_dsc_c , "	if( p_len ) (*p_len) = len ;\n" );
	fprintf( fp_dsc_c , "	\n" );
	fprintf( fp_dsc_c , "	return 0;\n" );
	fprintf( fp_dsc_c , "}\n" );
	
	/* Generate *.dsc.LOG.c */
	fprintf( fp_dsc_LOG_c , "#include \"%s.h\"\n" , pcmdparam->pathfilename );
	fprintf( fp_dsc_LOG_c , "\n" );
	fprintf( fp_dsc_LOG_c , "#ifndef FUNCNAME_DSCLOG_%s\n" , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_LOG_c , "#define FUNCNAME_DSCLOG_%s	DSCLOG_%s\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_LOG_c , "#endif\n" );
	fprintf( fp_dsc_LOG_c , "\n" );
	fprintf( fp_dsc_LOG_c , "#ifndef PREFIX_DSCLOG_%s\n" , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_LOG_c , "#define PREFIX_DSCLOG_%s	printf( \n" , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_LOG_c , "#endif\n" );
	fprintf( fp_dsc_LOG_c , "\n" );
	fprintf( fp_dsc_LOG_c , "#ifndef NEWLINE_DSCLOG_%s\n" , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_LOG_c , "#define NEWLINE_DSCLOG_%s	\"\\n\"\n" , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_LOG_c , "#endif\n" );
	fprintf( fp_dsc_LOG_c , "\n" );
	fprintf( fp_dsc_LOG_c , "int FUNCNAME_DSCLOG_%s( %s *pst )\n" , pmsginfo->next_struct->struct_name , pmsginfo->next_struct->struct_name );
	fprintf( fp_dsc_LOG_c , "{\n" );
	fprintf( fp_dsc_LOG_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
	SNPRINTF( LOG_pathname , sizeof(LOG_pathname)-1 , "%s." , pmsginfo->next_struct->struct_name );
	nret = GenerateCCode_LOG_c( pcmdparam , 0 , pmsginfo->next_struct , pmsginfo->next_struct , fp_dsc_LOG_c , LOG_pathname , "pst->" ) ;
	if( nret )
		return nret;
	fprintf( fp_dsc_LOG_c , "	return 0;\n" );
	fprintf( fp_dsc_LOG_c , "}\n" );
	
	return 0;
}
