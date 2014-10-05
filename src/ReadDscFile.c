#include "dsc.h"

/*
 * dsc - DirectSrtuct compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

int ReadDscFile( struct CommandParameter *pcmdparam , int depth , int *p_offset , char *dsc_pathfilename , int flag , FILE *fp_dsc , long lineno , struct StructInfo *psi )
{
	char			filebuffer[ 4096 + 1 ] ;
	char			filebuffer_bak[ 4096 + 1 ] ;
	char			*base = NULL ;
	char			*ptr = NULL ;
	char			*ptr2 = NULL ;
	char			*ptr3 = NULL ;
	int			struct_len ;
	
	struct FieldInfo	*pfld = NULL ;
	struct StructInfo	*psi_sub = NULL ;
	struct StructInfo	*psi_next = NULL ;
	
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
		
		strcpy( filebuffer_bak , filebuffer );
		
		switch( flag )
		{
			case 0 :
				base = filebuffer ;
				ptr = strtok2( & base ) ;
				if( STRCMP( ptr , == , "STRUCT" ) )
				{
					psi_next = (struct StructInfo *)malloc( sizeof(struct StructInfo) ) ;
					if( psi_next == NULL )
					{
						fprintf( stderr , "*** ERROR : alloc failed , errno[%d]\n" , errno );
						return -1;
					}
					memset( psi_next , 0x00 , sizeof(struct StructInfo) );
					
					ptr = strtok2( & base ) ;
					if( ptr == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%ld : invalid line [%s]\n" , dsc_pathfilename , lineno , filebuffer_bak );
						return -1;
					}
					strncpy( psi_next->struct_name , ptr , sizeof(psi_next->struct_name)-1 );
					
					while(1)
					{
						ptr = strtok2( & base ) ;
						if( ptr == NULL )
							break;
						
						ptr2 = strtok2( & base ) ;
						if( ptr2 == NULL )
						{
							fprintf( stderr , "*** ERROR : %s:%ld : invalid line [%s]\n" , dsc_pathfilename , lineno , filebuffer_bak );
							return -1;
						}
						
						if( STRCMP( ptr , == , "ARRAY" ) )
						{
							psi_next->array_size = atoi(ptr2) ;
						}
						else
						{
							fprintf( stderr , "*** ERROR : %s:%ld : invalid line [%s]\n" , dsc_pathfilename , lineno , filebuffer_bak );
							return -1;
						}
					}
					
					if( psi_next->array_size > 0 )
					{
						fprintabs( stdout , depth ); printf( "STRUCT %s ARRAY %d\n" , psi_next->struct_name , psi_next->array_size );
					}
					else
					{
						fprintabs( stdout , depth ); printf( "STRUCT %s\n" , psi_next->struct_name );
					}
					
					struct_len = ReadDscFile( pcmdparam , depth , p_offset , dsc_pathfilename , 1 , fp_dsc , lineno , psi_next ) ;
					if( struct_len < 0 )
						return struct_len;
					
					(*p_offset) += struct_len ;
					psi->next_struct = psi_next ;
					psi->struct_length += struct_len ;
					
					continue;
				}
				else
				{
					fprintf( stderr , "*** ERROR : %s:%ld : invalid keyword [%s]\n" , dsc_pathfilename , lineno , ptr );
					return -1;
				}
				
				break;
				
			case 1 :
				if( STRCMP( filebuffer , != , "{" ) )
				{
					fprintf( stderr , "*** ERROR : %s:%ld : invalid keyword [%s]\n" , dsc_pathfilename , lineno , filebuffer );
					return -1;
				}
				
				flag = 2 ;
				
				break;
				
			case 2 :
				
				if( STRCMP( filebuffer , == , "}" ) )
				{
					if( psi->array_size == 0 )
						return psi->struct_length;
					else
						return psi->struct_length * psi->array_size ;
				}
				
				base = filebuffer ;
				ptr = strtok2( & base ) ;
				if( ptr == NULL )
				{
					fprintf( stderr , "*** ERROR : %s:%ld : invalid line [%s]\n" , dsc_pathfilename , lineno , filebuffer_bak );
					return -1;
				}
				
				if( STRCMP( ptr , == , "INCLUDE" ) )
				{
					FILE	*include_fp_dsc = NULL ;
					
					ptr = strtok2( & base ) ;
					if( ptr == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%ld : invalid line [%s]\n" , dsc_pathfilename , lineno , filebuffer_bak );
						return -1;
					}
					
					include_fp_dsc = fopen( ptr , "r" ) ;
					if( include_fp_dsc == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%ld : file[%s] can't read\n" , dsc_pathfilename , lineno , ptr );
						return -1;
					}
					struct_len = ReadDscFile( pcmdparam , depth , p_offset , ptr , 2 , include_fp_dsc , 0 , psi ) ;
					fclose( include_fp_dsc );
					if( struct_len < 0 )
						return struct_len;
					psi->struct_length += struct_len ;
					
					continue;
				}
				if( STRCMP( ptr , == , "STRUCT" ) )
				{
					psi_sub = (struct StructInfo *)malloc( sizeof(struct StructInfo) ) ;
					if( psi_sub == NULL )
					{
						fprintf( stderr , "*** ERROR : : alloc failed , errno[%d]\n" , errno );
						return -1;
					}
					memset( psi_sub , 0x00 , sizeof(struct StructInfo) );
					
					ptr = strtok2( & base ) ;
					if( ptr == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%ld : invalid line [%s]\n" , dsc_pathfilename , lineno , filebuffer_bak );
						return -1;
					}
					strncpy( psi_sub->struct_name , ptr , sizeof(psi_sub->struct_name)-1 );
					
					while(1)
					{
						ptr = strtok2( & base ) ;
						if( ptr == NULL )
							break;
						
						ptr2 = strtok2( & base ) ;
						if( ptr2 == NULL )
						{
							fprintf( stderr , "*** ERROR : %s:%ld : invalid line [%s]\n" , dsc_pathfilename , lineno , filebuffer_bak );
							return -1;
						}
						
						if( STRCMP( ptr , == , "ARRAY" ) )
						{
							psi_sub->array_size = atoi(ptr2) ;
						}
						else
						{
							fprintf( stderr , "*** ERROR : %s:%ld : invalid keyword [%s]\n" , dsc_pathfilename , lineno , ptr );
							return -1;
						}
					}
					
					if( psi_sub->array_size > 0 )
					{
						fprintabs( stdout , depth ); printf( "STRUCT %s ARRAY %d\n" , psi_sub->struct_name , psi_sub->array_size );
					}
					else
					{
						fprintabs( stdout , depth ); printf( "STRUCT %s\n" , psi_sub->struct_name );
					}
					
					struct_len = ReadDscFile( pcmdparam , depth + 1 , p_offset , dsc_pathfilename , 1 , fp_dsc , lineno , psi_sub ) ;
					if( struct_len < 0 )
						return struct_len;
					
					if( psi->sub_struct_list == NULL )
					{
						psi->sub_struct_list = psi_sub ;
						psi->last_sub_struct = psi_sub ;
					}
					else
					{
						psi->last_sub_struct->next_struct = psi_sub ;
						psi->last_sub_struct = psi_sub ;
					}
					
					psi->struct_length += struct_len ;
					if( psi_sub->array_size == 0 )
						psi->field_count += psi_sub->field_count ;
					else
						psi->field_count += psi_sub->field_count * psi_sub->array_size ;
					
					continue;
				}
				
				if( STRCMP( ptr , == , "CREATE_SQL" ) )
				{
					if( psi->create_sql_count >= sizeof(psi->create_sql) / sizeof(psi->create_sql[psi->create_sql_count]) )
					{
						fprintf( stderr , "*** ERROR : %s:%ld : too many create sql\n" , dsc_pathfilename , lineno );
						return -1;
					}
					
					ptr2 = strtok2( & base ) ;
					if( ptr2 == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%ld : invalid line [%s]\n" , dsc_pathfilename , lineno , filebuffer_bak );
						return -1;
					}
					strncpy( psi->create_sql[psi->create_sql_count] , ptr2 , sizeof(psi->create_sql[psi->create_sql_count])-1 );
					psi->create_sql_count++;
					
					continue;
				}
				else if( STRCMP( ptr , == , "DROP_SQL" ) )
				{
					if( psi->drop_sql_count >= sizeof(psi->drop_sql) / sizeof(psi->drop_sql[psi->drop_sql_count]) )
					{
						fprintf( stderr , "*** ERROR : %s:%ld : too many drop sql\n" , dsc_pathfilename , lineno );
						return -1;
					}
					
					ptr2 = strtok2( & base ) ;
					if( ptr2 == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%ld : invalid line [%s]\n" , dsc_pathfilename , lineno , filebuffer_bak );
						return -1;
					}
					strncpy( psi->drop_sql[psi->drop_sql_count] , ptr2 , sizeof(psi->drop_sql[psi->drop_sql_count])-1 );
					psi->drop_sql_count++;
					
					continue;
				}
				
				pfld = (struct FieldInfo *)malloc( sizeof(struct FieldInfo) ) ;
				if( pfld == NULL )
				{
					fprintf( stderr , "*** ERROR : : alloc failed , errno[%d]\n" , errno );
					return -1;
				}
				memset( pfld , 0x00 , sizeof(struct FieldInfo) );
				
				ptr2 = strtok2( & base ) ;
				ptr3 = strtok2( & base ) ;
				if( ptr2 == NULL || ptr3 == NULL )
				{
					fprintf( stderr , "*** ERROR : %s:%ld : invalid line [%s]\n" , dsc_pathfilename , lineno , filebuffer_bak );
					return -1;
				}
				strncpy( pfld->field_type , ptr , sizeof(pfld->field_type)-1 );
				pfld->field_len = atoi(ptr2) ;
				strncpy( pfld->field_name , ptr3 , sizeof(pfld->field_name)-1 );
				
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
					fprintf( stderr , "*** ERROR : %s:%ld : invalid line [%s]\n" , dsc_pathfilename , lineno , filebuffer_bak );
					return -1;
				}
				
				while(1)
				{
					ptr = strtok2( & base ) ;
					if( ptr == NULL )
						break;
					
					ptr2 = strtok2( & base ) ;
					if( ptr2 == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%ld : invalid line [%s]\n" , dsc_pathfilename , lineno , filebuffer_bak );
						return -1;
					}
					
					if( STRCMP( ptr , == , "DEFAULT" ) )
					{
						strncpy( pfld->init_default , ptr2 , sizeof(pfld->init_default)-1 );
					}
					else
					{
						fprintf( stderr , "*** ERROR : invalid keyword [%s]\n" , ptr );
						return -1;
					}
				}
				
				pfld->field_offset = (*p_offset) ;
				
				fprintabs( stdout , depth ); printf( "	%s %d %s\n" , pfld->field_type , pfld->field_len , pfld->field_name );
				
				psi->struct_length += pfld->field_len ;
				psi->field_count++;
				(*p_offset) += pfld->field_len ;
				if( psi->field_list == NULL )
				{
					psi->field_list = pfld ;
				}
				else
				{
					psi->last_field->next_field = pfld ;
				}
				psi->last_field = pfld ;
				
				break;
		}
	}
	
	if( psi->array_size == 0 )
		return psi->struct_length;
	else
		return psi->struct_length * psi->array_size ;
}
