#include "dsc.h"

/*
 * dsc - DirectSrtuct compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

int ReadDscFile( struct CommandParameter *pcmdparam , int depth , int *p_offset , char *dsc_pathfilename , int flag , FILE *fp_dsc , long lineno , struct StructInfo *pmsginfo )
{
	char			filebuffer[ 4096 + 1 ] ;
	char			tmp[ 64 + 1 ] ;
	char			tmp2[ 64 + 1 ] ;
	int			count ;
	int			struct_len ;
	
	char			include_pathfilename[ 256 + 1 ] ;
	
	struct FieldInfo	*pfld = NULL ;
	struct StructInfo	*pst = NULL ;
	
	if( depth > DSC_MAXDEPTH - 1 )
	{
		fprintf( stderr , "*** ERROR : : too many sub structs\n" );
		return -1;
	}
	
	while(1)
	{
		memset( filebuffer , 0x00 , sizeof(filebuffer) );
		if( fgets( filebuffer , sizeof(filebuffer) , fp_dsc ) == NULL )
			break;
		
		lineno++;
		
		ClearLeft( filebuffer );
		ClearRight( filebuffer );
		StringNoEnter( filebuffer );
		if( STRCMP( filebuffer , == , "" ) )
			continue;
		
		switch( flag )
		{
			case 0 :
				memset( tmp , 0x00 , sizeof(tmp) );
				count = sscanf( filebuffer , "%s" , tmp ) ;
				if( STRCMP( tmp , == , "STRUCT" ) )
				{
					pst = (struct StructInfo *)malloc( sizeof(struct StructInfo) ) ;
					if( pst == NULL )
					{
						fprintf( stderr , "*** ERROR : : alloc failed , errno[%d]\n" , errno );
						return -1;
					}
					
					memset( tmp , 0x00 , sizeof(tmp) );
					memset( tmp2 , 0x00 , sizeof(tmp2) );
					memset( pst , 0x00 , sizeof(struct StructInfo) );
					count = sscanf( filebuffer , "%s %s %s %d" , tmp , pst->struct_name , tmp2 , & (pst->array_size) ) ;
					if( count == 2 && STRCMP( tmp , == , "STRUCT" ) )
					{
						fprintabs( stdout , depth ); printf( "STRUCT %s\n" , pst->struct_name );
					}
					else if( count == 4 && STRCMP( tmp , == , "STRUCT" ) && STRCMP( tmp2 , == , "ARRAY" ) )
					{
						fprintabs( stdout , depth ); printf( "STRUCT %s ARRAY %d\n" , pst->struct_name , pst->array_size );
					}
					else
					{
						fprintf( stderr , "*** ERROR : %s:%ld : line [%s] invalid\n" , dsc_pathfilename , lineno , filebuffer );
						return -1;
					}
					
					struct_len = ReadDscFile( pcmdparam , depth , p_offset , dsc_pathfilename , 1 , fp_dsc , lineno , pst ) ;
					if( struct_len < 0 )
						return struct_len;
					
					(*p_offset) += struct_len ;
					pmsginfo->next_struct = pst ;
					pmsginfo->struct_length += struct_len ;
					
					continue;
				}
				else
				{
					fprintf( stderr , "*** ERROR : %s:%ld : head line [%s] invalid\n" , dsc_pathfilename , lineno , filebuffer );
					return -1;
				}
				
				break;
				
			case 1 :
				if( STRCMP( filebuffer , != , "{" ) )
				{
					fprintf( stderr , "*** ERROR : %s:%ld : second line [%s] invalid\n" , dsc_pathfilename , lineno , filebuffer );
					return -1;
				}
				
				flag = 2 ;
				
				break;
				
			case 2 :
				
				if( STRCMP( filebuffer , == , "}" ) )
				{
					if( pmsginfo->array_size == 0 )
						return pmsginfo->struct_length;
					else
						return pmsginfo->struct_length * pmsginfo->array_size ;
				}
				
				memset( tmp , 0x00 , sizeof(tmp) );
				memset( include_pathfilename , 0x00 , sizeof(include_pathfilename) );
				sscanf( filebuffer , "%s %s" , tmp , include_pathfilename );
				if( STRCMP( tmp , == , "INCLUDE" ) )
				{
					FILE	*include_fp_dsc = NULL ;
					include_fp_dsc = fopen( include_pathfilename , "r" ) ;
					if( include_fp_dsc == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%ld : file[%s] can't read\n" , dsc_pathfilename , lineno , include_pathfilename );
						return -1;
					}
					struct_len = ReadDscFile( pcmdparam , depth , p_offset , include_pathfilename , 2 , include_fp_dsc , 0 , pmsginfo ) ;
					fclose( include_fp_dsc );
					if( struct_len < 0 )
						return struct_len;
					pmsginfo->struct_length += struct_len ;
					
					continue;
				}
				if( STRCMP( tmp , == , "STRUCT" ) )
				{
					pst = (struct StructInfo *)malloc( sizeof(struct StructInfo) ) ;
					if( pst == NULL )
					{
						fprintf( stderr , "*** ERROR : : alloc failed , errno[%d]\n" , errno );
						return -1;
					}
					
					memset( tmp , 0x00 , sizeof(tmp) );
					memset( tmp2 , 0x00 , sizeof(tmp2) );
					memset( pst , 0x00 , sizeof(struct StructInfo) );
					count = sscanf( filebuffer , "%s %s %s %d" , tmp , pst->struct_name , tmp2 , & (pst->array_size) ) ;
					if( count == 2 && STRCMP( tmp , == , "STRUCT" ) )
					{
						fprintabs( stdout , depth + 1 ); printf( "STRUCT %s\n" , pst->struct_name );
					}
					else if( count == 4 && STRCMP( tmp , == , "STRUCT" ) && STRCMP( tmp2 , == , "ARRAY" ) )
					{
						fprintabs( stdout , depth + 1 ); printf( "STRUCT %s ARRAY %d\n" , pst->struct_name , pst->array_size );
					}
					else
					{
						fprintf( stderr , "*** ERROR : %s:%ld : line [%s] invalid\n" , dsc_pathfilename , lineno , filebuffer );
						return -1;
					}
					
					struct_len = ReadDscFile( pcmdparam , depth + 1 , p_offset , dsc_pathfilename , 1 , fp_dsc , lineno , pst ) ;
					if( struct_len < 0 )
						return struct_len;
					
					if( pmsginfo->sub_struct_list == NULL )
					{
						pmsginfo->sub_struct_list = pst ;
						pmsginfo->last_sub_struct = pst ;
					}
					else
					{
						pmsginfo->last_sub_struct->next_struct = pst ;
						pmsginfo->last_sub_struct = pst ;
					}
					
					pmsginfo->struct_length += struct_len ;
					pmsginfo->field_count += pst->field_count * pst->array_size ;
					
					continue;
				}
				
				pfld = (struct FieldInfo *)malloc( sizeof(struct FieldInfo) ) ;
				if( pfld == NULL )
				{
					fprintf( stderr , "*** ERROR : : alloc failed , errno[%d]\n" , errno );
					return -1;
				}
				memset( pfld , 0x00 , sizeof(struct FieldInfo) );
				
				memset( tmp , 0x00 , sizeof(tmp) );
				count = sscanf( filebuffer , "%s %d %s %s %[^\n]" , pfld->field_type , & (pfld->field_len) , pfld->field_name , tmp , pfld->init_default ) ;
				if(	(
						( STRCMP( pfld->field_type , == , "INT" ) || STRCMP( pfld->field_type , == , "UINT" ) )
						&&
						( pfld->field_len == 1 || pfld->field_len == 2 || pfld->field_len == 4 || pfld->field_len == 8 )
					)
					||
					(
						STRCMP( pfld->field_type , == , "FLOAT" )
						&&
						( pfld->field_len == 4 || pfld->field_len == 8 )
					)
					||
					(
						( STRCMP( pfld->field_type , == , "CHAR" ) || STRCMP( pfld->field_type , == , "UCHAR" ) )
						&&
						pfld->field_len == 1
					)
					||
					(
						STRCMP( pfld->field_type , == , "STRING" )
						&&
						pfld->field_len > 0
					)
				)
				{
					;
				}
				else
				{
					fprintf( stderr , "*** ERROR : %s:%ld : line [%s] invalid\n" , dsc_pathfilename , lineno , filebuffer );
					return -1;
				}
				
				if( STRCMP( tmp , == , "" ) )
				{
				}
				else if( STRCMP( tmp , == , "DEFAULT" ) )
				{
				}
				else
				{
					fprintf( stderr , "*** ERROR : %s:%ld : line [%s] invalid\n" , dsc_pathfilename , lineno , filebuffer );
					return -1;
				}
				
				pfld->field_offset = (*p_offset) ;
				
				fprintabs( stdout , depth ); printf( "	%s %d %s\n" , pfld->field_type , pfld->field_len , pfld->field_name );
				
				pmsginfo->struct_length += pfld->field_len ;
				pmsginfo->field_count++;
				(*p_offset) += pfld->field_len ;
				/*
				if( STRCMP( pfld->field_type , == , "STRING" ) )
				{
					pmsginfo->struct_length++;
					(*p_offset)++;
				}
				*/
				if( pmsginfo->field_list == NULL )
				{
					pmsginfo->field_list = pfld ;
				}
				else
				{
					pmsginfo->last_field->next_field = pfld ;
				}
				pmsginfo->last_field = pfld ;
				
				break;
		}
	}
	
	if( pmsginfo->array_size == 0 )
		return pmsginfo->struct_length;
	else
		return pmsginfo->struct_length * pmsginfo->array_size ;
}
