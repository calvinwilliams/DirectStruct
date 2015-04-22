#include "dsc.h"

/*
 * dsc - DirectSrtuct compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

int ReadDscFile( struct CommandParameter *pcmdparam , int depth , int *p_offset , char *dsc_pathfilename , int flag , FILE *fp_dsc , int *p_lineno , struct StructInfo *pstruct )
{
	char			filebuffer[ 4096 + 1 ] ;
	char			filebuffer_bak[ 4096 + 1 ] ;
	char			quotas ;
	char			*base = NULL ;
	char			*ptr = NULL ;
	char			*ptr2 = NULL ;
	char			*ptr3 = NULL ;
	char			*ptr6 = NULL ;
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
		
		(*p_lineno)++;
		
		for( ptr = filebuffer , quotas = 0 ; (*ptr) ; ptr++ )
		{
			if( quotas == 0 )
			{
				if( (*ptr) == '#' )
				{
					(*ptr) = '\0' ;
					break;
				}
				else if( (*ptr) == '\'' || (*ptr) == '"' )
				{
					quotas = (*ptr) ;
				}
			}
			else
			{
				if( (*ptr) == quotas )
					quotas = 0 ;
			}
		}
		if( quotas )
		{
			fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer );
			return -1;
		}
		
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
						fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
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
							fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
							return -1;
						}
						
						if( STRCMP( ptr , == , "ARRAY" ) )
						{
							psi_next->array_size = atoi(ptr2) ;
						}
						else
						{
							fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
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
					
					struct_len = ReadDscFile( pcmdparam , depth , p_offset , dsc_pathfilename , 1 , fp_dsc , p_lineno , psi_next ) ;
					if( struct_len < 0 )
						return struct_len;
					
					(*p_offset) += struct_len ;
					pstruct->next_struct = psi_next ;
					pstruct->struct_length += struct_len ;
					
					continue;
				}
				else
				{
					fprintf( stderr , "*** ERROR : %s:%d : invalid keyword [%s]\n" , dsc_pathfilename , (*p_lineno) , ptr );
					return -1;
				}
				
				break;
				
			case 1 :
				if( STRCMP( filebuffer , != , "{" ) )
				{
					fprintf( stderr , "*** ERROR : %s:%d : invalid keyword [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer );
					return -1;
				}
				
				flag = 2 ;
				
				break;
				
			case 2 :
				if( STRCMP( filebuffer , == , "}" ) )
				{
					if( pstruct->array_size == 0 )
						return pstruct->struct_length;
					else
						return pstruct->struct_length * pstruct->array_size ;
				}
				
				base = filebuffer ;
				ptr = strtok2( & base ) ;
				if( ptr == NULL )
				{
					fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
					return -1;
				}
				
				if( STRCMP( ptr , == , "INCLUDE" ) )
				{
					FILE	*include_fp_dsc = NULL ;
					int	lineno ;
					
					ptr = strtok2( & base ) ;
					if( ptr == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
						return -1;
					}
					
					include_fp_dsc = fopen( ptr , "r" ) ;
					if( include_fp_dsc == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%d : file[%s] can't read\n" , dsc_pathfilename , (*p_lineno) , ptr );
						return -1;
					}
					lineno = 0 ;
					struct_len = ReadDscFile( pcmdparam , depth , p_offset , ptr , 2 , include_fp_dsc , & lineno , pstruct ) ;
					fclose( include_fp_dsc );
					if( struct_len < 0 )
						return struct_len;
					pstruct->struct_length += struct_len ;
					
					continue;
				}
				else if( STRCMP( ptr , == , "STRUCT" ) )
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
						fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
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
							fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
							return -1;
						}
						
						if( STRCMP( ptr , == , "ARRAY" ) )
						{
							psi_sub->array_size = atoi(ptr2) ;
						}
						else
						{
							fprintf( stderr , "*** ERROR : %s:%d : invalid keyword [%s]\n" , dsc_pathfilename , (*p_lineno) , ptr );
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
					
					struct_len = ReadDscFile( pcmdparam , depth + 1 , p_offset , dsc_pathfilename , 1 , fp_dsc , p_lineno , psi_sub ) ;
					if( struct_len < 0 )
						return struct_len;
					
					if( pstruct->sub_struct_list == NULL )
					{
						pstruct->sub_struct_list = psi_sub ;
						pstruct->last_sub_struct = psi_sub ;
					}
					else
					{
						pstruct->last_sub_struct->next_struct = psi_sub ;
						pstruct->last_sub_struct = psi_sub ;
					}
					
					pstruct->struct_length += struct_len ;
					if( psi_sub->array_size == 0 )
						pstruct->field_count += psi_sub->field_count ;
					else
						pstruct->field_count += psi_sub->field_count * psi_sub->array_size ;
					
					continue;
				}
				else if( STRCMP( ptr , == , "H" ) )
				{
					struct HeaderOutput	*line = NULL ;
					
					line = (struct HeaderOutput *)malloc( sizeof(struct HeaderOutput) ) ;
					if( line == NULL )
					{
						fprintf( stderr , "*** ERROR : : alloc failed , errno[%d]\n" , errno );
						return -1;
					}
					memset( line , 0x00 , sizeof(struct HeaderOutput) );
					
					ptr2 = strtok2( & base ) ;
					if( ptr2 == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
						return -1;
					}
					
					line->content = strdup( ptr2 ) ;
					if( line->content == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
						return -1;
					}
					
					if( pstruct->first_line == NULL )
					{
						pstruct->first_line = line ;
						pstruct->last_line = line ;
					}
					else
					{
						pstruct->last_line->next_line = line ;
						pstruct->last_line = line ;
					}
					
					continue;
				}
				else if( STRCMP( ptr , == , "CREATE_SQL" ) )
				{
					struct CreateSqlOutput	*line = NULL ;
					
					line = (struct CreateSqlOutput *)malloc( sizeof(struct CreateSqlOutput) ) ;
					if( line == NULL )
					{
						fprintf( stderr , "*** ERROR : : alloc failed , errno[%d]\n" , errno );
						return -1;
					}
					memset( line , 0x00 , sizeof(struct CreateSqlOutput) );
					
					ptr2 = strtok2( & base ) ;
					if( ptr2 == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
						return -1;
					}
					
					line->content = strdup( ptr2 ) ;
					if( line->content == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
						return -1;
					}
					
					if( pstruct->first_create_sql_line == NULL )
					{
						pstruct->first_create_sql_line = line ;
						pstruct->last_create_sql_line = line ;
					}
					else
					{
						pstruct->last_create_sql_line->next_line = line ;
						pstruct->last_create_sql_line = line ;
					}
					
					continue;
				}
				else if( STRCMP( ptr , == , "DROP_SQL" ) )
				{
					struct DropSqlOutput	*line = NULL ;
					
					line = (struct DropSqlOutput *)malloc( sizeof(struct DropSqlOutput) ) ;
					if( line == NULL )
					{
						fprintf( stderr , "*** ERROR : : alloc failed , errno[%d]\n" , errno );
						return -1;
					}
					memset( line , 0x00 , sizeof(struct DropSqlOutput) );
					
					ptr2 = strtok2( & base ) ;
					if( ptr2 == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
						return -1;
					}
					
					line->content = strdup( ptr2 ) ;
					if( line->content == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
						return -1;
					}
					
					if( pstruct->first_drop_sql_line == NULL )
					{
						pstruct->first_drop_sql_line = line ;
						pstruct->last_drop_sql_line = line ;
					}
					else
					{
						pstruct->last_drop_sql_line->next_line = line ;
						pstruct->last_drop_sql_line = line ;
					}
					
					continue;
				}
				else if( STRCMP( ptr , == , "SQLACTION" ) )
				{
					struct SqlActionOutput	*line = NULL ;
					
					line = (struct SqlActionOutput *)malloc( sizeof(struct SqlActionOutput) ) ;
					if( line == NULL )
					{
						fprintf( stderr , "*** ERROR : : alloc failed , errno[%d]\n" , errno );
						return -1;
					}
					memset( line , 0x00 , sizeof(struct SqlActionOutput) );
					
					ptr2 = strtok2( & base ) ;
					if( ptr2 == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
						return -1;
					}
					
					if( STRNCMP( ptr2 , == , "SELECT" , 6 ) )
					{
					}
					else if( STRNCMP( ptr2 , == , "INSERT" , 6 ) )
					{
					}
					else if( STRNCMP( ptr2 , == , "UPDATE" , 6 ) )
					{
					}
					else if( STRNCMP( ptr2 , == , "DELETE" , 6 ) )
					{
					}
					else if( STRNCMP( ptr2 , == , "CURSOR" , 6 ) )
					{
					}
					else
					{
						fprintf( stderr , "*** ERROR : %s:%d : invalid sqlaction [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
						return -1;
					}
					
					line->content = strdup( ptr2 ) ;
					if( line->content == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
						return -1;
					}
					
					line->funcname = strdup( ptr2 ) ;
					if( line->funcname == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
						return -1;
					}
					
					ptr6 = line->funcname ;
					while(*ptr6)
					{
						if( strchr( " \t" , (*ptr6) ) )
							(*ptr6) = '_' ;
						else if( (*ptr6) == '*' )
							(*ptr6) = 'A' ;
						else if( (*ptr6) == ',' )
							(*ptr6) = '_' ;
						else if( strncmp( ptr6 , ">=" , 2 ) == 0 )
							strncpy( ptr6++ , "GE" , 2 );
						else if( strncmp( ptr6 , "<=" , 2 ) == 0 )
							strncpy( ptr6++ , "LE" , 2 );
						else if( strncmp( ptr6 , "<>" , 2 ) == 0 )
							strncpy( ptr6++ , "NE" , 2 );
						else if( (*ptr6) == '=' )
							(*ptr6) = 'E' ;
						else if( (*ptr6) == '>' )
							(*ptr6) = 'G' ;
						else if( (*ptr6) == '<' )
							(*ptr6) = 'L' ;
						
						ptr6++;
					}
					
					if( pstruct->first_sqlaction_line == NULL )
					{
						pstruct->first_sqlaction_line = line ;
						pstruct->last_sqlaction_line = line ;
					}
					else
					{
						pstruct->last_sqlaction_line->next_line = line ;
						pstruct->last_sqlaction_line = line ;
					}
					
					continue;
				}
				else if( STRCMP( ptr , == , "SQLCONN" ) )
				{
					pstruct->sqlconn = 1 ;
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
					fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
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
					||
					(
						STRCMP( pfld->field_type , == , "BOOL" )
						&&
						pfld->field_len == 1
					)
				)
				{
					;
				}
				else
				{
					fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
					return -1;
				}
				
				while(1)
				{
					ptr = strtok2( & base ) ;
					if( ptr == NULL )
						break;
					
					if( STRCMP( ptr , == , "DEFAULT" ) )
					{
						ptr2 = strtok2( & base ) ;
						if( ptr2 == NULL )
						{
							fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
							return -1;
						}
						
						strncpy( pfld->init_default , ptr2 , sizeof(pfld->init_default)-1 );
						
						continue;
					}
					
					if( STRCMP( ptr , == , "NULL" ) )
					{
						pfld->null_or_notnull = DSC_FIELD_NULL ;
						
						continue;
					}
					else if( STRCMP( ptr , == , "NOTNULL" ) )
					{
						pfld->null_or_notnull = DSC_FIELD_NOTNULL ;
						
						continue;
					}
					else
					{
						fprintf( stderr , "*** ERROR : invalid keyword [%s]\n" , ptr );
						return -1;
					}
				}
				
				pfld->field_offset = (*p_offset) ;
				
				fprintabs( stdout , depth ); printf( "	%s %d %s\n" , pfld->field_type , pfld->field_len , pfld->field_name );
				
				pstruct->struct_length += pfld->field_len ;
				pstruct->field_count++;
				(*p_offset) += pfld->field_len ;
				if( pstruct->field_list == NULL )
				{
					pstruct->field_list = pfld ;
				}
				else
				{
					pstruct->last_field->next_field = pfld ;
				}
				pstruct->last_field = pfld ;
				
				break;
		}
	}
	
	if( pstruct->array_size == 0 )
		return pstruct->struct_length;
	else
		return pstruct->struct_length * pstruct->array_size ;
}
