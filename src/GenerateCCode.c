#include "dsc.h"

/*
 * dsc - DirectSrtuct compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

int GenerateCCode_h( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_h )
{
	struct StructInfo	*psi = NULL ;
	struct FieldInfo	*pfi = NULL ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
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
		
		for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
		{
			if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	char	%s ;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	short	%s ;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	int	%s ;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	longlong %s ;\n" , pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UINT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	unsigned char	%s ;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	unsigned short	%s ;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	unsigned int	%s ;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	unsigned longlong %s ;\n" , pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
			{
				if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	float	%s ;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	double	%s ;\n" , pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "CHAR" ) )
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	char	%s ;\n" , pfi->field_name );
			}
			else if( STRCMP( pfi->field_type , == , "UCHAR" ) )
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	unsigned char %s ;\n" , pfi->field_name );
			}
			else if( STRCMP( pfi->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	char	%s[ %d + 1 ] ;\n" , pfi->field_name , pfi->field_len );
			}
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_h( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_h ) ;
			if( nret )
				return nret;
		}
		
		if( depth == 0 )
		{
			fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "} %s ;\n" , psi->struct_name );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "} %s ;\n" , psi->struct_name );
			}
			else
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "} %s [ %d ] ;\n" , psi->struct_name , psi->array_size );
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "int	_%s_count ;\n" , psi->struct_name );
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "int	_%s_size ;\n" , psi->struct_name );
			}
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCINIT( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfi = NULL ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , psi->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname , up_pathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
		}
		
		for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
		{
			if( STRCMP( pfi->init_default , == , "" ) )
				continue;
			
			if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfi->field_name , pfi->init_default );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfi->field_name , pfi->init_default );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfi->field_name , pfi->init_default );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfi->field_name , pfi->init_default );
				}
			}
			else if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfi->field_name , pfi->init_default );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfi->field_name , pfi->init_default );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfi->field_name , pfi->init_default );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfi->field_name , pfi->init_default );
				}
			}
			else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
			{
				if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfi->field_name , pfi->init_default );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfi->field_name , pfi->init_default );
				}
			}
			else if( STRCMP( pfi->field_type , == , "CHAR" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfi->field_name , pfi->init_default );
			}
			else if( STRCMP( pfi->field_type , == , "UCHAR" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfi->field_name , pfi->init_default );
			}
			else if( STRCMP( pfi->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	strcpy( %s%s , \"%s\" );\n" , pathname,pfi->field_name , pfi->init_default );
			}
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCINIT( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s_%s_size = %d ;\n" , up_pathname,psi->struct_name , psi->array_size );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCNETORDER( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfi = NULL ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , psi->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
		}
		
		for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
		{
			if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=(short)htons((unsigned short)(%s%s));\n" , pathname,pfi->field_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=(int)htonl((unsigned int)(%s%s));\n" , pathname,pfi->field_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__HTONLL(%s%s);\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UINT" ) )
			{
				if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=htons(%s%s);\n" , pathname,pfi->field_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=htonl(%s%s);\n" , pathname,pfi->field_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__HTONLL(%s%s);\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
			{
				if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__HTONF(%s%s);\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__HTONLF(%s%s);\n" , pathname,pfi->field_name );
				}
			}
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCNETORDER( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCHOSTORDER( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfi = NULL ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , psi->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
		}
		
		for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
		{
			if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=(short)ntohs((unsigned short)(%s%s));\n" , pathname,pfi->field_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=(int)ntohl((unsigned int)(%s%s));\n" , pathname,pfi->field_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__NTOHLL(%s%s);\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UINT" ) )
			{
				if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=ntohs(%s%s);\n" , pathname,pfi->field_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=ntohl(%s%s);\n" , pathname,pfi->field_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__NTOHLL(%s%s);\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
			{
				if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__NTOHF(%s%s);\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__NTOHLF(%s%s);\n" , pathname,pfi->field_name );
				}
			}
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCHOSTORDER( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_COMPACT( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfi = NULL ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , psi->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
		}
		
		for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
		{
			if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ short s; s=(short)htons((unsigned short)(%s%s)); memcpy( ptr , (char*)&s , 2 ); }\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ int i; i=(int)htonl((unsigned int)(%s%s)); memcpy( ptr , (char*)&i , 4 ); }\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ longlong ll=%s%s; __HTONLL(ll); memcpy( ptr , (char*)&ll , 8 ); }\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UINT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = (char)%s%s ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ short s; s=htons(%s%s); memcpy( ptr , (char*)&s , 2 ); }\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ int i; i=htonl(%s%s); memcpy( ptr , (char*)&i , 4 ); }\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ longlong ll=%s%s; __HTONLL(ll); memcpy( ptr , (char*)&ll , 8 ); }\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
			{
				if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ float f=%s%s; __HTONF(f); memcpy( ptr , (char*)&f , 4 ); }\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ double d=%s%s; __HTONLF(d); memcpy( ptr , (char*)&d , 8 ); }\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "CHAR" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UCHAR" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = (char)%s%s ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( ptr , %s%s , %d );\n" , pathname,pfi->field_name , pfi->field_len );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=%d; ptr+=%d;\n" , pfi->field_len , pfi->field_len );
			}
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_COMPACT( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_COMPACT( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfi = NULL ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , psi->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
		}
		
		for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
		{
			if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (*ptr) ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 2 ); %s%s=(short)htons((unsigned short)(%s%s));\n" , pathname,pfi->field_name , pathname,pfi->field_name , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 4 ); %s%s=(int)htonl((unsigned int)(%s%s));\n" , pathname,pfi->field_name , pathname,pfi->field_name , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); __HTONLL(%s%s);\n" , pathname,pfi->field_name , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UINT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (unsigned char)(*ptr) ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 2 ); %s%s=htons(%s%s);\n" , pathname,pfi->field_name , pathname,pfi->field_name , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 4 ); %s%s=htonl(%s%s);\n" , pathname,pfi->field_name , pathname,pfi->field_name , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); __HTONLL(%s%s);\n" , pathname,pfi->field_name , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
			{
				if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 4 ); __NTOHF(%s%s);\n" , pathname,pfi->field_name , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); __NTOHLF(%s%s);\n" , pathname,pfi->field_name , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "CHAR" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (char)(*ptr) ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UCHAR" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (unsigned char)(*ptr) ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( %s%s , ptr , %d );\n" , pathname,pfi->field_name , pfi->field_len );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=%d; ptr+=%d;\n" , pfi->field_len , pfi->field_len );
			}
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_COMPACT( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_COMPRESS( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfi = NULL ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , psi->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
		}
		
		for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
		{
			if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ short s; s=(short)htons((unsigned short)(%s%s)); memcpy( ptr , (char*)&s , 2 ); }\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	COMPRESS_INT4( %s%s , ptr , len )\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	COMPRESS_INT8( %s%s , ptr , len )\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UINT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = (char)%s%s ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ short s; s=htons(%s%s); memcpy( ptr , (char*)&s , 2 ); }\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	COMPRESS_UINT4( %s%s , ptr , len )\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	COMPRESS_UINT8( %s%s , ptr , len )\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
			{
				if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ float f=%s%s; __HTONF(f); memcpy( ptr , (char*)&f , 4 ); }\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ double d=%s%s; __HTONLF(d); memcpy( ptr , (char*)&d , 8 ); }\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "CHAR" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UCHAR" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = (char)%s%s ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	unsigned int size=strlen(%s%s);\n" , pathname,pfi->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	COMPRESS_UINT4( size , ptr , len )\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( ptr , %s%s , size );\n" , pathname,pfi->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=size; ptr+=size;\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
			}
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_COMPRESS( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_COMPRESS( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfi = NULL ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , psi->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
		}
		
		for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
		{
			if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (*ptr) ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 2 ); %s%s=(short)htons((unsigned short)(%s%s));\n" , pathname,pfi->field_name , pathname,pfi->field_name , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	UNCOMPRESS_INT4( ptr , len , %s%s )\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	UNCOMPRESS_INT8( ptr , len , %s%s )\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UINT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (unsigned char)(*ptr) ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 2 ); %s%s=htons(%s%s);\n" , pathname,pfi->field_name , pathname,pfi->field_name , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	UNCOMPRESS_UINT4( ptr , len , %s%s )\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	UNCOMPRESS_UINT8( ptr , len , %s%s )\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
			{
				if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 4 ); __NTOHF(%s%s);\n" , pathname,pfi->field_name , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); __NTOHLF(%s%s);\n" , pathname,pfi->field_name , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "CHAR" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (char)(*ptr) ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UCHAR" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (unsigned char)(*ptr) ;\n" , pathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfi->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	unsigned int size ;\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	UNCOMPRESS_UINT4( ptr , len , size )\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( %s%s , ptr , size );\n" , pathname,pfi->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=size; ptr+=size;\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
			}
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_COMPRESS( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_XML( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfi = NULL ;
	char			tabs[ DSC_MAXDEPTH ] ;
	int			nret = 0 ;
	
	memset( tabs , '\t' , DSC_MAXDEPTH );
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , psi->array_size , depth );
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
		}
		
		fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\\n\"); if( len <= 0 ) return -1; buf+=len; remain_len-=len;\n" , depth,tabs , psi->struct_name );
		
		for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
		{
			if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%d\",%s%s); XMLESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%hd\",%s%s); XMLESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%d\",%s%s); XMLESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC,%s%s); XMLESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UINT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%u\",%s%s); XMLESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%hu\",%s%s); XMLESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%u\",%s%s); XMLESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC,%s%s); XMLESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
			{
				if( pfi->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%f\",%s%s); XMLESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%lf\",%s%s); XMLESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "CHAR" ) )
			{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%c\",%s%s); XMLESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , pfi->field_name );
			}
			else if( STRCMP( pfi->field_type , == , "UCHAR" ) )
			{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%c\",%s%s); XMLESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , pfi->field_name );
			}
			else if( STRCMP( pfi->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfi->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%s\",%s%s); XMLESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , pfi->field_name );
			}
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_XML( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s</%s>\\n\"); if( len <= 0 ) return -1; buf+=len; remain_len-=len;\n" , depth,tabs , psi->struct_name );
		
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_XML_ENTERNODE( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname , char *up_xmlpathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	char			xmlpathname[ 1024 + 1 ] ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , up_xmlpathname,psi->struct_name );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{if( %s_%s_count > %d ) return -8;}\n" , up_pathname,psi->struct_name , psi->array_size );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
			strcpy( xmlpathname  , up_xmlpathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
			SNPRINTF( xmlpathname , sizeof(xmlpathname)-1 , "%s/%s" , up_xmlpathname , psi->struct_name );
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_XML_ENTERNODE( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname , xmlpathname ) ;
			if( nret )
				return nret;
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_XML_LEAVENODE( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname , char *up_xmlpathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	char			xmlpathname[ 1024 + 1 ] ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , up_xmlpathname,psi->struct_name );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{%s_%s_count++;}\n" , up_pathname,psi->struct_name );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
			strcpy( xmlpathname  , up_xmlpathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
			SNPRINTF( xmlpathname , sizeof(xmlpathname)-1 , "%s/%s" , up_xmlpathname , psi->struct_name );
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_XML_LEAVENODE( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname , xmlpathname ) ;
			if( nret )
				return nret;
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_XML_LEAF( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname , char *up_xmlpathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	char			xmlpathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfi = NULL ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
			strcpy( xmlpathname  , up_xmlpathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[%s_%s_count]." , up_pathname , psi->struct_name , up_pathname,psi->struct_name );
			}
			SNPRINTF( xmlpathname , sizeof(xmlpathname)-1 , "%s/%s" , up_xmlpathname , psi->struct_name );
		}
		
		for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
		{
			if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , xmlpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOC(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , xmlpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOS(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , xmlpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOI(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , xmlpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLL(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UINT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , xmlpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOC(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , xmlpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOS(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , xmlpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOI(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , xmlpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLL(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
			{
				if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , xmlpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOF(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , xmlpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLF(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "CHAR" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , xmlpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOC(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UCHAR" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , xmlpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOC(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , xmlpathname,pfi->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{XMLUNESCAPE_FOLD(content,content_len,%s%s);\n" , pathname,pfi->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{if( content_len > sizeof(%s%s)-1 ) return -7;}}\n" , pathname,pfi->field_name );
			}
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_XML_LEAF( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname , xmlpathname ) ;
			if( nret )
				return nret;
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_JSON( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfi = NULL ;
	char			tabs[ DSC_MAXDEPTH ] ;
	int			nret = 0 ;
	
	memset( tabs , '\t' , DSC_MAXDEPTH );
	
	if( depth == 0 )
	{
		fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s{\\n\"); if( len <= 0 ) return -1; buf+=len; remain_len-=len;\n" , depth,tabs );
	}
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \\n\"); if( len <= 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , psi->struct_name );
		if( psi->array_size > 0 )
		{
			fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s[\\n\"); if( len <= 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
		}
		
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+2 ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , psi->array_size , depth );
			fprintabs( fp_dsc_c , depth+2 ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s{\\n\"); if( len <= 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
		}
		
		for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
		{
			if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+2,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%d\",%s%s); JSONESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+2,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%hd\",%s%s); JSONESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+2,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%d\",%s%s); JSONESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+2,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC\"\",%s%s); JSONESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UINT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+2,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%u\",%s%s); JSONESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+2,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%hu\",%s%s); JSONESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+2,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%u\",%s%s); JSONESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+2,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC\"\",%s%s); JSONESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
			{
				if( pfi->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+2,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%f\",%s%s); JSONESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+2,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%lf\",%s%s); JSONESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "CHAR" ) )
			{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+2,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%c\",%s%s); JSONESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
			}
			else if( STRCMP( pfi->field_type , == , "UCHAR" ) )
			{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+2,tabs , pfi->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%c\",%s%s); JSONESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
			}
			else if( STRCMP( pfi->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+2,tabs , pfi->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%s\",%s%s); JSONESCAPE_EXPAND(buf,len,remain_len); if( len < 0 ) return -1;\n" , pathname,pfi->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" );
			}
			
			if( pfi->next_field || psi->sub_struct_list )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\" ,\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" );
			}
			else
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\n\"); if( len < 0 ) return -1; buf+=len; remain_len-=len;\n" );
			}
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_JSON( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+2 ); fprintf( fp_dsc_c , "\tif(index[%d]<%d-1)\n" , depth , psi->array_size );
			fprintabs( fp_dsc_c , depth+2 ); fprintf( fp_dsc_c , "\t{ len=SNPRINTF(buf,remain_len,\"%.*s} ,\\n\"); if( len <= 0 ) return -1; buf+=len; remain_len-=len; }\n" , depth+1,tabs );
			fprintabs( fp_dsc_c , depth+2 ); fprintf( fp_dsc_c , "\telse\n" );
			fprintabs( fp_dsc_c , depth+2 ); fprintf( fp_dsc_c , "\t{ len=SNPRINTF(buf,remain_len,\"%.*s}\\n\"); if( len <= 0 ) return -1; buf+=len; remain_len-=len; }\n" , depth+1,tabs );
		}
		else
		{
			if( psi->next_struct )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s} ,\\n\"); if( len <= 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
			}
			else
			{
				fprintabs( fp_dsc_c , depth+2 ); fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s}\\n\"); if( len <= 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
			}
		}
		
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+2 ); fprintf( fp_dsc_c , "	}\n" );
			
			if( psi->next_struct )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s] ,\\n\"); if( len <= 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
			}
			else
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s]\\n\"); if( len <= 0 ) return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
			}
		}
	}
	
	if( depth == 0 )
	{
		fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s}\\n\"); if( len <= 0 ) return -1; buf+=len; remain_len-=len;\n" , depth,tabs );
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_JSON_ENTERNODE( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname , char *up_jsonpathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	char			jsonpathname[ 1024 + 1 ] ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , up_jsonpathname,psi->struct_name );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{if( %s_%s_count > %d ) return -8;}\n" , up_pathname,psi->struct_name , psi->array_size );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
			strcpy( jsonpathname  , up_jsonpathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
			SNPRINTF( jsonpathname , sizeof(jsonpathname)-1 , "%s/%s" , up_jsonpathname , psi->struct_name );
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_JSON_ENTERNODE( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname , jsonpathname ) ;
			if( nret )
				return nret;
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_JSON_LEAVENODE( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname , char *up_jsonpathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	char			jsonpathname[ 1024 + 1 ] ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , up_jsonpathname,psi->struct_name );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{%s_%s_count++;}\n" , up_pathname,psi->struct_name );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
			strcpy( jsonpathname  , up_jsonpathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
			SNPRINTF( jsonpathname , sizeof(jsonpathname)-1 , "%s/%s" , up_jsonpathname , psi->struct_name );
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_JSON_LEAVENODE( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname , jsonpathname ) ;
			if( nret )
				return nret;
		}
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_JSON_LEAF( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_c , char *up_pathname , char *up_jsonpathname )
{
	struct StructInfo	*psi = NULL ;
	char			pathname[ 1024 + 1 ] ;
	char			jsonpathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfi = NULL ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
			strcpy( jsonpathname  , up_jsonpathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[%s_%s_count]." , up_pathname , psi->struct_name , up_pathname,psi->struct_name );
			}
			SNPRINTF( jsonpathname , sizeof(jsonpathname)-1 , "%s/%s" , up_jsonpathname , psi->struct_name );
		}
		
		for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
		{
			if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , jsonpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOC(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , jsonpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOS(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , jsonpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOI(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , jsonpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLL(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UINT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , jsonpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOC(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , jsonpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOS(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , jsonpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOI(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , jsonpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLL(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
			{
				if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , jsonpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOF(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , jsonpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLF(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "CHAR" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , jsonpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOC(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UCHAR" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , jsonpathname,pfi->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOC(content,content_len,%s%s);}\n" , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfi->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , jsonpathname,pfi->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{JSONUNESCAPE_FOLD(content,content_len,%s%s);\n" , pathname,pfi->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{if( content_len > sizeof(%s%s)-1 ) return -7;}}\n" , pathname,pfi->field_name );
			}
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_JSON_LEAF( pcp , depth + 1 , psi->sub_struct_list , fp_dsc_c , pathname , jsonpathname ) ;
			if( nret )
				return nret;
		}
	}
	
	return 0;
}

static int GenerateCCode_LOG_c( struct CommandParameter *pcp , int depth , struct StructInfo *grandfather_pmsginfo , struct StructInfo *first_pmsginfo , FILE *fp_dsc_LOG_c , char *LOG_up_pathname , char *up_pathname )
{
	struct StructInfo	*psi = NULL ;
	char			LOG_pathname[ 1024 + 1 ] ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfi = NULL ;
	
	int			nret = 0 ;
	
	for( psi = first_pmsginfo ; psi ; psi = psi->next_struct )
	{
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , psi->array_size , depth );
			fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( LOG_pathname , LOG_up_pathname );
			strcpy( pathname , up_pathname );
		}
		else
		{
			if( psi->array_size == 0 )
			{
				SNPRINTF( LOG_pathname , sizeof(LOG_pathname)-1 , "%s%s." , LOG_up_pathname , psi->struct_name );
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , psi->struct_name );
			}
			else
			{
				SNPRINTF( LOG_pathname , sizeof(LOG_pathname)-1 , "%s%s[index[%d]]." , LOG_up_pathname , psi->struct_name , depth );
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , psi->struct_name , depth );
			}
		}
		
		for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
		{
			if( STRCMP( pfi->field_type , == , "INT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%d]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfi->field_name , grandfather_pmsginfo->struct_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%hd]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfi->field_name , grandfather_pmsginfo->struct_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%d]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfi->field_name , grandfather_pmsginfo->struct_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[\" LONGLONG_FORMAT_SPEC \"d]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfi->field_name , grandfather_pmsginfo->struct_name , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "UINT" ) )
			{
				if( pfi->field_len == 1 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%u]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfi->field_name , grandfather_pmsginfo->struct_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 2 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%hu]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfi->field_name , grandfather_pmsginfo->struct_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%u]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfi->field_name , grandfather_pmsginfo->struct_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[\" LONGLONG_FORMAT_SPEC \"u]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfi->field_name , grandfather_pmsginfo->struct_name , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
			{
				if( pfi->field_len == 4 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%f]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfi->field_name , grandfather_pmsginfo->struct_name , pathname,pfi->field_name );
				}
				else if( pfi->field_len == 8 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%lf]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfi->field_name , grandfather_pmsginfo->struct_name , pathname,pfi->field_name );
				}
			}
			else if( STRCMP( pfi->field_type , == , "CHAR" ) )
			{
				fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%c]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfi->field_name , grandfather_pmsginfo->struct_name , pathname,pfi->field_name );
			}
			else if( STRCMP( pfi->field_type , == , "UCHAR" ) )
			{
				fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%c]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfi->field_name , grandfather_pmsginfo->struct_name , pathname,pfi->field_name );
			}
			else if( STRCMP( pfi->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%s]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfi->field_name , grandfather_pmsginfo->struct_name , pathname,pfi->field_name );
			}
		}
		
		if( psi->sub_struct_list )
		{
			nret = GenerateCCode_LOG_c( pcp , depth + 1 , grandfather_pmsginfo , psi->sub_struct_list , fp_dsc_LOG_c , LOG_pathname , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( psi->array_size > 0 )
		{
			fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	}\n" );
		}
	}
	
	return 0;
}

int GenerateCCode( struct CommandParameter *pcp , struct StructInfo *psi , FILE *fp_dsc_h , FILE *fp_dsc_c , FILE *fp_dsc_LOG_c )
{
	char		LOG_pathname[ 64 + 1 ] ;
	int		nret = 0 ;
	
	/* Generate *.dsc.h */
	fprintf( fp_dsc_h , "/* It had generated by DirectStruct v%s */\n" , DIRECTSTRUCT_VERSION );
	fprintf( fp_dsc_h , "#ifndef _H_%s_\n" , psi->next_struct->struct_name );
	fprintf( fp_dsc_h , "#define _H_%s_\n" , psi->next_struct->struct_name );
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
	fprintf( fp_dsc_h , "#define SNPRINTF		snprintf\n" );
	fprintf( fp_dsc_h , "#elif ( defined _WIN32 )\n" );
	fprintf( fp_dsc_h , "#define SNPRINTF		_snprintf\n" );
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
	fprintf( fp_dsc_h , "#define ATOLL	atoll\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#if ( defined _AIX )\n" );
	fprintf( fp_dsc_h , "#define __BIG_ENDIAN	BIG_ENDIAN\n" );
	fprintf( fp_dsc_h , "#define __LITTLE_ENDIAN	LITTLE_ENDIAN\n" );
	fprintf( fp_dsc_h , "#define __BYTE_ORDER	BYTE_ORDER\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#elif ( defined _WIN32 )\n" );
	fprintf( fp_dsc_h , "#ifndef longlong\n" );
	fprintf( fp_dsc_h , "#define longlong	__int64\n" );
	fprintf( fp_dsc_h , "#define ulonglong	unsigned __int64\n" );
	fprintf( fp_dsc_h , "#define LL(_ll_)	_ll_\n" );
	fprintf( fp_dsc_h , "#define ULL(_ll_)	_ll_\n" );
	fprintf( fp_dsc_h , "#define LONGLONG_FORMAT_SPEC	\"%%I64\"\n" );
	fprintf( fp_dsc_h , "#define ATOLL	_atoi64\n" );
	fprintf( fp_dsc_h , "#define __BIG_ENDIAN	4321\n" );
	fprintf( fp_dsc_h , "#define __LITTLE_ENDIAN	1234\n" );
	fprintf( fp_dsc_h , "#define __BYTE_ORDER	__LITTLE_ENDIAN\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#ifndef NATOC\n" );
	fprintf( fp_dsc_h , "#define NATOC(_base_,_len_,_result_)	{char buf[3+1];memset(buf,0x00,sizeof(buf));strncpy(buf,_base_,3);_result_=(char)atoi(buf);}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#ifndef NATOS\n" );
	fprintf( fp_dsc_h , "#define NATOS(_base_,_len_,_result_)	{char buf[5+1];memset(buf,0x00,sizeof(buf));strncpy(buf,_base_,5);_result_=(short)atol(buf);}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#ifndef NATOI\n" );
	fprintf( fp_dsc_h , "#define NATOI(_base_,_len_,_result_)	{char buf[10+1];memset(buf,0x00,sizeof(buf));strncpy(buf,_base_,10);_result_=atoi(buf);}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#ifndef NATOLL\n" );
	fprintf( fp_dsc_h , "#define NATOLL(_base_,_len_,_result_)	{char buf[20+1];memset(buf,0x00,sizeof(buf));strncpy(buf,_base_,20);_result_=ATOLL(buf);}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#ifndef NATOF\n" );
	fprintf( fp_dsc_h , "#define NATOF(_base_,_len_,_result_)	{char buf[20+1];memset(buf,0x00,sizeof(buf));strncpy(buf,_base_,20);_result_=(float)atof(buf);}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#ifndef NATOLF\n" );
	fprintf( fp_dsc_h , "#define NATOLF(_base_,_len_,_result_)	{char buf[60+1];memset(buf,0x00,sizeof(buf));strncpy(buf,_base_,60);_result_=atof(buf);}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	
	nret = GenerateCCode_h( pcp , 0 , psi->next_struct , fp_dsc_h ) ;
	if( nret )
		return nret;
	
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCINIT_%s( %s *pst );\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
	
	if( pcp->output_c_order_flag == 1 )
	{
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "#ifndef __HTONLL\n" );
		fprintf( fp_dsc_h , "#define __HTONLL(_ulonglong_)	if(__BYTE_ORDER==__LITTLE_ENDIAN){_ulonglong_=( (((ulonglong)htonl((unsigned int)(((_ulonglong_)<<32)>>32)))<<32) | ((ulonglong)htonl((unsigned int)((_ulonglong_)>>32))) ) ; }\n" );
		fprintf( fp_dsc_h , "#define __NTOHLL(_ulonglong_)	if(__BYTE_ORDER==__LITTLE_ENDIAN){_ulonglong_=( (ulonglong)( (((ulonglong)ntohl((unsigned int)(((_ulonglong_)<<32)>>32)))<<32) | ((ulonglong)ntohl((unsigned int)((_ulonglong_)>>32))) ) ) ; }\n" );
		fprintf( fp_dsc_h , "#endif\n" );
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "#ifndef __HTONF\n" );
		fprintf( fp_dsc_h , "#define __HTONF(_float_)		if(__BYTE_ORDER==__LITTLE_ENDIAN){union{unsigned int ui;float f;}_ui_f_; _ui_f_.f=_float_; _ui_f_.ui=htonl(_ui_f_.ui); _float_=_ui_f_.f; }\n" );
		fprintf( fp_dsc_h , "#define __NTOHF(_float_)		if(__BYTE_ORDER==__LITTLE_ENDIAN){union{unsigned int ui;float f;}_ui_f_; _ui_f_.f=_float_; _ui_f_.ui=ntohl(_ui_f_.ui); _float_=_ui_f_.f; }\n" );
		fprintf( fp_dsc_h , "#endif\n" );
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "#ifndef __HTONLF\n" );
		fprintf( fp_dsc_h , "#define __HTONLF(_double_)		if(__BYTE_ORDER==__LITTLE_ENDIAN){union{ulonglong ull;double d;}_ull_d_; _ull_d_.d=_double_; __HTONLL(_ull_d_.ull); _double_=_ull_d_.d; }\n" );
		fprintf( fp_dsc_h , "#define __NTOHLF(_double_)		if(__BYTE_ORDER==__LITTLE_ENDIAN){union{ulonglong ull;double d;}_ull_d_; _ull_d_.d=_double_; __NTOHLL(_ull_d_.ull); _double_=_ull_d_.d; }\n" );
		fprintf( fp_dsc_h , "#endif\n" );
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCNETORDER_%s( %s *pst );\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCHOSTORDER_%s( %s *pst );\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
	}
	
	if( pcp->output_c_compact_flag == 1 )
	{
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "#define DSCSERIALIZE_COMPACT_BUFSIZE_%s	%d\n" , psi->next_struct->struct_name , psi->next_struct->struct_length );
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_COMPACT_%s( %s *pst , char *buf , int *p_len );\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_COMPACT_%s( char *buf , int *p_len , %s *pst );\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
	}
	
	if( pcp->output_c_compress_flag == 1 )
	{
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
		fprintf( fp_dsc_h , "			_n_ = _h_ ;						\\\n" );
		fprintf( fp_dsc_h , "			__HTONLL( _n_ );					\\\n" );
		fprintf( fp_dsc_h , "			_flag_ |= 0x10 ;					\\\n" );
		fprintf( fp_dsc_h , "		}								\\\n" );
		fprintf( fp_dsc_h , "		else								\\\n" );
		fprintf( fp_dsc_h , "		{								\\\n" );
		fprintf( fp_dsc_h , "			_h_ = _int8_ ;						\\\n" );
		fprintf( fp_dsc_h , "			_n_ = _h_ ;						\\\n" );
		fprintf( fp_dsc_h , "			__HTONLL( _n_ );					\\\n" );
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
		fprintf( fp_dsc_h , "		__NTOHLL( _int8_ );						\\\n" );
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
		fprintf( fp_dsc_h , "		_n_ = _h_ ;							\\\n" );
		fprintf( fp_dsc_h , "		__HTONLL( _n_ );						\\\n" );
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
		fprintf( fp_dsc_h , "		__NTOHLL( _uint8_ );						\\\n" );
		fprintf( fp_dsc_h , "		_bufoffset_+=1+_buf_[0]; _buf_+=1+_buf_[0];			\\\n" );
		fprintf( fp_dsc_h , "	}\n" );
		fprintf( fp_dsc_h , "#endif\n" );
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "#ifndef COMPRESS_FLOAT4\n" );
		fprintf( fp_dsc_h , "#define COMPRESS_FLOAT4(_float4_,_buf_,_bufoffset_)				\\\n" );
		fprintf( fp_dsc_h , "	{									\\\n" );
		fprintf( fp_dsc_h , "		__HTONF( _float4_ );						\\\n" );
		fprintf( fp_dsc_h , "		memcpy( _buf_ , ((unsigned char *)&_n_) , 4 );			\\\n" );
		fprintf( fp_dsc_h , "		_bufoffset_+=4; _buf_+=4 ;					\\\n" );
		fprintf( fp_dsc_h , "	}\n" );
		fprintf( fp_dsc_h , "#endif\n" );
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "#ifndef UNCOMPRESS_FLOAT4\n" );
		fprintf( fp_dsc_h , "#define UNCOMPRESS_FLOAT4(_buf_,_bufoffset_,_float4_)				\\\n" );
		fprintf( fp_dsc_h , "	{									\\\n" );
		fprintf( fp_dsc_h , "		memcpy( ((unsigned char*)&_float4_) , _buf_ , 4 );		\\\n" );
		fprintf( fp_dsc_h , "		__NTOHF( _float4_ );						\\\n" );
		fprintf( fp_dsc_h , "		_bufoffset_+=4; _buf_+=4;					\\\n" );
		fprintf( fp_dsc_h , "	}\n" );
		fprintf( fp_dsc_h , "#endif\n" );
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "#ifndef COMPRESS_FLOAT8\n" );
		fprintf( fp_dsc_h , "#define COMPRESS_FLOAT8(_float8_,_buf_,_bufoffset_)				\\\n" );
		fprintf( fp_dsc_h , "	{									\\\n" );
		fprintf( fp_dsc_h , "		__HTONLF( _float8_ );						\\\n" );
		fprintf( fp_dsc_h , "		memcpy( _buf_ , ((unsigned char *)&_n_) , 8 );			\\\n" );
		fprintf( fp_dsc_h , "		_bufoffset_+=8; _buf_+=8 ;					\\\n" );
		fprintf( fp_dsc_h , "	}\n" );
		fprintf( fp_dsc_h , "#endif\n" );
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "#ifndef UNCOMPRESS_FLOAT8\n" );
		fprintf( fp_dsc_h , "#define UNCOMPRESS_FLOAT8(_buf_,_bufoffset_,_float8_)				\\\n" );
		fprintf( fp_dsc_h , "	{									\\\n" );
		fprintf( fp_dsc_h , "		memcpy( ((unsigned char*)&_float8_) , _buf_ , 8 );		\\\n" );
		fprintf( fp_dsc_h , "		__NTOHLF( _float8_ );						\\\n" );
		fprintf( fp_dsc_h , "		_bufoffset_+=8; _buf_+=8;					\\\n" );
		fprintf( fp_dsc_h , "	}\n" );
		fprintf( fp_dsc_h , "#endif\n" );
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "#define DSCSERIALIZE_COMPRESS_BUFSIZE_%s	%d\n" , psi->next_struct->struct_name , psi->next_struct->struct_length + psi->next_struct->field_count );
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_COMPRESS_%s( %s *pst , char *buf , int *p_len );\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_COMPRESS_%s( char *buf , int *p_len , %s *pst );\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
	}
	
	if( pcp->output_c_xml_flag == 1 )
	{
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_XML_%s( %s *pst , char *encoding , char *buf , int *p_len );\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_XML_%s( char *encoding , char *buf , int *p_len , %s *pst );\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
	}	
	
	if( pcp->output_c_json_flag == 1 )
	{
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_JSON_%s( %s *pst , char *encoding , char *buf , int *p_len );\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_JSON_%s( char *encoding , char *buf , int *p_len , %s *pst );\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
	}	
	
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	
	/* Generate *.dsc.c */
	fprintf( fp_dsc_c , "/* It had generated by DirectStruct v%s */\n" , DIRECTSTRUCT_VERSION );
	fprintf( fp_dsc_c , "#include \"%s.h\"\n" , pcp->pathfilename );
	
	/* Generate DSCINIT_*() */
	fprintf( fp_dsc_c , "\n" );
	fprintf( fp_dsc_c , "int DSCINIT_%s( %s *pst )\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
	fprintf( fp_dsc_c , "{\n" );
	fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
	fprintf( fp_dsc_c , "	memset( pst , 0x00 , sizeof(%s) );\n" , psi->next_struct->struct_name );
	nret = GenerateCCode_c_DSCINIT( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" ) ;
	if( nret )
		return nret;
	fprintf( fp_dsc_c , "	return 0;\n" );
	fprintf( fp_dsc_c , "}\n" );
	
	if( pcp->output_c_order_flag == 1 )
	{
		/* Generate DSCNETORDER_*() */
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "int DSCNETORDER_%s( %s *pst )\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
		nret = GenerateCCode_c_DSCNETORDER( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "	return 0;\n" );
		fprintf( fp_dsc_c , "}\n" );
		
		/* Generate DSCHOSTORDER_*() */
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "int DSCHOSTORDER_%s( %s *pst )\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
		nret = GenerateCCode_c_DSCHOSTORDER( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "	return 0;\n" );
		fprintf( fp_dsc_c , "}\n" );
	}
	
	if( pcp->output_c_compact_flag == 1 )
	{
		/* Generate DSCSERIALIZE_COMPACT_*() */
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "int DSCSERIALIZE_COMPACT_%s( %s *pst , char *buf , int *p_len )\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	char	*ptr = buf ;\n" );
		fprintf( fp_dsc_c , "	int	len = 0 ;\n" );
		fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
		fprintf( fp_dsc_c , "	\n" );
		nret = GenerateCCode_c_DSCSERIALIZE_COMPACT( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	if( p_len ) (*p_len) = len ;\n" );
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	return 0;\n" );
		fprintf( fp_dsc_c , "}\n" );
		
		/* Generate DSCDESERIALIZE_COMPACT_*() */
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "int DSCDESERIALIZE_COMPACT_%s( char *buf , int *p_len , %s *pst )\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	char	*ptr = buf ;\n" );
		fprintf( fp_dsc_c , "	int	len = 0 ;\n" );
		fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
		fprintf( fp_dsc_c , "	\n" );
		nret = GenerateCCode_c_DSCDESERIALIZE_COMPACT( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	if( p_len ) (*p_len) = len ;\n" );
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	return 0;\n" );
		fprintf( fp_dsc_c , "}\n" );
	}
	
	if( pcp->output_c_compress_flag == 1 )
	{
		/* Generate DSCSERIALIZE_COMPRESS_*() */
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "int DSCSERIALIZE_COMPRESS_%s( %s *pst , char *buf , int *p_len )\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	char	*ptr = buf ;\n" );
		fprintf( fp_dsc_c , "	int	len = 0 ;\n" );
		fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
		nret = GenerateCCode_c_DSCSERIALIZE_COMPRESS( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	if( p_len ) (*p_len) = len ;\n" );
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	return 0;\n" );
		fprintf( fp_dsc_c , "}\n" );
		
		/* Generate DSCDESERIALIZE_COMPRESS_*() */
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "int DSCDESERIALIZE_COMPRESS_%s( char *buf , int *p_len , %s *pst )\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	char	*ptr = buf ;\n" );
		fprintf( fp_dsc_c , "	int	len = 0 ;\n" );
		fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
		nret = GenerateCCode_c_DSCDESERIALIZE_COMPRESS( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	if( p_len ) (*p_len) = len ;\n" );
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	return 0;\n" );
		fprintf( fp_dsc_c , "}\n" );
	}
	
	if( pcp->output_c_xml_flag == 1 )
	{
		char	xmlpathname[ 1024 + 1 ] ;
		/* Generate DSCSERIALIZE_XML_*() */
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "#include \"fasterxml.h\"\n" );
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "int DSCSERIALIZE_XML_%s( %s *pst , char *encoding , char *buf , int *p_len )\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	int	remain_len ;\n" );
		fprintf( fp_dsc_c , "	int	len ;\n" );
		fprintf( fp_dsc_c , "	char	tabs[%d+1] ;\n" , DSC_MAXDEPTH );
		fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
		fprintf( fp_dsc_c , "	remain_len = (*p_len) - 1 ;\n" );
		fprintf( fp_dsc_c , "	memset( tabs , '\\t' , %d );\n" , DSC_MAXDEPTH );
		fprintf( fp_dsc_c , "	len=SNPRINTF(buf,remain_len,\"<?xml version=\\\"1.0\\\" encoding=\\\"%%s\\\"?>\\n\",encoding); if( len <= 0 ) return -1; buf+=len; remain_len-=len;\n" );
		nret = GenerateCCode_c_DSCSERIALIZE_XML( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	if( p_len ) (*p_len) = (*p_len)-1 - remain_len ;\n" );
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	return 0;\n" );
		fprintf( fp_dsc_c , "}\n" );
		
		/* Generate DSCDESERIALIZE_XML_*() */
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "funcCallbackOnXmlNode CallbackOnXmlNode_%s ;\n" , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "int CallbackOnXmlNode_%s( int type , char *xpath , int xpath_len , int xpath_size , char *nodename , int nodename_len , char *properties , int properties_len , char *content , int content_len , void *p )\n" , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	%s	*pst = (%s*)p ;\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	if( type & FASTERXML_NODE_BRANCH )\n" );
		fprintf( fp_dsc_c , "	{\n" );
		fprintf( fp_dsc_c , "		if( type & FASTERXML_NODE_ENTER )\n" );
		fprintf( fp_dsc_c , "		{\n" );
		SNPRINTF( xmlpathname , sizeof(xmlpathname)-1 , "/%s" , psi->next_struct->struct_name );
		nret = GenerateCCode_c_DSCDESERIALIZE_XML_ENTERNODE( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" , xmlpathname ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "		}\n" );
		fprintf( fp_dsc_c , "		else if( type & FASTERXML_NODE_LEAVE )\n" );
		fprintf( fp_dsc_c , "		{\n" );
		SNPRINTF( xmlpathname , sizeof(xmlpathname)-1 , "/%s" , psi->next_struct->struct_name );
		nret = GenerateCCode_c_DSCDESERIALIZE_XML_LEAVENODE( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" , xmlpathname ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "		}\n" );
		fprintf( fp_dsc_c , "	}\n" );
		fprintf( fp_dsc_c , "	else if( type & FASTERXML_NODE_LEAF )\n" );
		fprintf( fp_dsc_c , "	{\n" );
		SNPRINTF( xmlpathname , sizeof(xmlpathname)-1 , "/%s" , psi->next_struct->struct_name );
		nret = GenerateCCode_c_DSCDESERIALIZE_XML_LEAF( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" , xmlpathname ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "	}\n" );
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	return 0;\n" );
		fprintf( fp_dsc_c , "}\n" );
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "int DSCDESERIALIZE_XML_%s( char *encoding , char *buf , int *p_len , %s *pst )\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	char	xpath[ 1024 + 1 ] ;\n" );
		fprintf( fp_dsc_c , "	int	nret = 0 ;\n" );
		fprintf( fp_dsc_c , "	memset( xpath , 0x00 , sizeof(xpath) );\n" );
		fprintf( fp_dsc_c , "	nret = TravelXmlBuffer( buf , xpath , sizeof(xpath) , & CallbackOnXmlNode_%s , (void*)pst ) ;\n" , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "	if( nret && nret != FASTERXML_INFO_END_OF_BUFFER )\n" );
		fprintf( fp_dsc_c , "		return nret;\n" );
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	return 0;\n" );
		fprintf( fp_dsc_c , "}\n" );
	}
	
	if( pcp->output_c_json_flag == 1 )
	{
		char	jsonpathname[ 1024 + 1 ] ;
		/* Generate DSCSERIALIZE_JSON_*() */
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "#include \"fasterjson.h\"\n" );
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "int DSCSERIALIZE_JSON_%s( %s *pst , char *encoding , char *buf , int *p_len )\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	int	remain_len ;\n" );
		fprintf( fp_dsc_c , "	int	len ;\n" );
		fprintf( fp_dsc_c , "	char	tabs[%d+1] ;\n" , DSC_MAXDEPTH );
		fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
		fprintf( fp_dsc_c , "	remain_len = (*p_len) - 1 ;\n" );
		fprintf( fp_dsc_c , "	memset( tabs , '\\t' , %d );\n" , DSC_MAXDEPTH );
		nret = GenerateCCode_c_DSCSERIALIZE_JSON( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	if( p_len ) (*p_len) = (*p_len)-1 - remain_len ;\n" );
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	return 0;\n" );
		fprintf( fp_dsc_c , "}\n" );
		
		/* Generate DSCDESERIALIZE_JSON_*() */
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "funcCallbackOnJsonNode CallbackOnJsonNode_%s ;\n" , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "int CallbackOnJsonNode_%s( int type , char *jpath , int jpath_len , int jpath_size , char *nodename , int nodename_len , char *content , int content_len , void *p )\n" , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	%s	*pst = (%s*)p ;\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	if( type & FASTERJSON_NODE_BRANCH )\n" );
		fprintf( fp_dsc_c , "	{\n" );
		fprintf( fp_dsc_c , "		if( type & FASTERJSON_NODE_ENTER )\n" );
		fprintf( fp_dsc_c , "		{\n" );
		SNPRINTF( jsonpathname , sizeof(jsonpathname)-1 , "/%s" , psi->next_struct->struct_name );
		nret = GenerateCCode_c_DSCDESERIALIZE_JSON_ENTERNODE( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" , jsonpathname ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "		}\n" );
		fprintf( fp_dsc_c , "		else if( type & FASTERJSON_NODE_LEAVE )\n" );
		fprintf( fp_dsc_c , "		{\n" );
		SNPRINTF( jsonpathname , sizeof(jsonpathname)-1 , "/%s" , psi->next_struct->struct_name );
		nret = GenerateCCode_c_DSCDESERIALIZE_JSON_LEAVENODE( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" , jsonpathname ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "		}\n" );
		fprintf( fp_dsc_c , "	}\n" );
		fprintf( fp_dsc_c , "	else if( type & FASTERJSON_NODE_LEAF )\n" );
		fprintf( fp_dsc_c , "	{\n" );
		SNPRINTF( jsonpathname , sizeof(jsonpathname)-1 , "/%s" , psi->next_struct->struct_name );
		nret = GenerateCCode_c_DSCDESERIALIZE_JSON_LEAF( pcp , 0 , psi->next_struct , fp_dsc_c , "pst->" , jsonpathname ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "	}\n" );
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	return 0;\n" );
		fprintf( fp_dsc_c , "}\n" );
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "int DSCDESERIALIZE_JSON_%s( char *encoding , char *buf , int *p_len , %s *pst )\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	char	jpath[ 1024 + 1 ] ;\n" );
		fprintf( fp_dsc_c , "	int	nret = 0 ;\n" );
		fprintf( fp_dsc_c , "	memset( jpath , 0x00 , sizeof(jpath) );\n" );
		fprintf( fp_dsc_c , "	nret = TravelJsonBuffer( buf , jpath , sizeof(jpath) , & CallbackOnJsonNode_%s , (void*)pst ) ;\n" , psi->next_struct->struct_name );
		fprintf( fp_dsc_c , "	if( nret && nret != FASTERJSON_INFO_END_OF_BUFFER )\n" );
		fprintf( fp_dsc_c , "		return nret;\n" );
		fprintf( fp_dsc_c , "	\n" );
		fprintf( fp_dsc_c , "	return 0;\n" );
		fprintf( fp_dsc_c , "}\n" );
	}
	
	if( pcp->output_c_LOG_flag == 1 )
	{
		/* Generate *.dsc.LOG.c */
		fprintf( fp_dsc_LOG_c , "/* It had generated by DirectStruct v%s */\n" , DIRECTSTRUCT_VERSION );
		fprintf( fp_dsc_LOG_c , "#include \"%s.h\"\n" , pcp->pathfilename );
		fprintf( fp_dsc_LOG_c , "\n" );
		fprintf( fp_dsc_LOG_c , "#ifndef FUNCNAME_DSCLOG_%s\n" , psi->next_struct->struct_name );
		fprintf( fp_dsc_LOG_c , "#define FUNCNAME_DSCLOG_%s	DSCLOG_%s\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_LOG_c , "#endif\n" );
		fprintf( fp_dsc_LOG_c , "\n" );
		fprintf( fp_dsc_LOG_c , "#ifndef PREFIX_DSCLOG_%s\n" , psi->next_struct->struct_name );
		fprintf( fp_dsc_LOG_c , "#define PREFIX_DSCLOG_%s	printf( \n" , psi->next_struct->struct_name );
		fprintf( fp_dsc_LOG_c , "#endif\n" );
		fprintf( fp_dsc_LOG_c , "\n" );
		fprintf( fp_dsc_LOG_c , "#ifndef NEWLINE_DSCLOG_%s\n" , psi->next_struct->struct_name );
		fprintf( fp_dsc_LOG_c , "#define NEWLINE_DSCLOG_%s	\"\\n\"\n" , psi->next_struct->struct_name );
		fprintf( fp_dsc_LOG_c , "#endif\n" );
		fprintf( fp_dsc_LOG_c , "\n" );
		fprintf( fp_dsc_LOG_c , "int FUNCNAME_DSCLOG_%s( %s *pst )\n" , psi->next_struct->struct_name , psi->next_struct->struct_name );
		fprintf( fp_dsc_LOG_c , "{\n" );
		fprintf( fp_dsc_LOG_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , DSC_MAXDEPTH );
		SNPRINTF( LOG_pathname , sizeof(LOG_pathname)-1 , "%s." , psi->next_struct->struct_name );
		nret = GenerateCCode_LOG_c( pcp , 0 , psi->next_struct , psi->next_struct , fp_dsc_LOG_c , LOG_pathname , "pst->" ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_LOG_c , "	return 0;\n" );
		fprintf( fp_dsc_LOG_c , "}\n" );
	}
	
	return 0;
}
