/*
 * dsc - DirectSrtuct compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char	__DIRECTSTRUCT_VERSION_1_11_0[] = "1.11.0" ;
char	*__DIRECTSTRUCT_VERSION = __DIRECTSTRUCT_VERSION_1_11_0 ;

#ifndef STRCMP
#define STRCMP(_a_,_C_,_b_) ( strcmp(_a_,_b_) _C_ 0 )
#define STRNCMP(_a_,_C_,_b_,_n_) ( strncmp(_a_,_b_,_n_) _C_ 0 )
#endif

#ifndef MEMCMP
#define MEMCMP(_a_,_C_,_b_,_n_) ( memcmp(_a_,_b_,_n_) _C_ 0 )
#endif

#if ( defined __unix ) || ( defined __linux__ )
#define SNPRINTF		snprintf
#define VSNPRINTF		vsnprintf
#define STDOUT_HANDLE		STDOUT_FILENO
#define STDERR_HANDLE		STDERR_FILENO
#define FD_NULL			-1
#define WRITE			write
#define STAT			stat
#define ACCESS			access
#define ACCESS_OK		F_OK
#define RENAME			rename
#define STRICMP(_a_,_C_,_b_) ( strcasecmp(_a_,_b_) _C_ 0 )
#define STRNICMP(_a_,_C_,_b_,_n_) ( strncasecmp(_a_,_b_,_n_) _C_ 0 )
#elif ( defined _WIN32 )
#define SNPRINTF		_snprintf
#define VSNPRINTF		_vsnprintf
#define STDOUT_HANDLE		1
#define STDERR_HANDLE		2
#define FD_NULL			-1
#define WRITE			_write
#define STAT			_stat
#define ACCESS			_access
#define ACCESS_OK		00
#define RENAME			rename
#define STRICMP(_a_,_C_,_b_) ( stricmp(_a_,_b_) _C_ 0 )
#define STRNICMP(_a_,_C_,_b_,_n_) ( strnicmp(_a_,_b_,_n_) _C_ 0 )
#endif

#if ( defined _WIN32 )
char *strtok_r (char *s, const char *delim, char **save_ptr);
#endif

struct CommandParameter
{
	char	*pathfilename ;
	int	output_c_flag ;
	int	output_c_order_flag ;
	int	output_c_compact_flag ;
	int	output_c_compress_flag ;
	int	output_c_xml_flag ;
	int	output_c_xml_compact_flag ;
	int	output_c_json_flag ;
	int	output_c_json_compact_flag ;
	int	output_c_LOG_flag ;
	int	output_sql_pgsql_flag ;
	int	output_sql_oracle_flag ;
	int	output_ec_pgsql_flag ;
	int	output_ec_oracle_flag ;
} ;

#define DSC_MAXDEPTH		10

struct StructInfo ;

#define DSC_FIELD_NULL		1
#define DSC_FIELD_NOTNULL	2

struct FieldInfo
{
	char			field_type[ 10 + 1 ] ;
	int			field_len ;
	int			field_len2 ;
	int			field_offset ;
	char			field_name[ 64 + 1 ] ;
	char			field_desc[ 256 + 1 ] ;
	
	char			init_default[ 1024 + 1 ] ;
	int			null_or_notnull ;
	
	struct FieldInfo	*next_field ;
	
	struct StructInfo	*struct_list ;
} ;

struct HeaderOutput
{
	char			*content ;
	struct HeaderOutput	*next_line ;
} ;

struct SqlConnectionName
{
	char				*connection_name ;
	struct SqlConnectionName	*next_line ;
} ;

struct CreateSqlOutput
{
	char			*content ;
	struct CreateSqlOutput	*next_line ;
} ;

struct DropSqlOutput
{
	char			*content ;
	struct DropSqlOutput	*next_line ;
} ;

struct SqlActionOutput
{
	char			*content ;
	char			*funcname ;
	struct SqlActionOutput	*next_line ;
	char			*connection_name ;
} ;

struct StructInfo
{
	char			struct_name[ 64 + 1 ] ;
	char			struct_desc[ 256 + 1 ] ;
	int			struct_length ;
	int			field_count ;
	int			array_size ;
	
	struct HeaderOutput	*first_line ;
	struct HeaderOutput	*last_line ;
	struct SqlConnectionName	*first_connection_name_line ;
	struct SqlConnectionName	*last_connection_name_line ;
	struct CreateSqlOutput	*first_create_sql_line ;
	struct CreateSqlOutput	*last_create_sql_line ;
	struct DropSqlOutput	*first_drop_sql_line ;
	struct DropSqlOutput	*last_drop_sql_line ;
	struct SqlActionOutput	*first_sqlaction_line ;
	struct SqlActionOutput	*last_sqlaction_line ;
	int			sqlconn ;
	
	struct FieldInfo	*first_field ;
	struct FieldInfo	*last_field ;
	
	struct StructInfo	*first_sub_struct ;
	struct StructInfo	*last_sub_struct ;
	
	struct StructInfo	*next_struct ;
} ;

/* for future
struct EnumMacro
{
	char			macro[ 256 + 1 ] ;
	int			value ;

	struct EnumMacro	*next_macro ;
} ;

struct EnumInfo
{
	char			enum_name[ 64 + 1 ] ;
	struct EnumMacro	*macro_list ;
	
	struct EnumInfo		*next_enum ;
} ;
*/

#if ( defined _WIN32 )
static char* __rawmemchr (s, c_in)
     const char* s;
     int c_in;
{
  const unsigned char *char_ptr;
  const unsigned long int *longword_ptr;
  unsigned long int longword, magic_bits, charmask;
  unsigned char c;

  c = (unsigned char) c_in;

  /* Handle the first few characters by reading one character at a time.
     Do this until CHAR_PTR is aligned on a longword boundary.  */
  for (char_ptr = (const unsigned char *) s;
       ((unsigned long int) char_ptr & (sizeof (longword) - 1)) != 0;
       ++char_ptr)
    if (*char_ptr == c)
      return (char*) char_ptr;

  /* All these elucidatory comments refer to 4-byte longwords,
     but the theory applies equally well to 8-byte longwords.  */

  longword_ptr = (unsigned long int *) char_ptr;

  /* Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
     the "holes."  Note that there is a hole just to the left of
     each byte, with an extra at the end:

     bits:  01111110 11111110 11111110 11111111
     bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD

     The 1-bits make sure that carries propagate to the next 0-bit.
     The 0-bits provide holes for carries to fall into.  */

  if (sizeof (longword) != 4 && sizeof (longword) != 8)
    abort ();

#if LONG_MAX <= LONG_MAX_32_BITS
  magic_bits = 0x7efefeff;
#else
  magic_bits = ((unsigned long int) 0x7efefefe << 32) | 0xfefefeff;
#endif

  /* Set up a longword, each of whose bytes is C.  */
  charmask = c | (c << 8);
  charmask |= charmask << 16;
#if LONG_MAX > LONG_MAX_32_BITS
  charmask |= charmask << 32;
#endif

  /* Instead of the traditional loop which tests each character,
     we will test a longword at a time.  The tricky part is testing
     if *any of the four* bytes in the longword in question are zero.  */
  while (1)
    {
      /* We tentatively exit the loop if adding MAGIC_BITS to
	 LONGWORD fails to change any of the hole bits of LONGWORD.

	 1) Is this safe?  Will it catch all the zero bytes?
	 Suppose there is a byte with all zeros.  Any carry bits
	 propagating from its left will fall into the hole at its
	 least significant bit and stop.  Since there will be no
	 carry from its most significant bit, the LSB of the
	 byte to the left will be unchanged, and the zero will be
	 detected.

	 2) Is this worthwhile?  Will it ignore everything except
	 zero bytes?  Suppose every byte of LONGWORD has a bit set
	 somewhere.  There will be a carry into bit 8.  If bit 8
	 is set, this will carry into bit 16.  If bit 8 is clear,
	 one of bits 9-15 must be set, so there will be a carry
	 into bit 16.  Similarly, there will be a carry into bit
	 24.  If one of bits 24-30 is set, there will be a carry
	 into bit 31, so all of the hole bits will be changed.

	 The one misfire occurs when bits 24-30 are clear and bit
	 31 is set; in this case, the hole at bit 31 is not
	 changed.  If we had access to the processor carry flag,
	 we could close this loophole by putting the fourth hole
	 at bit 32!

	 So it ignores everything except 128's, when they're aligned
	 properly.

	 3) But wait!  Aren't we looking for C, not zero?
	 Good point.  So what we do is XOR LONGWORD with a longword,
	 each of whose bytes is C.  This turns each byte that is C
	 into a zero.  */

      longword = *longword_ptr++ ^ charmask;

      /* Add MAGIC_BITS to LONGWORD.  */
      if ((((longword + magic_bits)

	    /* Set those bits that were unchanged by the addition.  */
	    ^ ~longword)

	   /* Look at only the hole bits.  If any of the hole bits
	      are unchanged, most likely one of the bytes was a
	      zero.  */
	   & ~magic_bits) != 0)
	{
	  /* Which of the bytes was C?  If none of them were, it was
	     a misfire; continue the search.  */

	  const unsigned char *cp = (const unsigned char *) (longword_ptr - 1);

	  if (cp[0] == c)
	    return (char*) cp;
	  if (cp[1] == c)
	    return (char*) &cp[1];
	  if (cp[2] == c)
	    return (char*) &cp[2];
	  if (cp[3] == c)
	    return (char*) &cp[3];
#if LONG_MAX > 2147483647
	  if (cp[4] == c)
	    return (char*) &cp[4];
	  if (cp[5] == c)
	    return (char*) &cp[5];
	  if (cp[6] == c)
	    return (char*) &cp[6];
	  if (cp[7] == c)
	    return (char*) &cp[7];
#endif
	}
    }
}

char *strtok_r (char *s, const char *delim, char **save_ptr)
{
  char *token;

  if (s == NULL)
    s = *save_ptr;

  /* Scan leading delimiters.  */
  s += strspn (s, delim);
  if (*s == '\0')
    {
      *save_ptr = s;
      return NULL;
    }

  /* Find the end of the token.  */
  token = s;
  s = strpbrk (token, delim);
  if (s == NULL)
    /* This token finishes the string.  */
    *save_ptr = __rawmemchr (token, '\0');
  else
    {
      /* Terminate the token and make *SAVE_PTR point past it.  */
      *s = '\0';
      *save_ptr = s + 1;
    }
  return token;
}
#endif

char *strtok2( char **base )
{
	char	*begin = NULL ;
	
	if( base == NULL || (*base) == NULL )
		return NULL;
	
	for( ; (**base) ; (*base)++ )
	{
		if( ! strchr( " \t\r\n" , (**base) ) )
			break;
	}
	if( (**base) == '\0' )
		return NULL;
	
	if( strchr( "\"\'" , (**base) ) )
	{
		(*base)++;
		begin = (*base) ;
		
		for( ; (**base) ; (*base)++ )
		{
			if( (**base) == *(begin-1) )
				break;
		}
		if( (**base) == '\0' )
			return NULL;
		
		(**base) = '\0' ;
		(*base)++;
	}
	else
	{
		begin = (*base) ;
		
		for( ; (**base) ; (*base)++ )
		{
			if( strchr( " \t\r\n" , (**base) ) )
				break;
		}
		if( (**base) == '\0' )
		{
			(**base) = '\0' ;
			(*base) = NULL ;
		}
		else
		{
			(**base) = '\0' ;
			(*base)++;
		}
	}
	
	return begin;
}

int ClearLeft( char *str )
{
	char *pc = NULL ;
	long l;
	long len = strlen( str ) ;
	
	if( str == NULL )
		return -1;
	
	if( *str == '\0' )
		return -2;
	
	pc = str ;
	while(1)
	{
		if
		(
			*pc == ' '
			||
			*pc == '\t'
		)
		{
			;
		}
		else if
		(
			pc < str + len - 1
			&&
			(
				(unsigned char)(*pc) == 0xA1
				&&
				(unsigned char)(*(pc+1)) == 0xA1
			)
		)
		{
			pc += 1 ;
		}
		else
		{
			break;
		}
		
		pc++;
		
		if( pc == str + len )
			break;
	}
	
	if( pc != str )
	{
		len = strlen( pc );
		for( l = 0 ; l < len + 1 ; l++ )
			*( str + l ) = *( pc + l ) ;
	}
	
	return 0;
}

int ClearRight( char *str )
{
	char *pc = NULL ;
	long len;
	
	if( str == NULL )
		return -1;
	
	if( *str == '\0' )
		return -2;
	
	len = strlen( str ) ;
	pc = str + len - 1 ;
	while(1)
	{
		if
		(
			*pc == ' '
			||
			*pc == '\t'
		)
		{
			;
		}
		else if
		(
			pc > str
			&&
			(
				(unsigned char)(*pc) == 0xA1
				&&
				(unsigned char)(*(pc-1)) == 0xA1
			)
		)
		{
			pc -= 1 ;
		}
		else
		{
			*( pc + 1 ) = '\0' ;
			
			break;
		}
		
		if( pc == str )
		{
			(*pc) = '\0' ;
			
			break;
		}
		
		pc--;
	}
	
	return 0;
}

char *StringNoEnter( char *str )
{
	char	*ptr = NULL ;
	
	if( str == NULL )
		return NULL;
	
	for( ptr = str + strlen(str) - 1 ; ptr >= str ; ptr-- )
	{
		if( (*ptr) == '\r' || (*ptr) == '\n' )
			(*ptr) = '\0' ;
		else
			break;
	}
	
	return str;
}

void fprintabs( FILE *fp , int depth )
{
	int		i ;
	
	for( i = 0 ; i < depth ; i++ )
	{
		fprintf( fp , "\t" );
	}
	
	return;
}

int ReadDscFile( struct CommandParameter *pcmdparam , int depth , int *p_offset , char *dsc_pathfilename , int flag , FILE *fp_dsc , int *p_lineno , struct StructInfo *pstruct )
{
	char			filebuffer[ 4096 + 1 ] ;
	char			filebuffer_bak[ 4096 + 1 ] ;
	char			quotas ;
	char			desc[ 256 + 1 ] ;
	char			*base = NULL ;
	char			*ptr = NULL ;
	char			*ptr2 = NULL ;
	char			*ptr3 = NULL ;
	char			*ptr6 = NULL ;
	char			*ptr8 = NULL ;
	int			struct_len ;
	
	struct FieldInfo	*pfld = NULL ;
	struct StructInfo	*psi_sub = NULL ;
	struct StructInfo	*psi_next = NULL ;
	
	if( depth > DSC_MAXDEPTH - 1 )
	{
		fprintf( stderr , "*** ERROR : too many sub structs\n" );
		return -1;
	}
	
	while(1)
	{
		memset( filebuffer , 0x00 , sizeof(filebuffer) );
		if( fgets( filebuffer , sizeof(filebuffer) , fp_dsc ) == NULL )
			break;
		
		(*p_lineno)++;
		
		memset( desc , 0x00 , sizeof(desc) );
		for( ptr = filebuffer , quotas = 0 ; (*ptr) ; ptr++ )
		{
			if( quotas == 0 )
			{
				if( (*ptr) == '#' )
				{
					strncpy( desc , ptr+1 , sizeof(desc)-1 );
					ClearLeft( desc );
					ClearRight( desc );
					StringNoEnter( desc );
					
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
					strcpy( psi_next->struct_desc , desc );
					
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
						fprintabs( stdout , depth ); printf( "STRUCT %s ARRAY %d DESC %s\n" , psi_next->struct_name , psi_next->array_size , psi_next->struct_desc );
					}
					else
					{
						fprintabs( stdout , depth ); printf( "STRUCT %s DESC %s\n" , psi_next->struct_name , psi_next->struct_desc );
					}
					
					struct_len = ReadDscFile( pcmdparam , depth , p_offset , dsc_pathfilename , 1 , fp_dsc , p_lineno , psi_next ) ;
					if( struct_len < 0 )
						return struct_len;
					
					(*p_offset) += struct_len ;
					if( pstruct->next_struct == NULL )
					{
						pstruct->next_struct = psi_next ;
					}
					else
					{
						struct StructInfo	*last_struct = NULL ;
						for( last_struct = pstruct->next_struct ; last_struct->next_struct ; last_struct = last_struct->next_struct )
							;
						last_struct->next_struct = psi_next ;
					}
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
						fprintf( stderr , "*** ERROR : alloc failed , errno[%d]\n" , errno );
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
					strcpy( psi_sub->struct_desc , desc );
					
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
						fprintabs( stdout , depth ); printf( "STRUCT %s ARRAY %d DESC %s\n" , psi_sub->struct_name , psi_sub->array_size , psi_sub->struct_desc );
					}
					else
					{
						fprintabs( stdout , depth ); printf( "STRUCT %s DESC %s\n" , psi_sub->struct_name , psi_sub->struct_desc );
					}
					
					struct_len = ReadDscFile( pcmdparam , depth + 1 , p_offset , dsc_pathfilename , 1 , fp_dsc , p_lineno , psi_sub ) ;
					if( struct_len < 0 )
						return struct_len;
					
					if( pstruct->first_sub_struct == NULL )
					{
						pstruct->first_sub_struct = psi_sub ;
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
						fprintf( stderr , "*** ERROR : alloc failed , errno[%d]\n" , errno );
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
						fprintf( stderr , "*** ERROR : alloc failed , errno[%d]\n" , errno );
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
						fprintf( stderr , "*** ERROR : alloc failed , errno[%d]\n" , errno );
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
						fprintf( stderr , "*** ERROR : alloc failed , errno[%d]\n" , errno );
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
					
					ptr3 = strtok2( & base ) ;
					if( ptr3 )
					{
						struct SqlConnectionName	*pconnection = NULL ;
						
						for( pconnection = pstruct->first_connection_name_line ; pconnection ; pconnection = pconnection->next_line )
						{
							if( pconnection->connection_name && STRCMP( pconnection->connection_name , == , ptr3 ) )
								break;
						}
						if( pconnection == NULL )
						{
							fprintf( stderr , "*** ERROR : connection name[%s] not found\n" , ptr3 );
							return -1;
						}
						
						line->connection_name = strdup( ptr3 ) ;
						if( line->connection_name == NULL )
						{
							fprintf( stderr , "*** ERROR : alloc failed , errno[%d]\n" , errno );
							return -1;
						}
					}
					
					if( line->connection_name == NULL )
					{
						line->funcname = strdup( ptr2 ) ;
						if( line->funcname == NULL )
						{
							fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
							return -1;
						}
					}
					else
					{
						line->funcname = (char*)malloc( strlen(ptr2) + 4 + strlen(line->connection_name) + 1 ) ;
						if( line->funcname == NULL )
						{
							fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
							return -1;
						}
						sprintf( line->funcname , "%s_AT_%s" , ptr2 , line->connection_name );
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
					struct SqlConnectionName	*line = NULL ;
					
					pstruct->sqlconn = 1 ;
					
					line = (struct SqlConnectionName *)malloc( sizeof(struct SqlConnectionName) ) ;
					if( line == NULL )
					{
						fprintf( stderr , "*** ERROR : alloc failed , errno[%d]\n" , errno );
						return -1;
					}
					memset( line , 0x00 , sizeof(struct SqlConnectionName) );
					
					ptr2 = strtok2( & base ) ;
					if( ptr2 )
					{
						line->connection_name = strdup( ptr2 ) ;
						if( line->connection_name == NULL )
						{
							fprintf( stderr , "*** ERROR : %s:%d : invalid line [%s]\n" , dsc_pathfilename , (*p_lineno) , filebuffer_bak );
							return -1;
						}
					}
					
					if( pstruct->first_connection_name_line == NULL )
					{
						pstruct->first_connection_name_line = line ;
						pstruct->last_connection_name_line = line ;
					}
					else
					{
						pstruct->last_connection_name_line->next_line = line ;
						pstruct->last_connection_name_line = line ;
					}
					
					continue;
				}
				
				pfld = (struct FieldInfo *)malloc( sizeof(struct FieldInfo) ) ;
				if( pfld == NULL )
				{
					fprintf( stderr , "*** ERROR : alloc failed , errno[%d]\n" , errno );
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
				ptr8 = strchr( ptr2 , ',' ) ;
				if( ptr8 )
					pfld->field_len2 = atoi(ptr8+1) ;
				strncpy( pfld->field_name , ptr3 , sizeof(pfld->field_name)-1 );
				strcpy( pfld->field_desc , desc );
				
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
					||
					(
						STRCMP( pfld->field_type , == , "NUMERIC" )
						&&
						( pfld->field_len > 0 && pfld->field_len2 > 0 )
					)
					||
					(
						STRCMP( pfld->field_type , == , "AMOUNT" )
						&&
						( pfld->field_len > 8 && (pfld->field_len-8) % 2 == 0 )
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
				
				fprintabs( stdout , depth ); printf( "	%s %d %s %s\n" , pfld->field_type , pfld->field_len , pfld->field_name , pfld->field_desc );
				
				pstruct->struct_length += pfld->field_len ;
				pstruct->field_count++;
				(*p_offset) += pfld->field_len ;
				if( pstruct->first_field == NULL )
				{
					pstruct->first_field = pfld ;
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

int GenerateCCode_h( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_h )
{
	struct StructInfo	*pstruct = NULL ;
	struct HeaderOutput	*line = NULL ;
	struct FieldInfo	*pfield = NULL ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		for( line = pstruct->first_line ; line ; line = line->next_line )
		{
			fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "%s\n" , line->content );
		}
		
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
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	char	%s ;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	short	%s ;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	int	%s ;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	longlong %s ;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	unsigned char	%s ;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	unsigned short	%s ;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	unsigned int	%s ;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	unsigned longlong %s ;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	float	%s ;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	double	%s ;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	double	%s ;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	double	%s ;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	char	%s ;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	unsigned char %s ;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	char	%s[ %d + 1 ] ;\n" , pfield->field_name , pfield->field_len );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "	char	%s ;\n" , pfield->field_name );
				}
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_h( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_h ) ;
			if( nret )
				return nret;
		}
		
		if( depth == 0 )
		{
			fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "} %s ;\n" , pstruct->struct_name );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "} %s ;\n" , pstruct->struct_name );
			}
			else
			{
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "} %s [ %d ] ;\n" , pstruct->struct_name , pstruct->array_size );
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "int	_%s_count ;\n" , pstruct->struct_name );
				fprintabs( fp_dsc_h , depth ); fprintf( fp_dsc_h , "int	_%s_size ;\n" , pstruct->struct_name );
			}
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCINIT( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pstruct->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->init_default , == , "" ) )
				continue;
			
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
				}
			}
			else if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	strcpy( %s%s , \"%s\" );\n" , pathname,pfield->field_name , pfield->init_default );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = %s ;\n" , pathname,pfield->field_name , pfield->init_default );
				}
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCINIT( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s_%s_size = %d ;\n" , up_pathname,pstruct->struct_name , pstruct->array_size );
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCNETORDER( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pstruct->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=(short)htons((unsigned short)(%s%s));\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=(int)htonl((unsigned int)(%s%s));\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__HTONLL(%s%s);\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=htons(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=htonl(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__HTONLL(%s%s);\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__HTONF(%s%s);\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__HTONLF(%s%s);\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__HTONLF(%s%s);\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__HTONLF(%s%s);\n" , pathname,pfield->field_name );
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCNETORDER( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCHOSTORDER( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pstruct->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=(short)ntohs((unsigned short)(%s%s));\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=(int)ntohl((unsigned int)(%s%s));\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__NTOHLL(%s%s);\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=ntohs(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s=ntohl(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__NTOHLL(%s%s);\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__NTOHF(%s%s);\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__NTOHLF(%s%s);\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__NTOHLF(%s%s);\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	__NTOHLF(%s%s);\n" , pathname,pfield->field_name );
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCHOSTORDER( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

#define DUP_CHECK	\
			"if(len<0||remain_len<=len)" \
			"{" \
				"char *tmp=NULL;" \
				"int buf_offset=buf-(*pp_base);" \
				"int new_buf_size;" \
				"if(buf_size<1024*1024*1024)" \
					"new_buf_size=buf_size*2;" \
				"else " \
					"new_buf_size=buf_size+10*1024*1024;" \
				"tmp=(char*)realloc(*pp_base,new_buf_size);" \
				"if(tmp==NULL)" \
					"return -2;" \
				"else " \
					"(*pp_base)=tmp,buf=(*pp_base)+buf_offset,remain_len+=new_buf_size-buf_size,buf_size=new_buf_size,memset(buf,0x00,remain_len+1);" \
			"}" \
			"else " \
			"{" \
				"break;" \
			"}" \

static int GenerateCCode_c_DSCSERIALIZE_COMPACT( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pstruct->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ short s; s=(short)htons((unsigned short)(%s%s)); memcpy( ptr , (char*)&s , 2 ); }\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ int i; i=(int)htonl((unsigned int)(%s%s)); memcpy( ptr , (char*)&i , 4 ); }\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ longlong ll=%s%s; __HTONLL(ll); memcpy( ptr , (char*)&ll , 8 ); }\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = (char)%s%s ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ short s; s=htons(%s%s); memcpy( ptr , (char*)&s , 2 ); }\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ int i; i=htonl(%s%s); memcpy( ptr , (char*)&i , 4 ); }\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ longlong ll=%s%s; __HTONLL(ll); memcpy( ptr , (char*)&ll , 8 ); }\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ float f=%s%s; __HTONF(f); memcpy( ptr , (char*)&f , 4 ); }\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ double d=%s%s; __HTONLF(d); memcpy( ptr , (char*)&d , 8 ); }\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ double d=%s%s; __HTONLF(d); memcpy( ptr , (char*)&d , 8 ); }\n" , pathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ double d=%s%s; __HTONLF(d); memcpy( ptr , (char*)&d , 8 ); }\n" , pathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = (char)%s%s ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( ptr , %s%s , %d );\n" , pathname,pfield->field_name , pfield->field_len );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=%d; ptr+=%d;\n" , pfield->field_len , pfield->field_len );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_COMPACT( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_COMPACT( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pstruct->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (*ptr) ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 2 ); %s%s=(short)htons((unsigned short)(%s%s));\n" , pathname,pfield->field_name , pathname,pfield->field_name , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 4 ); %s%s=(int)htonl((unsigned int)(%s%s));\n" , pathname,pfield->field_name , pathname,pfield->field_name , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); __HTONLL(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (unsigned char)(*ptr) ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 2 ); %s%s=htons(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 4 ); %s%s=htonl(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); __HTONLL(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 4 ); __NTOHF(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); __NTOHLF(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); __NTOHLF(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); __NTOHLF(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (char)(*ptr) ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (unsigned char)(*ptr) ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( %s%s , ptr , %d );\n" , pathname,pfield->field_name , pfield->field_len );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=%d; ptr+=%d;\n" , pfield->field_len , pfield->field_len );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (char)(*ptr) ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_COMPACT( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_COMPRESS( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pstruct->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ short s; s=(short)htons((unsigned short)(%s%s)); memcpy( ptr , (char*)&s , 2 ); }\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	COMPRESS_INT4( %s%s , ptr , len )\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	COMPRESS_INT8( %s%s , ptr , len )\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = (char)%s%s ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ short s; s=htons(%s%s); memcpy( ptr , (char*)&s , 2 ); }\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	COMPRESS_UINT4( %s%s , ptr , len )\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	COMPRESS_UINT8( %s%s , ptr , len )\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ float f=%s%s; __HTONF(f); memcpy( ptr , (char*)&f , 4 ); }\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ double d=%s%s; __HTONLF(d); memcpy( ptr , (char*)&d , 8 ); }\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ double d=%s%s; __HTONLF(d); memcpy( ptr , (char*)&d , 8 ); }\n" , pathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{ double d=%s%s; __HTONLF(d); memcpy( ptr , (char*)&d , 8 ); }\n" , pathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = (char)%s%s ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	unsigned int size=strlen(%s%s);\n" , pathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	COMPRESS_UINT4( size , ptr , len )\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( ptr , %s%s , size );\n" , pathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=size; ptr+=size;\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	(*ptr) = %s%s ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_COMPRESS( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_COMPRESS( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pstruct->array_size , depth );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (*ptr) ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 2 ); %s%s=(short)htons((unsigned short)(%s%s));\n" , pathname,pfield->field_name , pathname,pfield->field_name , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	UNCOMPRESS_INT4( ptr , len , %s%s )\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	UNCOMPRESS_INT8( ptr , len , %s%s )\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (unsigned char)(*ptr) ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 2 ); %s%s=htons(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=2; ptr+=2;\n" );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	UNCOMPRESS_UINT4( ptr , len , %s%s )\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	UNCOMPRESS_UINT8( ptr , len , %s%s )\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 4 ); __NTOHF(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=4; ptr+=4;\n" );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); __NTOHLF(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); __NTOHLF(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( (char*)&(%s%s) , ptr , 8 ); __NTOHLF(%s%s);\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=8; ptr+=8;\n" );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (char)(*ptr) ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (unsigned char)(*ptr) ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	unsigned int size ;\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	UNCOMPRESS_UINT4( ptr , len , size )\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	memcpy( %s%s , ptr , size );\n" , pathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len+=size; ptr+=size;\n" );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	%s%s = (char)(*ptr) ;\n" , pathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	len++; ptr++;\n" );
				}
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_COMPRESS( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	}\n" );
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_XML( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	char			tabs[ DSC_MAXDEPTH ] ;
	int			nret = 0 ;
	
	memset( tabs , '\t' , DSC_MAXDEPTH );
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d]<%s_%s_count ; index[%d]++ )\n" , depth , depth , up_pathname , pstruct->struct_name , depth );
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth,tabs , pstruct->struct_name );
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%d\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%hd\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%d\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC,%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%u\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%hu\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%u\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC,%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%f\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%lf\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%.%dlf\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_len2 , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%.2lf\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%c\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%c\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
				fprintf( fp_dsc_c , "\tXMLESCAPE_EXPAND(%s%s,strlen(%s%s),buf,len,remain_len); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\tif(%s%s==DSCTRUE)len=SNPRINTF(buf,remain_len,\"true\");else len=SNPRINTF(buf,remain_len,\"false\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_XML( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s</%s>\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth,tabs , pstruct->struct_name );
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	}\n" );
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_XML_COMPACT( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d]<%s_%s_count ; index[%d]++ )\n" , depth , depth , up_pathname , pstruct->struct_name , depth );
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pstruct->struct_name );
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%d\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%hd\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%d\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC,%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%u\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%hu\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%u\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC,%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%f\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%lf\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%.%dlf\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_len2 , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%.2lf\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%c\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%c\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tXMLESCAPE_EXPAND(%s%s,strlen(%s%s),buf,len,remain_len); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"<%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tif(%s%s==DSCTRUE)len=SNPRINTF(buf,remain_len,\"true\");else len=SNPRINTF(buf,remain_len,\"false\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_XML_COMPACT( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"</%s>\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pstruct->struct_name );
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "}\n" );
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_XML_DUP( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	char			tabs[ DSC_MAXDEPTH ] ;
	int			nret = 0 ;
	
	memset( tabs , '\t' , DSC_MAXDEPTH );
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d]<%s_%s_count ; index[%d]++ )\n" , depth , depth , up_pathname , pstruct->struct_name , depth );
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth,tabs , pstruct->struct_name );
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%d\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%hd\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%d\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC,%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%u\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%hu\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%u\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC,%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%f\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%lf\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%.%dlf\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_len2 , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%.2lf\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%c\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%c\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){XMLESCAPE_EXPAND(%s%s,strlen(%s%s),buf,len,remain_len);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){if(%s%s==DSCTRUE)len=SNPRINTF(buf,remain_len,\"true\");else len=SNPRINTF(buf,remain_len,\"false\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_XML_DUP( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s</%s>\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth,tabs , pstruct->struct_name );
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	}\n" );
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_XML_COMPACT_DUP( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d]<%s_%s_count ; index[%d]++ )\n" , depth , depth , up_pathname , pstruct->struct_name , depth );
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pstruct->struct_name );
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%d\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%hd\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%d\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC,%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%u\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%hu\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%u\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC,%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%f\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%lf\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%.%dlf\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_len2 , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%.2lf\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%c\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%c\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){XMLESCAPE_EXPAND(%s%s,strlen(%s%s),buf,len,remain_len);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"<%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){if(%s%s==DSCTRUE)len=SNPRINTF(buf,remain_len,\"true\");else len=SNPRINTF(buf,remain_len,\"false\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				}
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_XML_COMPACT_DUP( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"</%s>\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pstruct->struct_name );
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "}\n" );
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_XML_ENTERNODE( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname , char *up_xmlpathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	char			xmlpathname[ 1024 + 1 ] ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n", (int)(strlen(up_xmlpathname)+1+strlen(pstruct->struct_name)) , up_xmlpathname,pstruct->struct_name );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{if(%s_%s_count>%d) return -8;}\n" , up_pathname,pstruct->struct_name , pstruct->array_size );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
			strcpy( xmlpathname  , up_xmlpathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[%s_%s_count]." , up_pathname , pstruct->struct_name , up_pathname,pstruct->struct_name );
			}
			SNPRINTF( xmlpathname , sizeof(xmlpathname)-1 , "%s/%s" , up_xmlpathname , pstruct->struct_name );
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_XML_ENTERNODE( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname , xmlpathname ) ;
			if( nret )
				return nret;
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_XML_LEAVENODE( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname , char *up_xmlpathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	char			xmlpathname[ 1024 + 1 ] ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(up_xmlpathname)+1+strlen(pstruct->struct_name)) , up_xmlpathname,pstruct->struct_name );
			fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{%s_%s_count++;}\n" , up_pathname,pstruct->struct_name );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
			strcpy( xmlpathname  , up_xmlpathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[%s_%s_count]." , up_pathname , pstruct->struct_name , up_pathname,pstruct->struct_name );
			}
			SNPRINTF( xmlpathname , sizeof(xmlpathname)-1 , "%s/%s" , up_xmlpathname , pstruct->struct_name );
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_XML_LEAVENODE( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname , xmlpathname ) ;
			if( nret )
				return nret;
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_XML_LEAF( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname , char *up_xmlpathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	char			xmlpathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
			strcpy( xmlpathname  , up_xmlpathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[%s_%s_count]." , up_pathname , pstruct->struct_name , up_pathname,pstruct->struct_name );
			}
			SNPRINTF( xmlpathname , sizeof(xmlpathname)-1 , "%s/%s" , up_xmlpathname , pstruct->struct_name );
		}
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOI1(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOS(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOI(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLL(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOI1(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOS(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOI(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLL(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOF(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLF(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLF(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLF(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOC(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOC(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{XMLUNESCAPE_FOLD(content,content_len,%s%s,len,sizeof(%s%s));\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{if( len < 0 ) return -7;}}\n" );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( xpath_len == %d && strncmp( xpath , \"%s/%s\" , xpath_len ) == 0 )\n" , (int)(strlen(xmlpathname)+1+strlen(pfield->field_name)) , xmlpathname,pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOB(content,content_len,%s%s);}\n" , pathname,pfield->field_name );
				}
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_XML_LEAF( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname , xmlpathname ) ;
			if( nret )
				return nret;
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_JSON( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	char			tabs[ DSC_MAXDEPTH ] ;
	int			nret = 0 ;
	
	memset( tabs , '\t' , DSC_MAXDEPTH );
	
	if( depth == 0 )
	{
		fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s{\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth,tabs );
	}
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( depth > 0 )
		{
			fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth,tabs , pstruct->struct_name );
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s[\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth,tabs );
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d]<%s_%s_count ; index[%d]++ )\n" , depth , depth , up_pathname , pstruct->struct_name , depth );
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth > 0 )
		{
			if( pstruct->first_field && pstruct->first_field->field_name[0] == '_' )
				;
			else
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s{\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth,tabs );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%d\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%hd\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%d\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC\"\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%u\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%hu\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%u\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC\"\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%f\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%lf\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%.%dlf\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_len2 , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%.2lf\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%c\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%c\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tif(%s%s[0]==127)\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\t{\n" );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"null\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
				fprintf( fp_dsc_c , "\t}\n" );
				fprintf( fp_dsc_c , "\telse\n" );
				fprintf( fp_dsc_c , "\t{\n" );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
				fprintf( fp_dsc_c , "\tJSONESCAPE_EXPAND(%s%s,strlen(%s%s),buf,len,remain_len); if(len<0)return -7; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
				fprintf( fp_dsc_c , "\t}\n" );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tif(%s%s==DSCTRUE)len=SNPRINTF(buf,remain_len,\"true\");else len=SNPRINTF(buf,remain_len,\"false\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			
			if( pfield->next_field || pstruct->first_sub_struct )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\" ,\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
			}
			else
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_JSON( pcp , depth+1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pstruct->array_size > 0 )
		{
			if( pstruct->first_field && pstruct->first_field->field_name[0] == '_' )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\tif((%s_%s_count==0)?(index[%d]<%d-1):(index[%d]<%s_%s_count-1))\n" , up_pathname , pstruct->struct_name , depth , pstruct->array_size , depth , up_pathname , pstruct->struct_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ len=SNPRINTF(buf,remain_len,\"%.*s ,\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len; }\n" , depth,tabs );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\telse\n" );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ len=SNPRINTF(buf,remain_len,\"%.*s\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len; }\n" , depth,tabs );
			}
			else
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\tif((%s_%s_count==0)?(index[%d]<%d-1):(index[%d]<%s_%s_count-1))\n" , up_pathname , pstruct->struct_name , depth , pstruct->array_size , depth , up_pathname , pstruct->struct_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ len=SNPRINTF(buf,remain_len,\"%.*s} ,\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len; }\n" , depth,tabs );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\telse\n" );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ len=SNPRINTF(buf,remain_len,\"%.*s}\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len; }\n" , depth,tabs );
			}
		}
		else
		{
			if( pstruct->next_struct )
			{
				if( pstruct->first_field && pstruct->first_field->field_name[0] == '_' )
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s ,\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth,tabs );
				else
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s} ,\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth,tabs );
			}
			else
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s}\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth,tabs );
			}
		}
		
		if( pstruct->array_size > 0 )
		{
			if( depth > 0 )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	}\n" );
			}
			
			if( pstruct->next_struct )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s] ,\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth,tabs );
			}
			else
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%.*s]\\n\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , depth,tabs );
			}
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_JSON_COMPACT( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	int			nret = 0 ;
	
	if( depth == 0 )
	{
		fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"{\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
	}
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( depth > 0 )
		{
			fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pstruct->struct_name );
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"[\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d]<%s_%s_count ; index[%d]++ )\n" , depth , depth , up_pathname , pstruct->struct_name , depth );
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth > 0 )
		{
			if( pstruct->first_field && pstruct->first_field->field_name[0] == '_' )
				;
			else
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"{\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%d\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%hd\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%d\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC\"\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%u\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%hu\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%u\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC\"\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%f\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%lf\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%.%dlf\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_len2 , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%.2lf\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%c\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"%%c\",%s%s); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tif(%s%s[0]==127)\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\t{\n" );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"null\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
				fprintf( fp_dsc_c , "\t}\n" );
				fprintf( fp_dsc_c , "\telse\n" );
				fprintf( fp_dsc_c , "\t{\n" );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
				fprintf( fp_dsc_c , "\tJSONESCAPE_EXPAND(%s%s,strlen(%s%s),buf,len,remain_len); if(len<0)return -7; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
				fprintf( fp_dsc_c , "\t}\n" );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\tif(%s%s==DSCTRUE)len=SNPRINTF(buf,remain_len,\"true\");else len=SNPRINTF(buf,remain_len,\"false\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			
			if( pfield->next_field || pstruct->first_sub_struct )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\",\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
			}
			else
			{
				/* fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" ); */
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_JSON_COMPACT( pcp , depth+1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pstruct->array_size > 0 )
		{
			if( pstruct->first_field && pstruct->first_field->field_name[0] == '_' )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\tif((%s_%s_count==0)?(index[%d]<%d-1):(index[%d]<%s_%s_count-1))\n" , up_pathname , pstruct->struct_name , depth , pstruct->array_size , depth , up_pathname , pstruct->struct_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ len=SNPRINTF(buf,remain_len,\",\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len; }\n" );
			}
			else
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\tif((%s_%s_count==0)?(index[%d]<%d-1):(index[%d]<%s_%s_count-1))\n" , up_pathname , pstruct->struct_name , depth , pstruct->array_size , depth , up_pathname , pstruct->struct_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ len=SNPRINTF(buf,remain_len,\"},\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len; }\n" );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\telse\n" );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ len=SNPRINTF(buf,remain_len,\"}\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len; }\n" );
			}
		}
		else
		{
			if( pstruct->next_struct )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"},\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
			}
			else
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"}\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
			}
		}
		
		if( pstruct->array_size > 0 )
		{
			if( depth > 0 )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "}\n" );
			}
			
			if( pstruct->next_struct )
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"],\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
			}
			else
			{
				fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"]\"); if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
			}
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_JSON_DUP( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	char			tabs[ DSC_MAXDEPTH ] ;
	int			nret = 0 ;
	
	memset( tabs , '\t' , DSC_MAXDEPTH );
	
	if( depth == 0 )
	{
		fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s{\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth,tabs );
	}
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( depth > 0 )
		{
			fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\\\"%s\\\" : \\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth,tabs , pstruct->struct_name );
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s[\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth,tabs );
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d]<%s_%s_count ; index[%d]++ )\n" , depth , depth , up_pathname , pstruct->struct_name , depth );
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth > 0 )
		{
			if( pstruct->first_field && pstruct->first_field->field_name[0] == '_' )
				;
			else
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s{\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth,tabs );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%d\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%hd\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%d\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC\"\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%u\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%hu\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%u\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC\"\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%f\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%lf\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%.%dlf\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_len2 , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%.2lf\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%c\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%c\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tif(%s%s[0]==127)\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\t{\n" );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"null\");"DUP_CHECK"} if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
				fprintf( fp_dsc_c , "\t}\n" );
				fprintf( fp_dsc_c , "\telse\n" );
				fprintf( fp_dsc_c , "\t{\n" );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
				fprintf( fp_dsc_c , "\twhile(1){JSONESCAPE_EXPAND(%s%s,strlen(%s%s),buf,len,remain_len);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
				fprintf( fp_dsc_c , "\t}\n" );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth+1,tabs );
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\" : \");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){if(%s%s==DSCTRUE)len=SNPRINTF(buf,remain_len,\"true\");else len=SNPRINTF(buf,remain_len,\"false\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			
			if( pfield->next_field || pstruct->first_sub_struct )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\" ,\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
			}
			else
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_JSON_DUP( pcp , depth+1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pstruct->array_size > 0 )
		{
			if( pstruct->first_field && pstruct->first_field->field_name[0] == '_' )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\tif((%s_%s_count==0)?(index[%d]<%d-1):(index[%d]<%s_%s_count-1))\n" , up_pathname , pstruct->struct_name , depth , pstruct->array_size , depth , up_pathname , pstruct->struct_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ while(1){len=SNPRINTF(buf,remain_len,\"%.*s ,\\n\");"DUP_CHECK"} buf+=len; remain_len-=len; }\n" , depth,tabs );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\telse\n" );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ while(1){len=SNPRINTF(buf,remain_len,\"%.*s\\n\");"DUP_CHECK"} buf+=len; remain_len-=len; }\n" , depth,tabs );
			}
			else
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\tif((%s_%s_count==0)?(index[%d]<%d-1):(index[%d]<%s_%s_count-1))\n" , up_pathname , pstruct->struct_name , depth , pstruct->array_size , depth , up_pathname , pstruct->struct_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ while(1){len=SNPRINTF(buf,remain_len,\"%.*s} ,\\n\");"DUP_CHECK"} buf+=len; remain_len-=len; }\n" , depth,tabs );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\telse\n" );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ while(1){len=SNPRINTF(buf,remain_len,\"%.*s}\\n\");"DUP_CHECK"} buf+=len; remain_len-=len; }\n" , depth,tabs );
			}
		}
		else
		{
			if( pstruct->next_struct )
			{
				if( pstruct->first_field && pstruct->first_field->field_name[0] == '_' )
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s ,\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth,tabs );
				else
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s} ,\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth,tabs );
			}
			else
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s}\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth,tabs );
			}
		}
		
		if( pstruct->array_size > 0 )
		{
			if( depth > 0 )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	}\n" );
			}
			
			if( pstruct->next_struct )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s] ,\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth,tabs );
			}
			else
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%.*s]\\n\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , depth,tabs );
			}
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCSERIALIZE_JSON_COMPACT_DUP( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	int			nret = 0 ;
	
	if( depth == 0 )
	{
		fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"{\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
	}
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( depth > 0 )
		{
			fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pstruct->struct_name );
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"[\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	for( index[%d] = 0 ; index[%d]<%s_%s_count ; index[%d]++ )\n" , depth , depth , up_pathname , pstruct->struct_name , depth );
			fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{\n" );
		}
		
		if( depth > 0 )
		{
			if( pstruct->first_field && pstruct->first_field->field_name[0] == '_' )
				;
			else
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"{\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%d\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%hd\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%d\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC\"\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%u\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%hu\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%u\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,LONGLONG_FORMAT_SPEC\"\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%f\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%lf\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%.%dlf\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_len2 , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%.2lf\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%c\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				if( pfield->field_name[0] != '_' )
					fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"%%c\",%s%s);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
				fprintf( fp_dsc_c , "\tif(%s%s[0]==127)\n" , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\t{\n" );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"null\");"DUP_CHECK"} if(len<0||remain_len<len)return -1; buf+=len; remain_len-=len;\n" );
				fprintf( fp_dsc_c , "\t}\n" );
				fprintf( fp_dsc_c , "\telse\n" );
				fprintf( fp_dsc_c , "\t{\n" );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
				fprintf( fp_dsc_c , "\twhile(1){JSONESCAPE_EXPAND(%s%s,strlen(%s%s),buf,len,remain_len);"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name , pathname,pfield->field_name );
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
				fprintf( fp_dsc_c , "\t}\n" );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					if( pfield->field_name[0] != '_' )
						fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"\\\"%s\\\":\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pfield->field_name );
					fprintf( fp_dsc_c , "\twhile(1){if(%s%s==DSCTRUE)len=SNPRINTF(buf,remain_len,\"true\");else len=SNPRINTF(buf,remain_len,\"false\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" , pathname,pfield->field_name );
				}
			}
			
			if( pfield->next_field || pstruct->first_sub_struct )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\",\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
			}
			else
			{
				/* fprintf( fp_dsc_c , "\tlen=SNPRINTF(buf,remain_len,\"\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" ); */
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCSERIALIZE_JSON_COMPACT_DUP( pcp , depth+1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pstruct->array_size > 0 )
		{
			if( pstruct->first_field && pstruct->first_field->field_name[0] == '_' )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\tif((%s_%s_count==0)?(index[%d]<%d-1):(index[%d]<%s_%s_count-1))\n" , up_pathname , pstruct->struct_name , depth , pstruct->array_size , depth , up_pathname , pstruct->struct_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ while(1){len=SNPRINTF(buf,remain_len,\",\");"DUP_CHECK"} buf+=len; remain_len-=len; }\n" );
			}
			else
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\tif((%s_%s_count==0)?(index[%d]<%d-1):(index[%d]<%s_%s_count-1))\n" , up_pathname , pstruct->struct_name , depth , pstruct->array_size , depth , up_pathname , pstruct->struct_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ while(1){len=SNPRINTF(buf,remain_len,\"},\");"DUP_CHECK"} buf+=len; remain_len-=len; }\n" );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\telse\n" );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "\t{ while(1){len=SNPRINTF(buf,remain_len,\"}\");"DUP_CHECK"} buf+=len; remain_len-=len; }\n" );
			}
		}
		else
		{
			if( pstruct->next_struct )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"},\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
			}
			else
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"}\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
			}
		}
		
		if( pstruct->array_size > 0 )
		{
			if( depth > 0 )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "}\n" );
			}
			
			if( pstruct->next_struct )
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"],\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
			}
			else
			{
				fprintf( fp_dsc_c , "\twhile(1){len=SNPRINTF(buf,remain_len,\"]\");"DUP_CHECK"} buf+=len; remain_len-=len;\n" );
			}
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_JSON_ENTERNODE( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname , char *up_jsonpathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	char			jsonpathname[ 1024 + 1 ] ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			if( pstruct->first_field && pstruct->first_field->field_name[0] == '_' )
			{
				;
			}
			else
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(up_jsonpathname)+1+strlen(pstruct->struct_name)) , up_jsonpathname,pstruct->struct_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{if(%s_%s_count>=%d)return -8;}\n" , up_pathname,pstruct->struct_name , pstruct->array_size );
			}
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
			strcpy( jsonpathname  , up_jsonpathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[%s_%s_count]." , up_pathname , pstruct->struct_name , up_pathname,pstruct->struct_name );
			}
			SNPRINTF( jsonpathname , sizeof(jsonpathname)-1 , "%s/%s" , up_jsonpathname , pstruct->struct_name );
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_JSON_ENTERNODE( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname , jsonpathname ) ;
			if( nret )
				return nret;
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_JSON_LEAVENODE( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname , char *up_jsonpathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	char			jsonpathname[ 1024 + 1 ] ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			if( pstruct->first_field && pstruct->first_field->field_name[0] == '_' )
			{
				;
			}
			else
			{
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(up_jsonpathname)+1+strlen(pstruct->struct_name)) , up_jsonpathname,pstruct->struct_name );
				fprintabs( fp_dsc_c , depth ); fprintf( fp_dsc_c , "	{%s_%s_count++;}\n" , up_pathname,pstruct->struct_name );
			}
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
			strcpy( jsonpathname  , up_jsonpathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[%s_%s_count]." , up_pathname , pstruct->struct_name , up_pathname,pstruct->struct_name );
			}
			SNPRINTF( jsonpathname , sizeof(jsonpathname)-1 , "%s/%s" , up_jsonpathname , pstruct->struct_name );
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_JSON_LEAVENODE( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname , jsonpathname ) ;
			if( nret )
				return nret;
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_c_DSCDESERIALIZE_JSON_LEAF( struct CommandParameter *pcp , int depth , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_c , char *up_pathname , char *up_jsonpathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			pathname[ 1024 + 1 ] ;
	char			jsonpathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	char			converted_field_name[ 64 + 1 ] ;
	char			converted_jsonpathname[ 1024 + 1 ] ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
			strcpy( jsonpathname  , up_jsonpathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[%s_%s_count]." , up_pathname , pstruct->struct_name , up_pathname,pstruct->struct_name );
			}
			SNPRINTF( jsonpathname , sizeof(jsonpathname)-1 , "%s/%s" , up_jsonpathname , pstruct->struct_name );
		}
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( pfield->field_name[0] == '_' )
			{
				strcpy( converted_field_name , "." );
				memset( converted_jsonpathname , 0x00 , sizeof(converted_jsonpathname) );
				SNPRINTF( converted_jsonpathname , sizeof(converted_jsonpathname)-1 , "%s_%s_count++;" , up_pathname,pstruct->struct_name );
			}
			else
			{
				strcpy( converted_field_name , pfield->field_name );
				memset( converted_jsonpathname , 0x00 , sizeof(converted_jsonpathname) );
			}
			
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOI1(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOS(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOI(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLL(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOI1(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOS(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOI(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLL(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOF(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLF(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLF(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOLF(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOC(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOC(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
				}
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
				fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{JSONUNESCAPE_FOLD(content,content_len,%s%s,len,sizeof(%s%s)-1); if(len<0){_DSC_errline=__LINE__;return -7;}%s}\n" , pathname,pfield->field_name , pathname,pfield->field_name , converted_jsonpathname );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	/* %s */\n" , pfield->field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	if( jpath_len == %d && strncmp( jpath , \"%s/%s\" , jpath_len ) == 0 )\n" , (int)(strlen(jsonpathname)+1+strlen(converted_field_name)) , jsonpathname,converted_field_name );
					fprintabs( fp_dsc_c , depth+1 ); fprintf( fp_dsc_c , "	{NATOB(content,content_len,%s%s);%s}\n" , pathname,pfield->field_name , converted_jsonpathname );
				}
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_c_DSCDESERIALIZE_JSON_LEAF( pcp , depth + 1 , pstruct->first_sub_struct , 0 , fp_dsc_c , pathname , jsonpathname ) ;
			if( nret )
				return nret;
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

static int GenerateCCode_LOG_c( struct CommandParameter *pcp , int depth , struct StructInfo *grandfather_pmsginfo , struct StructInfo *first_struct , int only_once , FILE *fp_dsc_LOG_c , char *LOG_up_pathname , char *up_pathname )
{
	struct StructInfo	*pstruct = NULL ;
	char			LOG_pathname[ 1024 + 1 ] ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfield = NULL ;
	
	int			nret = 0 ;
	
	for( pstruct = first_struct ; pstruct ; pstruct = pstruct->next_struct )
	{
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s_%s_count[%%d]\" NEWLINE_DSCLOG_%s , %s_%s_count );\n" , grandfather_pmsginfo->struct_name , LOG_up_pathname,pstruct->struct_name , grandfather_pmsginfo->struct_name , up_pathname,pstruct->struct_name );
			fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	for( index[%d] = 0 ; index[%d] < %s_%s_count ; index[%d]++ )\n" , depth , depth , up_pathname,pstruct->struct_name , depth );
			fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( LOG_pathname , LOG_up_pathname );
			strcpy( pathname , up_pathname );
		}
		else
		{
			if( pstruct->array_size == 0 )
			{
				SNPRINTF( LOG_pathname , sizeof(LOG_pathname)-1 , "%s%s." , LOG_up_pathname , pstruct->struct_name );
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pstruct->struct_name );
			}
			else
			{
				SNPRINTF( LOG_pathname , sizeof(LOG_pathname)-1 , "%s%s[index[%d]]." , LOG_up_pathname , pstruct->struct_name , depth );
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pstruct->struct_name , depth );
			}
		}
		
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			if( STRCMP( pfield->field_type , == , "INT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%d]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%hd]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%d]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[\" LONGLONG_FORMAT_SPEC \"d]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "UINT" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%u]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 2 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%hu]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%u]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[\" LONGLONG_FORMAT_SPEC \"u]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
			{
				if( pfield->field_len == 4 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%f]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
				}
				else if( pfield->field_len == 8 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%lf]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
				}
			}
			else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
			{
				fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%.%dlf]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , pfield->field_len2 , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
			{
				fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%.2lf]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "CHAR" ) )
			{
				fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%c]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "UCHAR" ) )
			{
				fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%c]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "STRING" ) )
			{
				fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%s]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
			}
			else if( STRCMP( pfield->field_type , == , "BOOL" ) )
			{
				if( pfield->field_len == 1 )
				{
					fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	PREFIX_DSCLOG_%s \"%s%s[%%c]\" NEWLINE_DSCLOG_%s , %s%s );\n" , grandfather_pmsginfo->struct_name , LOG_pathname,pfield->field_name , grandfather_pmsginfo->struct_name , pathname,pfield->field_name );
				}
			}
		}
		
		if( pstruct->first_sub_struct )
		{
			nret = GenerateCCode_LOG_c( pcp , depth + 1 , grandfather_pmsginfo , pstruct->first_sub_struct , 0 , fp_dsc_LOG_c , LOG_pathname , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pstruct->array_size > 0 )
		{
			fprintabs( fp_dsc_LOG_c , depth ); fprintf( fp_dsc_LOG_c , "	}\n" );
		}
		
		if( only_once )
			break;
	}
	
	return 0;
}

int GenerateCCode( struct CommandParameter *pcp , struct StructInfo *pstruct , FILE *fp_dsc_h , FILE *fp_dsc_c , FILE *fp_dsc_LOG_c )
{
	char			LOG_pathname[ 64 + 1 ] ;
	
	struct StructInfo	*next_struct = NULL ;
	
	int			nret = 0 ;
	
	/* Generate *.dsc.h */
	fprintf( fp_dsc_h , "/* It had generated by DirectStruct v%s */\n" , __DIRECTSTRUCT_VERSION );
	fprintf( fp_dsc_h , "#ifndef _H_%s_\n" , pstruct->next_struct->struct_name );
	fprintf( fp_dsc_h , "#define _H_%s_\n" , pstruct->next_struct->struct_name );
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
	fprintf( fp_dsc_h , "#ifndef DSCTRUE\n" );
	fprintf( fp_dsc_h , "#define DSCTRUE	(unsigned char)'t'\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#ifndef DSCFALSE\n" );
	fprintf( fp_dsc_h , "#define DSCFALSE	(unsigned char)'f'\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#ifndef DSCNULL\n" );
	fprintf( fp_dsc_h , "#define DSCNULL	(unsigned char)127\n" );
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
	fprintf( fp_dsc_h , "#ifndef LL\n" );
	fprintf( fp_dsc_h , "#define LL(_ll_)	_ll_##LL\n" );
	fprintf( fp_dsc_h , "#define ULL(_ll_)	_ll_##ULL\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#define LONGLONG_FORMAT_SPEC	\"%%lld\"\n" );
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
	fprintf( fp_dsc_h , "#ifndef LL\n" );
	fprintf( fp_dsc_h , "#define LL(_ll_)	_ll_\n" );
	fprintf( fp_dsc_h , "#define ULL(_ll_)	_ll_\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#define LONGLONG_FORMAT_SPEC	\"%%I64\"\n" );
	fprintf( fp_dsc_h , "#define ATOLL	_atoi64\n" );
	fprintf( fp_dsc_h , "#define __BIG_ENDIAN	4321\n" );
	fprintf( fp_dsc_h , "#define __LITTLE_ENDIAN	1234\n" );
	fprintf( fp_dsc_h , "#define __BYTE_ORDER	__LITTLE_ENDIAN\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#ifndef NATOC\n" );
	fprintf( fp_dsc_h , "#define NATOC(_base_,_len_,_result_)	{_result_=_base_[0];}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	fprintf( fp_dsc_h , "#ifndef NATOI1\n" );
	fprintf( fp_dsc_h , "#define NATOI1(_base_,_len_,_result_)	{char buf[3+1];memset(buf,0x00,sizeof(buf));strncpy(buf,_base_,3);_result_=(char)atoi(buf);}\n" );
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
	fprintf( fp_dsc_h , "#ifndef NATOB\n" );
	fprintf( fp_dsc_h , "#define NATOB(_base_,_len_,_result_)	{if(memcmp(_base_,\"true\",4)==0)_result_=DSCTRUE;else _result_=DSCFALSE;}\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	
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
	}
	
	for( next_struct = pstruct->next_struct ; next_struct ; next_struct = next_struct->next_struct )
	{
		nret = GenerateCCode_h( pcp , 0 , next_struct , 1 , fp_dsc_h ) ;
		if( nret )
			return nret;
		
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCINIT_%s( %s *pst );\n" , next_struct->struct_name , next_struct->struct_name );
		
		if( pcp->output_c_order_flag == 1 )
		{
			fprintf( fp_dsc_h , "\n" );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCNETORDER_%s( %s *pst );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCHOSTORDER_%s( %s *pst );\n" , next_struct->struct_name , next_struct->struct_name );
		}
	
		if( pcp->output_c_compact_flag == 1 )
		{
			fprintf( fp_dsc_h , "\n" );
			fprintf( fp_dsc_h , "#define DSCSERIALIZE_COMPACT_BUFSIZE_%s	%d\n" , next_struct->struct_name , next_struct->struct_length );
			fprintf( fp_dsc_h , "\n" );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_COMPACT_%s( %s *pst , char *buf , int *p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_COMPACT_%s( char *buf , int *p_len , %s *pst );\n" , next_struct->struct_name , next_struct->struct_name );
		}
		
		if( pcp->output_c_compress_flag == 1 )
		{
			fprintf( fp_dsc_h , "\n" );
			fprintf( fp_dsc_h , "#define DSCSERIALIZE_COMPRESS_BUFSIZE_%s	%d\n" , next_struct->struct_name , next_struct->struct_length + next_struct->field_count );
			fprintf( fp_dsc_h , "\n" );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_COMPRESS_%s( %s *pst , char *buf , int *p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_COMPRESS_%s( char *buf , int *p_len , %s *pst );\n" , next_struct->struct_name , next_struct->struct_name );
		}
		
		if( pcp->output_c_xml_flag == 1 )
		{
			fprintf( fp_dsc_h , "\n" );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_XML_%s( %s *pst , char *encoding , char *buf , int *p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_XML_DUP_%s( %s *pst , char *encoding , char **pp_base , int *p_buf_size , int *p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_XML_DUP_%s_V( void *pst , char *encoding , char **pp_base , int *p_buf_size , int *p_len );\n" , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_XML_%s( char *encoding , char *buf , int *p_len , %s *pst );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_XML_%s_V( char *encoding , char *buf , int *p_len , void *pst );\n" , next_struct->struct_name );
		}	
		
		if( pcp->output_c_xml_compact_flag == 1 )
		{
			fprintf( fp_dsc_h , "\n" );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_XML_COMPACT_%s( %s *pst , char *encoding , char *buf , int *p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_XML_COMPACT_DUP_%s( %s *pst , char *encoding , char **pp_base , int *p_buf_size , int *p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_XML_COMPACT_DUP_%s_V( void *pst , char *encoding , char **pp_base , int *p_buf_size , int *p_len );\n" , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_XML_COMPACT_%s( char *encoding , char *buf , int *p_len , %s *pst );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_XML_COMPACT_%s_V( char *encoding , char *buf , int *p_len , void *pst );\n" , next_struct->struct_name );
		}	
		
		if( pcp->output_c_json_flag == 1 || pcp->output_c_json_compact_flag == 1 )
		{
			fprintf( fp_dsc_h , "\n" );
			fprintf( fp_dsc_h , "#include \"fasterjson.h\"\n" );
		}
		
		if( pcp->output_c_json_flag == 1 )
		{
			fprintf( fp_dsc_h , "\n" );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_JSON_%s( %s *pst , char *encoding , char *buf , int *p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_JSON_DUP_%s( %s *pst , char *encoding , char **pp_base , int *p_buf_size , int *p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_JSON_DUP_%s_V( void *pv , char *encoding , char **pp_base , int *p_buf_size , int *p_len );\n" , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_JSON_%s( char *encoding , char *buf , int *p_len , %s *pst );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_JSON_%s_V( char *encoding , char *buf , int *p_len , void *pv );\n" , next_struct->struct_name );
		}	
		
		if( pcp->output_c_json_compact_flag == 1 )
		{
			fprintf( fp_dsc_h , "\n" );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_JSON_COMPACT_%s( %s *pst , char *encoding , char *buf , int *p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_JSON_COMPACT_DUP_%s( %s *pst , char *encoding , char **pp_base , int *p_buf_size , int *p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCSERIALIZE_JSON_COMPACT_DUP_%s_V( void *pv , char *encoding , char **pp_base , int *p_buf_size , int *p_len );\n" , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_JSON_COMPACT_%s( char *encoding , char *buf , int *p_len , %s *pst );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_h , "_WINDLL_FUNC int DSCDESERIALIZE_JSON_COMPACT_%s_V( char *encoding , char *buf , int *p_len , void *pv );\n" , next_struct->struct_name );
		}	
		
		fprintf( fp_dsc_h , "\n" );
		fprintf( fp_dsc_h , "int DSCGetErrorLine_%s();\n" , next_struct->struct_name );
	}
	
	fprintf( fp_dsc_h , "\n" );
	fprintf( fp_dsc_h , "#endif\n" );
	
	/* Generate *.dsc.c */
	fprintf( fp_dsc_c , "/* It had generated by DirectStruct v%s */\n" , __DIRECTSTRUCT_VERSION );
	fprintf( fp_dsc_c , "#include \"%s.h\"\n" , pcp->pathfilename );
	fprintf( fp_dsc_c , "\n" );
	fprintf( fp_dsc_c , "static int		_DSC_errline = 0 ;\n" );
	
	for( next_struct = pstruct->next_struct ; next_struct ; next_struct = next_struct->next_struct )
	{
		/* Generate DSCINIT_*() */
		fprintf( fp_dsc_c , "\n" );
		fprintf( fp_dsc_c , "int DSCINIT_%s( %s *pst )\n" , next_struct->struct_name , next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
		fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
		fprintf( fp_dsc_c , "	memset( pst , 0x00 , sizeof(%s) );\n" , next_struct->struct_name );
		nret = GenerateCCode_c_DSCINIT( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
		if( nret )
			return nret;
		fprintf( fp_dsc_c , "	return 0;\n" );
		fprintf( fp_dsc_c , "}\n" );
		
		if( pcp->output_c_order_flag == 1 )
		{
			/* Generate DSCNETORDER_*() */
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCNETORDER_%s( %s *pst )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			nret = GenerateCCode_c_DSCNETORDER( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			
			/* Generate DSCHOSTORDER_*() */
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCHOSTORDER_%s( %s *pst )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			nret = GenerateCCode_c_DSCHOSTORDER( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
		}
		
		if( pcp->output_c_compact_flag == 1 )
		{
			/* Generate DSCSERIALIZE_COMPACT_*() */
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCSERIALIZE_COMPACT_%s( %s *pst , char *buf , int *p_len )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	char	*ptr = buf ;\n" );
			fprintf( fp_dsc_c , "	int	len = 0 ;\n" );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			nret = GenerateCCode_c_DSCSERIALIZE_COMPACT( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	if( p_len ) (*p_len) = len ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			
			/* Generate DSCDESERIALIZE_COMPACT_*() */
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCDESERIALIZE_COMPACT_%s( char *buf , int *p_len , %s *pst )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	char	*ptr = buf ;\n" );
			fprintf( fp_dsc_c , "	int	len = 0 ;\n" );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			nret = GenerateCCode_c_DSCDESERIALIZE_COMPACT( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
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
			fprintf( fp_dsc_c , "int DSCSERIALIZE_COMPRESS_%s( %s *pst , char *buf , int *p_len )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	char	*ptr = buf ;\n" );
			fprintf( fp_dsc_c , "	int	len = 0 ;\n" );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			nret = GenerateCCode_c_DSCSERIALIZE_COMPRESS( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	if( p_len ) (*p_len) = len ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			
			/* Generate DSCDESERIALIZE_COMPRESS_*() */
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCDESERIALIZE_COMPRESS_%s( char *buf , int *p_len , %s *pst )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	char	*ptr = buf ;\n" );
			fprintf( fp_dsc_c , "	int	len = 0 ;\n" );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			nret = GenerateCCode_c_DSCDESERIALIZE_COMPRESS( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	if( p_len ) (*p_len) = len ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
		}
		
		if( pcp->output_c_xml_flag == 1 || pcp->output_c_xml_compact_flag == 1 )
		{
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "#include \"fasterxml.h\"\n" );
		}
		
		if( pcp->output_c_xml_flag == 1 )
		{
			/* Generate DSCSERIALIZE_XML_*() */
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCSERIALIZE_XML_%s( %s *pst , char *encoding , char *buf , int *p_len )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	int	remain_len ;\n" );
			fprintf( fp_dsc_c , "	int	len ;\n" );
			fprintf( fp_dsc_c , "	char	tabs[%d+1] ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			fprintf( fp_dsc_c , "	remain_len = (*p_len) ;\n" );
			fprintf( fp_dsc_c , "	memset( tabs , '\\t' , %d );\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	len=SNPRINTF(buf,remain_len,\"<?xml version=\\\"1.0\\\" encoding=\\\"%%s\\\"?>\\n\",encoding); if(len<0)return -1; buf+=len; remain_len-=len;\n" );
			nret = GenerateCCode_c_DSCSERIALIZE_XML( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	(*p_len) = (*p_len) - remain_len ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCSERIALIZE_XML_DUP_%s( %s *pst , char *encoding , char **pp_base , int *p_buf_size , int *p_len )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	int	buf_size ;\n" );
			fprintf( fp_dsc_c , "	int	remain_len ;\n" );
			fprintf( fp_dsc_c , "	char	*buf = NULL ;\n" );
			fprintf( fp_dsc_c , "	int	buf_begin_offset ;\n" );
			fprintf( fp_dsc_c , "	int	len ;\n" );
			fprintf( fp_dsc_c , "	char	tabs[%d+1] ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			fprintf( fp_dsc_c , "	if( (*pp_base) == NULL )\n" );
			fprintf( fp_dsc_c , "	{\n" );
			fprintf( fp_dsc_c , "		char	*tmp = NULL ;\n" );
			fprintf( fp_dsc_c , "		if( p_buf_size && (*p_buf_size) > 0 )\n" );
			fprintf( fp_dsc_c , "		{\n" );
			fprintf( fp_dsc_c , "			buf_size = (*p_buf_size) ;\n" );
			fprintf( fp_dsc_c , "			tmp = (char*)malloc( buf_size ) ;\n" );
			fprintf( fp_dsc_c , "			if( tmp == NULL )\n" );
			fprintf( fp_dsc_c , "				return -2;\n" );
			fprintf( fp_dsc_c , "			memset( tmp , 0x00 , buf_size );\n" );
			fprintf( fp_dsc_c , "			(*pp_base) = tmp ;\n" );
			fprintf( fp_dsc_c , "			remain_len = buf_size - 1 ;\n" );
			fprintf( fp_dsc_c , "		}\n" );
			fprintf( fp_dsc_c , "		else\n" );
			fprintf( fp_dsc_c , "		{\n" );
			fprintf( fp_dsc_c , "			buf_size = 1024 ;\n" );
			fprintf( fp_dsc_c , "			tmp = (char*)malloc( buf_size ) ;\n" );
			fprintf( fp_dsc_c , "			if( tmp == NULL )\n" );
			fprintf( fp_dsc_c , "				return -2;\n" );
			fprintf( fp_dsc_c , "			memset( tmp , 0x00 , buf_size );\n" );
			fprintf( fp_dsc_c , "			(*pp_base) = tmp ;\n" );
			fprintf( fp_dsc_c , "			remain_len = buf_size - 1 ;\n" );
			fprintf( fp_dsc_c , "		}\n" );
			fprintf( fp_dsc_c , "	}\n" );
			fprintf( fp_dsc_c , "	else\n" );
			fprintf( fp_dsc_c , "	{\n" );
			fprintf( fp_dsc_c , "		buf_size = (*p_buf_size) ;\n" );
			fprintf( fp_dsc_c , "		remain_len = (*p_len) ;\n" );
			fprintf( fp_dsc_c , "	}\n" );
			fprintf( fp_dsc_c , "	buf_begin_offset = buf_size-1 - remain_len ;\n" );
			fprintf( fp_dsc_c , "	buf = (*pp_base) + buf_begin_offset ;\n" );
			fprintf( fp_dsc_c , "	memset( tabs , '\\t' , %d );\n" , DSC_MAXDEPTH );
			nret = GenerateCCode_c_DSCSERIALIZE_XML_DUP( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	if( p_buf_size )\n" );
			fprintf( fp_dsc_c , "		(*p_buf_size) = buf_size ;\n" );
			fprintf( fp_dsc_c , "	if( p_len )\n" );
			fprintf( fp_dsc_c , "		(*p_len) = buf_size-1 - buf_begin_offset - remain_len ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCSERIALIZE_XML_DUP_%s_V( void *pv , char *encoding , char **pp_base , int *p_buf_size , int *p_len )\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	return DSCSERIALIZE_XML_DUP_%s( (%s*)pv , encoding , pp_base , p_buf_size , p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "}\n" );
		}
		
		if( pcp->output_c_xml_compact_flag == 1 )
		{
			/* Generate DSCSERIALIZE_XML_*() */
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCSERIALIZE_XML_COMPACT_%s( %s *pst , char *encoding , char *buf , int *p_len )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	int	remain_len ;\n" );
			fprintf( fp_dsc_c , "	int	len ;\n" );
			fprintf( fp_dsc_c , "	char	tabs[%d+1] ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			fprintf( fp_dsc_c , "	remain_len = (*p_len) ;\n" );
			fprintf( fp_dsc_c , "	memset( tabs , '\\t' , %d );\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	len=SNPRINTF(buf,remain_len,\"<?xml version=\\\"1.0\\\" encoding=\\\"%%s\\\"?>\",encoding); if(len<0)return -1; buf+=len; remain_len-=len;\n" );
			nret = GenerateCCode_c_DSCSERIALIZE_XML_COMPACT( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	(*p_len) = (*p_len) - remain_len ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCSERIALIZE_XML_COMPACT_DUP_%s( %s *pst , char *encoding , char **pp_base , int *p_buf_size , int *p_len )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	int	buf_size ;\n" );
			fprintf( fp_dsc_c , "	int	remain_len ;\n" );
			fprintf( fp_dsc_c , "	char	*buf = NULL ;\n" );
			fprintf( fp_dsc_c , "	int	buf_begin_offset ;\n" );
			fprintf( fp_dsc_c , "	int	len ;\n" );
			fprintf( fp_dsc_c , "	char	tabs[%d+1] ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			fprintf( fp_dsc_c , "	if( (*pp_base) == NULL )\n" );
			fprintf( fp_dsc_c , "	{\n" );
			fprintf( fp_dsc_c , "		char	*tmp = NULL ;\n" );
			fprintf( fp_dsc_c , "		if( p_buf_size && (*p_buf_size) > 0 )\n" );
			fprintf( fp_dsc_c , "		{\n" );
			fprintf( fp_dsc_c , "			buf_size = (*p_buf_size) ;\n" );
			fprintf( fp_dsc_c , "			tmp = (char*)malloc( buf_size ) ;\n" );
			fprintf( fp_dsc_c , "			if( tmp == NULL )\n" );
			fprintf( fp_dsc_c , "				return -2;\n" );
			fprintf( fp_dsc_c , "			memset( tmp , 0x00 , buf_size );\n" );
			fprintf( fp_dsc_c , "			(*pp_base) = tmp ;\n" );
			fprintf( fp_dsc_c , "			remain_len = buf_size - 1 ;\n" );
			fprintf( fp_dsc_c , "		}\n" );
			fprintf( fp_dsc_c , "		else\n" );
			fprintf( fp_dsc_c , "		{\n" );
			fprintf( fp_dsc_c , "			buf_size = 1024 ;\n" );
			fprintf( fp_dsc_c , "			tmp = (char*)malloc( buf_size ) ;\n" );
			fprintf( fp_dsc_c , "			if( tmp == NULL )\n" );
			fprintf( fp_dsc_c , "				return -2;\n" );
			fprintf( fp_dsc_c , "			memset( tmp , 0x00 , buf_size );\n" );
			fprintf( fp_dsc_c , "			(*pp_base) = tmp ;\n" );
			fprintf( fp_dsc_c , "			remain_len = buf_size - 1 ;\n" );
			fprintf( fp_dsc_c , "		}\n" );
			fprintf( fp_dsc_c , "	}\n" );
			fprintf( fp_dsc_c , "	else\n" );
			fprintf( fp_dsc_c , "	{\n" );
			fprintf( fp_dsc_c , "		buf_size = (*p_buf_size) ;\n" );
			fprintf( fp_dsc_c , "		remain_len = (*p_len) ;\n" );
			fprintf( fp_dsc_c , "	}\n" );
			fprintf( fp_dsc_c , "	buf_begin_offset = buf_size-1 - remain_len ;\n" );
			fprintf( fp_dsc_c , "	buf = (*pp_base) + buf_begin_offset ;\n" );
			fprintf( fp_dsc_c , "	memset( tabs , '\\t' , %d );\n" , DSC_MAXDEPTH );
			nret = GenerateCCode_c_DSCSERIALIZE_XML_COMPACT_DUP( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	if( p_buf_size )\n" );
			fprintf( fp_dsc_c , "		(*p_buf_size) = buf_size ;\n" );
			fprintf( fp_dsc_c , "	if( p_len )\n" );
			fprintf( fp_dsc_c , "		(*p_len) = buf_size-1 - buf_begin_offset - remain_len ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCSERIALIZEXML_COMPACT_DUP_%s_V( void *pv , char *encoding , char **pp_base , int *p_buf_size , int *p_len )\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	return DSCSERIALIZE_XML_COMPACT_DUP_%s( (%s*)pv , encoding , pp_base , p_buf_size , p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "}\n" );
		}
		
		if( pcp->output_c_xml_flag == 1 || pcp->output_c_xml_compact_flag == 1 )
		{
			char	xmlpathname[ 1024 + 1 ] ;
			
			/* Generate DSCDESERIALIZE_XML_*() */
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "funcCallbackOnXmlNode CallbackOnXmlNode_%s ;\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "int CallbackOnXmlNode_%s( int type , char *xpath , int xpath_len , int xpath_size , char *node , int node_len , char *properties , int properties_len , char *content , int content_len , void *p )\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	%s	*pst = (%s*)p ;\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	int	len = 0 ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			fprintf( fp_dsc_c , "	if( type & FASTERXML_NODE_BRANCH )\n" );
			fprintf( fp_dsc_c , "	{\n" );
			fprintf( fp_dsc_c , "		if( type & FASTERXML_NODE_ENTER )\n" );
			fprintf( fp_dsc_c , "		{\n" );
			SNPRINTF( xmlpathname , sizeof(xmlpathname)-1 , "/%s" , next_struct->struct_name );
			nret = GenerateCCode_c_DSCDESERIALIZE_XML_ENTERNODE( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" , xmlpathname ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "		}\n" );
			fprintf( fp_dsc_c , "		else if( type & FASTERXML_NODE_LEAVE )\n" );
			fprintf( fp_dsc_c , "		{\n" );
			SNPRINTF( xmlpathname , sizeof(xmlpathname)-1 , "/%s" , next_struct->struct_name );
			nret = GenerateCCode_c_DSCDESERIALIZE_XML_LEAVENODE( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" , xmlpathname ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "		}\n" );
			fprintf( fp_dsc_c , "	}\n" );
			fprintf( fp_dsc_c , "	else if( type & FASTERXML_NODE_LEAF )\n" );
			fprintf( fp_dsc_c , "	{\n" );
			SNPRINTF( xmlpathname , sizeof(xmlpathname)-1 , "/%s" , next_struct->struct_name );
			nret = GenerateCCode_c_DSCDESERIALIZE_XML_LEAF( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" , xmlpathname ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	}\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
		}
		if( pcp->output_c_xml_flag == 1 )
		{
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCDESERIALIZE_XML_%s( char *encoding , char *buf , int *p_len , %s *pst )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	char	xpath[ 1024 + 1 ] ;\n" );
			fprintf( fp_dsc_c , "	int	nret = 0 ;\n" );
			fprintf( fp_dsc_c , "	memset( xpath , 0x00 , sizeof(xpath) );\n" );
			fprintf( fp_dsc_c , "	nret = TravelXmlBuffer( buf , xpath , sizeof(xpath) , & CallbackOnXmlNode_%s , (void*)pst ) ;\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "	if( nret )\n" );
			fprintf( fp_dsc_c , "		return nret;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCDESERIALIZE_XML_%s_V( char *encoding , char *buf , int *p_len , void *pv )\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	return DSCDESERIALIZE_XML_%s( encoding , buf , p_len , (%s*)pv );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "}\n" );
		}
		if( pcp->output_c_xml_compact_flag == 1 )
		{
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCDESERIALIZE_XML_COMPACT_%s( char *encoding , char *buf , int *p_len , %s *pst )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	char	xpath[ 1024 + 1 ] ;\n" );
			fprintf( fp_dsc_c , "	int	nret = 0 ;\n" );
			fprintf( fp_dsc_c , "	memset( xpath , 0x00 , sizeof(xpath) );\n" );
			fprintf( fp_dsc_c , "	nret = TravelXmlBuffer( buf , xpath , sizeof(xpath) , & CallbackOnXmlNode_%s , (void*)pst ) ;\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "	if( nret )\n" );
			fprintf( fp_dsc_c , "		return nret;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCDESERIALIZE_XML_COMPACT_%s_V( char *encoding , char *buf , int *p_len , void *pv )\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	return DSCDESERIALIZE_XML_COMPACT_%s( encoding , buf , p_len , (%s*)pv );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "}\n" );
		}
		
		if( pcp->output_c_json_flag == 1 || pcp->output_c_json_compact_flag == 1 )
		{
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "#include \"fasterjson.h\"\n" );
		}
		
		if( pcp->output_c_json_flag == 1 )
		{
			/* Generate DSCSERIALIZE_JSON_*() */
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCSERIALIZE_JSON_%s( %s *pst , char *encoding , char *buf , int *p_len )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	int	remain_len ;\n" );
			fprintf( fp_dsc_c , "	int	len ;\n" );
			fprintf( fp_dsc_c , "	char	tabs[%d+1] ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			fprintf( fp_dsc_c , "	remain_len = (*p_len) ;\n" );
			fprintf( fp_dsc_c , "	memset( tabs , '\\t' , %d );\n" , DSC_MAXDEPTH );
			nret = GenerateCCode_c_DSCSERIALIZE_JSON( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	(*p_len) = (*p_len) - remain_len ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCSERIALIZE_JSON_DUP_%s( %s *pst , char *encoding , char **pp_base , int *p_buf_size , int *p_len )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	int	buf_size ;\n" );
			fprintf( fp_dsc_c , "	int	remain_len ;\n" );
			fprintf( fp_dsc_c , "	char	*buf = NULL ;\n" );
			fprintf( fp_dsc_c , "	int	buf_begin_offset ;\n" );
			fprintf( fp_dsc_c , "	int	len ;\n" );
			fprintf( fp_dsc_c , "	char	tabs[%d+1] ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			fprintf( fp_dsc_c , "	if( (*pp_base) == NULL )\n" );
			fprintf( fp_dsc_c , "	{\n" );
			fprintf( fp_dsc_c , "		char	*tmp = NULL ;\n" );
			fprintf( fp_dsc_c , "		if( p_buf_size && (*p_buf_size) > 0 )\n" );
			fprintf( fp_dsc_c , "		{\n" );
			fprintf( fp_dsc_c , "			buf_size = (*p_buf_size) ;\n" );
			fprintf( fp_dsc_c , "			tmp = (char*)malloc( buf_size ) ;\n" );
			fprintf( fp_dsc_c , "			if( tmp == NULL )\n" );
			fprintf( fp_dsc_c , "				return -2;\n" );
			fprintf( fp_dsc_c , "			memset( tmp , 0x00 , buf_size );\n" );
			fprintf( fp_dsc_c , "			(*pp_base) = tmp ;\n" );
			fprintf( fp_dsc_c , "			remain_len = buf_size - 1 ;\n" );
			fprintf( fp_dsc_c , "		}\n" );
			fprintf( fp_dsc_c , "		else\n" );
			fprintf( fp_dsc_c , "		{\n" );
			fprintf( fp_dsc_c , "			buf_size = 1024 ;\n" );
			fprintf( fp_dsc_c , "			tmp = (char*)malloc( buf_size ) ;\n" );
			fprintf( fp_dsc_c , "			if( tmp == NULL )\n" );
			fprintf( fp_dsc_c , "				return -2;\n" );
			fprintf( fp_dsc_c , "			memset( tmp , 0x00 , buf_size );\n" );
			fprintf( fp_dsc_c , "			(*pp_base) = tmp ;\n" );
			fprintf( fp_dsc_c , "			remain_len = buf_size - 1 ;\n" );
			fprintf( fp_dsc_c , "		}\n" );
			fprintf( fp_dsc_c , "	}\n" );
			fprintf( fp_dsc_c , "	else\n" );
			fprintf( fp_dsc_c , "	{\n" );
			fprintf( fp_dsc_c , "		buf_size = (*p_buf_size) ;\n" );
			fprintf( fp_dsc_c , "		remain_len = (*p_len) ;\n" );
			fprintf( fp_dsc_c , "	}\n" );
			fprintf( fp_dsc_c , "	buf_begin_offset = buf_size-1 - remain_len ;\n" );
			fprintf( fp_dsc_c , "	buf = (*pp_base) + buf_begin_offset ;\n" );
			fprintf( fp_dsc_c , "	memset( tabs , '\\t' , %d );\n" , DSC_MAXDEPTH );
			nret = GenerateCCode_c_DSCSERIALIZE_JSON_DUP( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	if( p_buf_size )\n" );
			fprintf( fp_dsc_c , "		(*p_buf_size) = buf_size ;\n" );
			fprintf( fp_dsc_c , "	if( p_len )\n" );
			fprintf( fp_dsc_c , "		(*p_len) = buf_size-1 - buf_begin_offset - remain_len ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCSERIALIZE_JSON_DUP_%s_V( void *pv , char *encoding , char **pp_base , int *p_buf_size , int *p_len )\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	return DSCSERIALIZE_JSON_DUP_%s( (%s*)pv , encoding , pp_base , p_buf_size , p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "}\n" );
		}
		
		if( pcp->output_c_json_compact_flag == 1 )
		{
			/* Generate DSCSERIALIZE_JSON_*() */
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCSERIALIZE_JSON_COMPACT_%s( %s *pst , char *encoding , char *buf , int *p_len )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	int	remain_len ;\n" );
			fprintf( fp_dsc_c , "	int	len ;\n" );
			fprintf( fp_dsc_c , "	char	tabs[%d+1] ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			fprintf( fp_dsc_c , "	remain_len = (*p_len) ;\n" );
			fprintf( fp_dsc_c , "	memset( tabs , '\\t' , %d );\n" , DSC_MAXDEPTH );
			nret = GenerateCCode_c_DSCSERIALIZE_JSON_COMPACT( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	(*p_len) = (*p_len) - remain_len ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCSERIALIZE_JSON_COMPACT_DUP_%s( %s *pst , char *encoding , char **pp_base , int *p_buf_size , int *p_len )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	int	buf_size ;\n" );
			fprintf( fp_dsc_c , "	int	remain_len ;\n" );
			fprintf( fp_dsc_c , "	char	*buf = NULL ;\n" );
			fprintf( fp_dsc_c , "	int	buf_begin_offset ;\n" );
			fprintf( fp_dsc_c , "	int	len ;\n" );
			fprintf( fp_dsc_c , "	char	tabs[%d+1] ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			fprintf( fp_dsc_c , "	if( (*pp_base) == NULL )\n" );
			fprintf( fp_dsc_c , "	{\n" );
			fprintf( fp_dsc_c , "		char	*tmp = NULL ;\n" );
			fprintf( fp_dsc_c , "		if( p_buf_size && (*p_buf_size) > 0 )\n" );
			fprintf( fp_dsc_c , "		{\n" );
			fprintf( fp_dsc_c , "			buf_size = (*p_buf_size) ;\n" );
			fprintf( fp_dsc_c , "			tmp = (char*)malloc( buf_size ) ;\n" );
			fprintf( fp_dsc_c , "			if( tmp == NULL )\n" );
			fprintf( fp_dsc_c , "				return -2;\n" );
			fprintf( fp_dsc_c , "			memset( tmp , 0x00 , buf_size );\n" );
			fprintf( fp_dsc_c , "			(*pp_base) = tmp ;\n" );
			fprintf( fp_dsc_c , "			remain_len = buf_size - 1 ;\n" );
			fprintf( fp_dsc_c , "		}\n" );
			fprintf( fp_dsc_c , "		else\n" );
			fprintf( fp_dsc_c , "		{\n" );
			fprintf( fp_dsc_c , "			buf_size = 1024 ;\n" );
			fprintf( fp_dsc_c , "			tmp = (char*)malloc( buf_size ) ;\n" );
			fprintf( fp_dsc_c , "			if( tmp == NULL )\n" );
			fprintf( fp_dsc_c , "				return -2;\n" );
			fprintf( fp_dsc_c , "			memset( tmp , 0x00 , buf_size );\n" );
			fprintf( fp_dsc_c , "			(*pp_base) = tmp ;\n" );
			fprintf( fp_dsc_c , "			remain_len = buf_size - 1 ;\n" );
			fprintf( fp_dsc_c , "		}\n" );
			fprintf( fp_dsc_c , "	}\n" );
			fprintf( fp_dsc_c , "	else\n" );
			fprintf( fp_dsc_c , "	{\n" );
			fprintf( fp_dsc_c , "		buf_size = (*p_buf_size) ;\n" );
			fprintf( fp_dsc_c , "		remain_len = (*p_len) ;\n" );
			fprintf( fp_dsc_c , "	}\n" );
			fprintf( fp_dsc_c , "	buf_begin_offset = buf_size-1 - remain_len ;\n" );
			fprintf( fp_dsc_c , "	buf = (*pp_base) + buf_begin_offset ;\n" );
			fprintf( fp_dsc_c , "	memset( tabs , '\\t' , %d );\n" , DSC_MAXDEPTH );
			nret = GenerateCCode_c_DSCSERIALIZE_JSON_COMPACT_DUP( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	if( p_buf_size )\n" );
			fprintf( fp_dsc_c , "		(*p_buf_size) = buf_size ;\n" );
			fprintf( fp_dsc_c , "	if( p_len )\n" );
			fprintf( fp_dsc_c , "		(*p_len) = buf_size-1 - buf_begin_offset - remain_len ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCSERIALIZE_JSON_COMPACT_DUP_%s_V( void *pv , char *encoding , char **pp_base , int *p_buf_size , int *p_len )\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	return DSCSERIALIZE_JSON_COMPACT_DUP_%s( (%s*)pv , encoding , pp_base , p_buf_size , p_len );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "}\n" );
		}
		
		if( pcp->output_c_json_flag == 1 || pcp->output_c_json_compact_flag == 1 )
		{
			char	jsonpathname[ 1024 + 1 ] ;
			
			/* Generate DSCDESERIALIZE_JSON_*() */
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "funcCallbackOnJsonNode CallbackOnJsonNode_%s ;\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "int CallbackOnJsonNode_%s( int type , char *jpath , int jpath_len , int jpath_size , char *node , int node_len , char *content , int content_len , void *p )\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	%s	*pst = (%s*)p ;\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "	int	index[%d] = { 0 } ;\n" , DSC_MAXDEPTH );
			fprintf( fp_dsc_c , "	int	len ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	index[0]++; index[0] = 0 ;\n" );
			fprintf( fp_dsc_c , "	len = 0 ;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	if( type & FASTERJSON_NODE_BRANCH )\n" );
			fprintf( fp_dsc_c , "	{\n" );
			fprintf( fp_dsc_c , "		if( type & FASTERJSON_NODE_ENTER )\n" );
			fprintf( fp_dsc_c , "		{\n" );
			nret = GenerateCCode_c_DSCDESERIALIZE_JSON_ENTERNODE( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" , "" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "		}\n" );
			fprintf( fp_dsc_c , "		else if( type & FASTERJSON_NODE_LEAVE )\n" );
			fprintf( fp_dsc_c , "		{\n" );
			nret = GenerateCCode_c_DSCDESERIALIZE_JSON_LEAVENODE( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" , "" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "		}\n" );
			fprintf( fp_dsc_c , "	}\n" );
			fprintf( fp_dsc_c , "	else if( type & FASTERJSON_NODE_LEAF )\n" );
			fprintf( fp_dsc_c , "	{\n" );
			SNPRINTF( jsonpathname , sizeof(jsonpathname)-1 , "/%s" , next_struct->struct_name );
			nret = GenerateCCode_c_DSCDESERIALIZE_JSON_LEAF( pcp , 0 , next_struct , 1 , fp_dsc_c , "pst->" , "" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_c , "	}\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
		}
		if( pcp->output_c_json_flag == 1 )
		{	
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCDESERIALIZE_JSON_%s( char *encoding , char *buf , int *p_len , %s *pst )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	char	jpath[ 1024 + 1 ] ;\n" );
			fprintf( fp_dsc_c , "	int	nret = 0 ;\n" );
			fprintf( fp_dsc_c , "	memset( jpath , 0x00 , sizeof(jpath) );\n" );
			fprintf( fp_dsc_c , "	nret = TravelJsonBuffer( buf , jpath , sizeof(jpath) , & CallbackOnJsonNode_%s , (void*)pst ) ;\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "	if( nret )\n" );
			fprintf( fp_dsc_c , "		return nret;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCDESERIALIZE_JSON_%s_V( char *encoding , char *buf , int *p_len , void *pv )\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	return DSCDESERIALIZE_JSON_%s( encoding , buf , p_len , (%s*)pv );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "}\n" );
		}
		if( pcp->output_c_json_compact_flag == 1 )
		{	
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCDESERIALIZE_JSON_COMPACT_%s( char *encoding , char *buf , int *p_len , %s *pst )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	char	jpath[ 1024 + 1 ] ;\n" );
			fprintf( fp_dsc_c , "	int	nret = 0 ;\n" );
			fprintf( fp_dsc_c , "	memset( jpath , 0x00 , sizeof(jpath) );\n" );
			fprintf( fp_dsc_c , "	nret = TravelJsonBuffer( buf , jpath , sizeof(jpath) , & CallbackOnJsonNode_%s , (void*)pst ) ;\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "	if( nret )\n" );
			fprintf( fp_dsc_c , "		return nret;\n" );
			fprintf( fp_dsc_c , "	\n" );
			fprintf( fp_dsc_c , "	return 0;\n" );
			fprintf( fp_dsc_c , "}\n" );
			fprintf( fp_dsc_c , "\n" );
			fprintf( fp_dsc_c , "int DSCDESERIALIZE_JSON_COMPACT_%s_V( char *encoding , char *buf , int *p_len , void *pv )\n" , next_struct->struct_name );
			fprintf( fp_dsc_c , "{\n" );
			fprintf( fp_dsc_c , "	return DSCDESERIALIZE_JSON_COMPACT_%s( encoding , buf , p_len , (%s*)pv );\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_c , "}\n" );
		}
		
		if( pcp->output_c_LOG_flag == 1 )
		{
			/* Generate *.dsc.LOG.c */
			fprintf( fp_dsc_LOG_c , "/* It had generated by DirectStruct v%s */\n" , __DIRECTSTRUCT_VERSION );
			fprintf( fp_dsc_LOG_c , "#include \"%s.h\"\n" , pcp->pathfilename );
			fprintf( fp_dsc_LOG_c , "\n" );
			fprintf( fp_dsc_LOG_c , "#ifndef FUNCNAME_DSCLOG_%s\n" , next_struct->struct_name );
			fprintf( fp_dsc_LOG_c , "#define FUNCNAME_DSCLOG_%s	DSCLOG_%s\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_LOG_c , "#endif\n" );
			fprintf( fp_dsc_LOG_c , "\n" );
			fprintf( fp_dsc_LOG_c , "#ifndef PREFIX_DSCLOG_%s\n" , next_struct->struct_name );
			fprintf( fp_dsc_LOG_c , "#define PREFIX_DSCLOG_%s	printf( \n" , next_struct->struct_name );
			fprintf( fp_dsc_LOG_c , "#endif\n" );
			fprintf( fp_dsc_LOG_c , "\n" );
			fprintf( fp_dsc_LOG_c , "#ifndef NEWLINE_DSCLOG_%s\n" , next_struct->struct_name );
			fprintf( fp_dsc_LOG_c , "#define NEWLINE_DSCLOG_%s	\"\\n\"\n" , next_struct->struct_name );
			fprintf( fp_dsc_LOG_c , "#endif\n" );
			fprintf( fp_dsc_LOG_c , "\n" );
			fprintf( fp_dsc_LOG_c , "int FUNCNAME_DSCLOG_%s( %s *pst )\n" , next_struct->struct_name , next_struct->struct_name );
			fprintf( fp_dsc_LOG_c , "{\n" );
			fprintf( fp_dsc_LOG_c , "	int	index[%d] = { 0 } ; index[0]++; index[0] = 0 ;\n" , DSC_MAXDEPTH );
			SNPRINTF( LOG_pathname , sizeof(LOG_pathname)-1 , "%s." , next_struct->struct_name );
			nret = GenerateCCode_LOG_c( pcp , 0 , next_struct , next_struct , 1 , fp_dsc_LOG_c , LOG_pathname , "pst->" ) ;
			if( nret )
				return nret;
			fprintf( fp_dsc_LOG_c , "	return 0;\n" );
			fprintf( fp_dsc_LOG_c , "}\n" );
		}
		
		fprintf( fp_dsc_c , "int DSCGetErrorLine_%s()\n" , next_struct->struct_name );
		fprintf( fp_dsc_c , "{\n" );
		fprintf( fp_dsc_c , "	return _DSC_errline;\n" );
		fprintf( fp_dsc_c , "}\n" );
		fprintf( fp_dsc_c , "\n" );
	}
	
	return 0;
}

int GenerateSqlCode_sql_create( struct StructInfo *pstruct , FILE *fp_dsc_create_sql )
{
	struct FieldInfo	*pfield = NULL ;
	int			flag ;
	
	struct CreateSqlOutput	*create_sql_line = NULL ;
	
	fprintf( fp_dsc_create_sql , "-- It had generated by DirectStruct v%s\n" , __DIRECTSTRUCT_VERSION );
	fprintf( fp_dsc_create_sql , "\n" );
	fprintf( fp_dsc_create_sql , "CREATE TABLE %s\n" , pstruct->struct_name );
	fprintf( fp_dsc_create_sql , "(\n" );
	for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
	{
		flag = 1 ;
		if( STRCMP( pfield->field_type , == , "INT" ) )
		{
			if( pfield->field_len == 1 )
			{
				fprintf( fp_dsc_create_sql , "	%s	SMALLINT" , pfield->field_name );
			}
			else if( pfield->field_len == 2 )
			{
				fprintf( fp_dsc_create_sql , "	%s	SMALLINT" , pfield->field_name );
			}
			else if( pfield->field_len == 4 )
			{
				fprintf( fp_dsc_create_sql , "	%s	INTEGER" , pfield->field_name );
			}
			else if( pfield->field_len == 8 )
			{
				fprintf( fp_dsc_create_sql , "	%s	BIGINT" , pfield->field_name );
			}
			else
			{
				flag = 0 ;
			}
		}
		else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
		{
			if( pfield->field_len == 4 )
			{
				fprintf( fp_dsc_create_sql , "	%s	REAL" , pfield->field_name );
			}
			else if( pfield->field_len == 8 )
			{
				fprintf( fp_dsc_create_sql , "	%s	DOUBLE PRECISION" , pfield->field_name );
			}
			else
			{
				flag = 0 ;
			}
		}
		else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
		{
			fprintf( fp_dsc_create_sql , "	%s	NUMERIC(%d,%d)" , pfield->field_name , pfield->field_len , pfield->field_len2 );
		}
		else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
		{
			fprintf( fp_dsc_create_sql , "	%s	NUMERIC(%d,2)" , pfield->field_name , (pfield->field_len-8)/2*4 );
		}
		else if( STRCMP( pfield->field_type , == , "STRING" ) )
		{
			fprintf( fp_dsc_create_sql , "	%s	CHARACTER VARYING(%d)" , pfield->field_name , pfield->field_len );
		}
		else if( STRCMP( pfield->field_type , == , "BOOL" ) )
		{
			if( pfield->field_len == 1 )
			{
				fprintf( fp_dsc_create_sql , "	%s	BOOLEAN" , pfield->field_name );
			}
		}
		else
		{
			flag = 0 ;
		}
		
		if( flag == 1 )
		{
			if( STRCMP( pfield->init_default , != , "" ) )
			{
				if( STRCMP( pfield->field_type , == , "STRING" ) )
					fprintf( fp_dsc_create_sql , "	DEFAULT '%s'" , pfield->init_default );
				else
					fprintf( fp_dsc_create_sql , "	DEFAULT %s" , pfield->init_default );
			}
			
			if( pfield->null_or_notnull == DSC_FIELD_NULL )
			{
				fprintf( fp_dsc_create_sql , " NULL" );
			}
			else if( pfield->null_or_notnull == DSC_FIELD_NOTNULL )
			{
				fprintf( fp_dsc_create_sql , " NOT NULL" );
			}
			
			if( pfield->next_field )
			{
				fprintf( fp_dsc_create_sql , " ,\n" );
			}
			else
			{
				fprintf( fp_dsc_create_sql , "\n" );
			}
		}
	}
	fprintf( fp_dsc_create_sql , ") ;\n" );
	
	if( pstruct->first_create_sql_line )
	{
		fprintf( fp_dsc_create_sql , "\n" );
	}
	for( create_sql_line = pstruct->first_create_sql_line ; create_sql_line ; create_sql_line = create_sql_line->next_line )
	{
		fprintf( fp_dsc_create_sql , "%s\n" , create_sql_line->content );
	}
	
	fprintf( fp_dsc_create_sql , "\n" );
	if( pstruct->struct_desc[0] )
	{
		fprintf( fp_dsc_create_sql , "COMMENT ON TABLE %s IS '%s' ;\n" , pstruct->struct_name , pstruct->struct_desc );
	}
	for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
	{
		if( pfield->field_desc[0] )
		{
			fprintf( fp_dsc_create_sql , "COMMENT ON COLUMN %s.%s IS '%s' ;\n" , pstruct->struct_name , pfield->field_name , pfield->field_desc );
		}
	}
	
	return 0;
}

int GenerateSqlCode_sql_drop( struct StructInfo *pstruct , FILE *fp_dsc_drop_sql )
{
	struct FieldInfo	*pfield = NULL ;
	struct DropSqlOutput	*drop_sql_line = NULL ;
	
	fprintf( fp_dsc_drop_sql , "-- It had generated by DirectStruct v%s\n" , __DIRECTSTRUCT_VERSION );
	
	fprintf( fp_dsc_drop_sql , "\n" );
	if( pstruct->struct_desc[0] )
	{
		fprintf( fp_dsc_drop_sql , "COMMENT ON TABLE %s IS NULL ;\n" , pstruct->struct_name );
	}
	for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
	{
		if( pfield->field_desc[0] )
		{
			fprintf( fp_dsc_drop_sql , "COMMENT ON COLUMN %s.%s IS NULL ;\n" , pstruct->struct_name , pfield->field_name );
		}
	}
	
	if( pstruct->first_drop_sql_line )
	{
		fprintf( fp_dsc_drop_sql , "\n" );
	}
	for( drop_sql_line = pstruct->first_drop_sql_line ; drop_sql_line ; drop_sql_line = drop_sql_line->next_line )
	{
		fprintf( fp_dsc_drop_sql , "%s\n" , drop_sql_line->content );
	}
	
	fprintf( fp_dsc_drop_sql , "\n" );
	fprintf( fp_dsc_drop_sql , "DROP TABLE %s ;\n" , pstruct->struct_name );
	fprintf( fp_dsc_drop_sql , "\n" );
	
	return 0;
}

int GenerateSqlCode( struct CommandParameter *pcp , struct StructInfo *pstruct , FILE *fp_dsc_create_sql , FILE *fp_dsc_drop_sql )
{
	int		nret = 0 ;
	
	/* Generate *.dsc.create.sql */
	nret = GenerateSqlCode_sql_create( pstruct->next_struct , fp_dsc_create_sql ) ;
	if( nret )
		return nret;
	
	/* Generate *.dsc.drop.sql */
	nret = GenerateSqlCode_sql_drop( pstruct->next_struct , fp_dsc_drop_sql ) ;
	if( nret )
		return nret;
	
	return 0;
}

static int GenerateECCode_eh_SQL( struct CommandParameter *pcp , struct StructInfo *pstruct , FILE *fp_dsc_ESQL_eh )
{
	struct FieldInfo	*pfield = NULL ;
	struct SqlActionOutput	*sqlaction_line ;
	
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "EXEC SQL BEGIN DECLARE SECTION ;\n" );
	for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
	{
		if( STRCMP( pfield->field_type , == , "INT" ) )
		{
			if( pfield->field_len == 1 )
			{
				fprintf( fp_dsc_ESQL_eh , "	extern char %s_%s ;	extern short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 2 )
			{
				fprintf( fp_dsc_ESQL_eh , "	extern short %s_%s ;	extern short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 4 )
			{
				fprintf( fp_dsc_ESQL_eh , "	extern int %s_%s ;	extern short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 8 )
			{
				fprintf( fp_dsc_ESQL_eh , "	extern longlong %s_%s ;	extern short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
		}
		else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
		{
			if( pfield->field_len == 4 )
			{
				fprintf( fp_dsc_ESQL_eh , "	extern float %s_%s ;	extern short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 8 )
			{
				fprintf( fp_dsc_ESQL_eh , "	extern double %s_%s ;	extern short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
		}
		else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
		{
			fprintf( fp_dsc_ESQL_eh , "	extern double %s_%s ;	extern short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
		{
			fprintf( fp_dsc_ESQL_eh , "	extern double %s_%s ;	extern short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "STRING" ) )
		{
			fprintf( fp_dsc_ESQL_eh , "	extern char %s_%s[ %d + 1 ] ;	extern short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pfield->field_len , pstruct->struct_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "BOOL" ) )
		{
			if( pfield->field_len == 1 )
			{
				fprintf( fp_dsc_ESQL_eh , "	extern char %s_%s ;	extern short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
		}
	}
	fprintf( fp_dsc_ESQL_eh , "EXEC SQL END DECLARE SECTION ;\n" );
	
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#define	TFLIST_%s \\\n" , pstruct->struct_name );
	for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
	{
		fprintf( fp_dsc_ESQL_eh , "	%s" , pfield->field_name );
		
		if( pfield->next_field )
		{
			fprintf( fp_dsc_ESQL_eh , " , \\\n" );
		}
		else
		{
			fprintf( fp_dsc_ESQL_eh , "\n" );
		}
	}
	
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#define	DBVLIST_%s \\\n" , pstruct->struct_name );
	for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
	{
		fprintf( fp_dsc_ESQL_eh , "	:%s_%s" , pstruct->struct_name , pfield->field_name );
		
		if( pfield->next_field )
		{
			fprintf( fp_dsc_ESQL_eh , " , \\\n" );
		}
		else
		{
			fprintf( fp_dsc_ESQL_eh , "\n" );
		}
	}
	
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#define	DBVLLIST_%s \\\n" , pstruct->struct_name );
	for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
	{
		fprintf( fp_dsc_ESQL_eh , "	:%s_%s :%s_%s_id" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
		
		if( pfield->next_field )
		{
			fprintf( fp_dsc_ESQL_eh , " , \\\n" );
		}
		else
		{
			fprintf( fp_dsc_ESQL_eh , "\n" );
		}
	}
	
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "void DSCINITV_%s();\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_eh , "void DSCVTOS_%s( %s *pst );\n" , pstruct->struct_name , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_eh , "void DSCSTOV_%s( %s *pst );\n" , pstruct->struct_name , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_eh , "void DSCTRIM_%s( %s *pst );\n" , pstruct->struct_name , pstruct->struct_name );
	
	if( pstruct->first_sqlaction_line )
	{
		fprintf( fp_dsc_ESQL_eh , "\n" );
	}
	for( sqlaction_line = pstruct->first_sqlaction_line ; sqlaction_line ; sqlaction_line = sqlaction_line->next_line )
	{
		if( STRNCMP( sqlaction_line->content , == , "CURSOR" , 6 ) )
		{
			char	cursor_name[ 256 + 1 ] ;
			
			memset( cursor_name , 0x00 , sizeof(cursor_name) );
			sscanf( sqlaction_line->content , "%*s%s" , cursor_name );
			
			fprintf( fp_dsc_ESQL_eh , "void DSCSQLACTION_OPEN_%s( %s *pst );\n" , sqlaction_line->funcname , pstruct->struct_name );
			fprintf( fp_dsc_ESQL_eh , "void DSCSQLACTION_FETCH_CURSOR_%s( %s *pst );\n" , cursor_name , pstruct->struct_name );
			fprintf( fp_dsc_ESQL_eh , "void DSCSQLACTION_CLOSE_CURSOR_%s();\n" , cursor_name );
		}
		else
		{
			fprintf( fp_dsc_ESQL_eh , "void DSCSQLACTION_%s( %s *pst );\n" , sqlaction_line->funcname , pstruct->struct_name );
		}
	}
	
	if( pstruct->sqlconn == 1 )
	{
		fprintf( fp_dsc_ESQL_eh , "void DSCDBCONN( char *host , int port , char *dbname , char *user , char *pass );\n" );
		fprintf( fp_dsc_ESQL_eh , "void DSCDBDISCONN();\n" );
		fprintf( fp_dsc_ESQL_eh , "void DSCDBBEGINWORK();\n" );
		fprintf( fp_dsc_ESQL_eh , "void DSCDBCOMMIT();\n" );
		fprintf( fp_dsc_ESQL_eh , "void DSCDBROLLBACK();\n" );
	}
	
	return 0;
}

static int GenerateECCode_ec_SQL_ACTION_SELECT( struct StructInfo *pstruct , struct SqlActionOutput *sqlaction_line , FILE *fp_dsc_ESQL_ec )
{
	char		*ptr = NULL , *saveptr = NULL ;
	char		*ptr2 = NULL , *saveptr2 = NULL ;
	
	saveptr = NULL ;
	ptr = strtok_r( strdup( sqlaction_line->content ) , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCSQLACTION_%s( %s *pst )\n" , sqlaction_line->funcname , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	fprintf( fp_dsc_ESQL_ec , "	DSCSTOV_%s( pst );\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
	if( sqlaction_line->connection_name )
	{
	fprintf( fp_dsc_ESQL_ec , "		AT	%s\n" , sqlaction_line->connection_name );
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "		SELECT	%s\n" , ptr );
	
	if( STRCMP( ptr , == , "*" ) )
	{
		fprintf( fp_dsc_ESQL_ec , "		INTO	DBVLLIST_%s\n" , pstruct->struct_name );
	}
	else
	{
		saveptr2 = NULL ;
		ptr2 = strtok_r( ptr , "," , & saveptr2 ) ;
		if( ptr2 == NULL )
		{
			fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
			return -1;
		}
		
		fprintf( fp_dsc_ESQL_ec , "		INTO	:%s_%s :%s_%s_id" , pstruct->struct_name , ptr2 , pstruct->struct_name , ptr2 );
		
		while( ( ptr2 = strtok_r( NULL , "," , & saveptr2 ) ) )
		{
			fprintf( fp_dsc_ESQL_ec , " , :%s_%s :%s_%s_id" , pstruct->struct_name , ptr2 , pstruct->struct_name , ptr2 );
		}
		
		fprintf( fp_dsc_ESQL_ec , "\n" );
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	if( STRCMP( ptr , != , "FROM" ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	if( STRCMP( ptr , != , pstruct->struct_name ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "		FROM	%s\n" , ptr );
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr != NULL )
	{
		if( STRCMP( ptr , == , "WHERE" ) )
		{
			char	*ptr10 = NULL ;
			char	*ptr11 = NULL ;
			char	*ptr12 = NULL ;
			
			fprintf( fp_dsc_ESQL_ec , "		WHERE	" );
			
			ptr11 = strtok_r( NULL , " \t" , & saveptr ) ;
			if( ptr11 == NULL )
			{
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
				return -1;
			}
			
			ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
			if( ptr12 == NULL )
			{
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
				return -1;
			}
			
			fprintf( fp_dsc_ESQL_ec , "%s %s :%s_%s" , ptr11 , ptr12 , pstruct->struct_name , ptr11 );
			
			while( ( ptr10 = strtok_r( NULL , " \t" , & saveptr ) ) )
			{
				ptr11 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr11 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
					return -1;
				}
				
				ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr12 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
					return -1;
				}
				
				fprintf( fp_dsc_ESQL_ec , " %s %s %s :%s_%s" , ptr10 , ptr11 , ptr12 , pstruct->struct_name , ptr11 );
			}
			
			fprintf( fp_dsc_ESQL_ec , "\n" );
		}
	}
	
	fprintf( fp_dsc_ESQL_ec , "		;\n" );
	fprintf( fp_dsc_ESQL_ec , "	if( SQLCODE )\n" );
	fprintf( fp_dsc_ESQL_ec , "		return;\n" );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	DSCVTOS_%s( pst );\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	return;\n" );
	fprintf( fp_dsc_ESQL_ec , "}\n" );
	
	return 0;
}

static int GenerateECCode_ec_SQL_ACTION_INSERT( struct StructInfo *pstruct , struct SqlActionOutput *sqlaction_line , FILE *fp_dsc_ESQL_ec )
{
	char		*ptr = NULL , *saveptr = NULL ;
	
	saveptr = NULL ;
	ptr = strtok_r( strdup( sqlaction_line->content ) , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCSQLACTION_%s( %s *pst )\n" , sqlaction_line->funcname , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	fprintf( fp_dsc_ESQL_ec , "	DSCSTOV_%s( pst );\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
	if( sqlaction_line->connection_name )
	{
	fprintf( fp_dsc_ESQL_ec , "		AT	%s\n" , sqlaction_line->connection_name );
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	if( STRCMP( ptr , != , "INTO" ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	if( STRCMP( ptr , != , pstruct->struct_name ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "		INSERT INTO	%s\n" , ptr );
	fprintf( fp_dsc_ESQL_ec , "		VALUES ( DBVLLIST_%s )\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "		;\n" );
	fprintf( fp_dsc_ESQL_ec , "	return;\n" );
	fprintf( fp_dsc_ESQL_ec , "}\n" );
	
	return 0;
}

static int GenerateECCode_ec_SQL_ACTION_UPDATE( struct StructInfo *pstruct , struct SqlActionOutput *sqlaction_line , FILE *fp_dsc_ESQL_ec )
{
	char			*ptr = NULL , *saveptr = NULL ;
	char			*ptr2 = NULL , *saveptr2 = NULL ;
	struct FieldInfo	*pfield = NULL ;
	
	saveptr = NULL ;
	ptr = strtok_r( strdup( sqlaction_line->content ) , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCSQLACTION_%s( %s *pst )\n" , sqlaction_line->funcname , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	fprintf( fp_dsc_ESQL_ec , "	DSCSTOV_%s( pst );\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
	if( sqlaction_line->connection_name )
	{
	fprintf( fp_dsc_ESQL_ec , "		AT	%s\n" , sqlaction_line->connection_name );
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	if( STRCMP( ptr , != , pstruct->struct_name ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "		UPDATE	%s\n" , ptr );
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	if( STRCMP( ptr , != , "SET" ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	if( STRCMP( ptr , == , "*" ) )
	{
		fprintf( fp_dsc_ESQL_ec , "		SET	" );
		for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
		{
			fprintf( fp_dsc_ESQL_ec , "%s = :%s_%s " , pfield->field_name , pstruct->struct_name , pfield->field_name );
			if( pfield->next_field )
			{
				fprintf( fp_dsc_ESQL_ec , ", " );
			}
		}
		fprintf( fp_dsc_ESQL_ec , "\n" );
	}
	else
	{
		saveptr2 = NULL ;
		ptr2 = strtok_r( ptr , "," , & saveptr2 ) ;
		if( ptr2 == NULL )
		{
			fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
			return -1;
		}
		
		fprintf( fp_dsc_ESQL_ec , "		SET	%s = :%s_%s" , ptr2 , pstruct->struct_name , ptr2 );
		
		while( ( ptr2 = strtok_r( NULL , "," , & saveptr2 ) ) )
		{
			fprintf( fp_dsc_ESQL_ec , ", %s = :%s_%s" , ptr2 , pstruct->struct_name , ptr2 );
		}
		
		fprintf( fp_dsc_ESQL_ec , "\n" );
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr )
	{
		if( STRCMP( ptr , == , "WHERE" ) )
		{
			char	*ptr10 = NULL ;
			char	*ptr11 = NULL ;
			char	*ptr12 = NULL ;
			
			fprintf( fp_dsc_ESQL_ec , "		WHERE	" );
			
			ptr11 = strtok_r( NULL , " \t" , & saveptr ) ;
			if( ptr11 == NULL )
			{
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
				return -1;
			}
			
			ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
			if( ptr12 == NULL )
			{
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
				return -1;
			}
			
			fprintf( fp_dsc_ESQL_ec , "%s %s :%s_%s" , ptr11 , ptr12 , pstruct->struct_name , ptr11 );
			
			while( ( ptr10 = strtok_r( NULL , " \t" , & saveptr ) ) )
			{
				ptr11 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr11 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
					return -1;
				}
				
				ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr12 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
					return -1;
				}
				
				fprintf( fp_dsc_ESQL_ec , " %s %s %s :%s_%s" , ptr10 , ptr11 , ptr12 , pstruct->struct_name , ptr11 );
			}
			
			fprintf( fp_dsc_ESQL_ec , "\n" );
		}
	}
	
	fprintf( fp_dsc_ESQL_ec , "		;\n" );
	fprintf( fp_dsc_ESQL_ec , "	return;\n" );
	fprintf( fp_dsc_ESQL_ec , "}\n" );
	
	return 0;
}

static int GenerateECCode_ec_SQL_ACTION_DELETE( struct StructInfo *pstruct , struct SqlActionOutput *sqlaction_line , FILE *fp_dsc_ESQL_ec )
{
	char		*ptr = NULL , *saveptr = NULL ;
	
	saveptr = NULL ;
	ptr = strtok_r( strdup( sqlaction_line->content ) , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCSQLACTION_%s( %s *pst )\n" , sqlaction_line->funcname , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	fprintf( fp_dsc_ESQL_ec , "	DSCSTOV_%s( pst );\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
	if( sqlaction_line->connection_name )
	{
	fprintf( fp_dsc_ESQL_ec , "		AT	%s\n" , sqlaction_line->connection_name );
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	if( STRCMP( ptr , != , "FROM" ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	if( STRCMP( ptr , != , pstruct->struct_name ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "		DELETE\n" );
	fprintf( fp_dsc_ESQL_ec , "		FROM	%s\n" , ptr );
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr )
	{
		if( STRCMP( ptr , == , "WHERE" ) )
		{
			char	*ptr10 = NULL ;
			char	*ptr11 = NULL ;
			char	*ptr12 = NULL ;
			
			fprintf( fp_dsc_ESQL_ec , "		WHERE	" );
			
			ptr11 = strtok_r( NULL , " \t" , & saveptr ) ;
			if( ptr11 == NULL )
			{
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
				return -1;
			}
			
			ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
			if( ptr12 == NULL )
			{
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
				return -1;
			}
			
			fprintf( fp_dsc_ESQL_ec , "%s %s :%s_%s" , ptr11 , ptr12 , pstruct->struct_name , ptr11 );
			
			while( ( ptr10 = strtok_r( NULL , " \t" , & saveptr ) ) )
			{
				ptr11 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr11 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
					return -1;
				}
				
				ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr12 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
					return -1;
				}
				
				fprintf( fp_dsc_ESQL_ec , " %s %s %s :%s_%s" , ptr10 , ptr11 , ptr12 , pstruct->struct_name , ptr11 );
			}
			
			fprintf( fp_dsc_ESQL_ec , "\n" );
		}
	}
	
	fprintf( fp_dsc_ESQL_ec , "		;\n" );
	fprintf( fp_dsc_ESQL_ec , "	return;\n" );
	fprintf( fp_dsc_ESQL_ec , "}\n" );
	
	return 0;
}

static int GenerateECCode_ec_SQL_ACTION_CURSOR( struct StructInfo *pstruct , struct SqlActionOutput *sqlaction_line , FILE *fp_dsc_ESQL_ec )
{
	char		*cursor_name = NULL ;
	char		*ptr = NULL , *saveptr = NULL ;
	char		*ptr2 = NULL , *saveptr2 = NULL ;
	
	saveptr = NULL ;
	ptr = strtok_r( strdup( sqlaction_line->content ) , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	cursor_name = ptr ;
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCSQLACTION_OPEN_%s( %s *pst )\n" , sqlaction_line->funcname , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	fprintf( fp_dsc_ESQL_ec , "	DSCSTOV_%s( pst );\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
	if( sqlaction_line->connection_name )
	{
	fprintf( fp_dsc_ESQL_ec , "		AT	%s\n" , sqlaction_line->connection_name );
	}
	fprintf( fp_dsc_ESQL_ec , "		DECLARE	%s_%s CURSOR FOR\n" , pstruct->struct_name , cursor_name );
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	if( STRCMP( ptr , != , "SELECT" ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	if( STRCMP( ptr , == , "*" ) )
	{
		fprintf( fp_dsc_ESQL_ec , "		SELECT	*\n" );
	}
	else
	{
		fprintf( fp_dsc_ESQL_ec , "		SELECT	%s\n" , ptr );
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	if( STRCMP( ptr , != , pstruct->struct_name ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "		FROM	%s\n" , ptr );
	
	while( ( ptr = strtok_r( NULL , " \t" , & saveptr ) ) )
	{
		if( STRCMP( ptr , == , "WHERE" ) )
		{
			char	*ptr10 = NULL ;
			char	*ptr11 = NULL ;
			char	*ptr12 = NULL ;
			
			fprintf( fp_dsc_ESQL_ec , "		WHERE	" );
			
			ptr11 = strtok_r( NULL , " \t" , & saveptr ) ;
			if( ptr11 == NULL )
			{
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
				return -1;
			}
			
			ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
			if( ptr12 == NULL )
			{
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
				return -1;
			}
			
			fprintf( fp_dsc_ESQL_ec , "%s %s :%s_%s" , ptr11 , ptr12 , pstruct->struct_name , ptr11 );
			
			while( ( ptr10 = strtok_r( NULL , " \t" , & saveptr ) ) )
			{
				if( STRCMP( ptr10 , != , "AND" ) && STRCMP( ptr10 , != , "OR" ) )
						break;
				
				ptr11 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr11 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
					return -1;
				}
				
				ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr12 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
					return -1;
				}
				
				fprintf( fp_dsc_ESQL_ec , " %s %s %s :%s_%s" , ptr10 , ptr11 , ptr12 , pstruct->struct_name , ptr11 );
			}
			
			fprintf( fp_dsc_ESQL_ec , "\n" );
			
			if( ptr10 )
			{
				ptr = ptr10 ;
				goto _OUTPUT_CURSOR_REMAIN_SECTIONS;
			}
		}
		else
		{
_OUTPUT_CURSOR_REMAIN_SECTIONS :
			fprintf( fp_dsc_ESQL_ec , "		%s" , ptr );
			
			while( ( ptr = strtok_r( NULL , " \t" , & saveptr ) ) )
			{
				fprintf( fp_dsc_ESQL_ec , " %s " , ptr );
			}
			
			fprintf( fp_dsc_ESQL_ec , "\n" );
		}
	}
	
	fprintf( fp_dsc_ESQL_ec , "		;\n" );
	fprintf( fp_dsc_ESQL_ec , "	if( SQLCODE )\n" );
	fprintf( fp_dsc_ESQL_ec , "		return;\n" );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
	fprintf( fp_dsc_ESQL_ec , "		OPEN	%s_%s\n" , pstruct->struct_name , cursor_name );
	fprintf( fp_dsc_ESQL_ec , "		;\n" );
	fprintf( fp_dsc_ESQL_ec , "	if( SQLCODE )\n" );
	fprintf( fp_dsc_ESQL_ec , "		return;\n" );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	return;\n" );
	fprintf( fp_dsc_ESQL_ec , "}\n" );
	
	saveptr = NULL ;
	ptr = strtok_r( strdup( sqlaction_line->content ) , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCSQLACTION_FETCH_CURSOR_%s( %s *pst )\n" , cursor_name , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
	fprintf( fp_dsc_ESQL_ec , "		FETCH	%s_%s\n" , pstruct->struct_name , cursor_name );
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	if( STRCMP( ptr , != , "SELECT" ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
		return -1;
	}
	
	if( STRCMP( ptr , == , "*" ) )
	{
		fprintf( fp_dsc_ESQL_ec , "		INTO	DBVLLIST_%s\n" , pstruct->struct_name );
	}
	else
	{
		saveptr2 = NULL ;
		ptr2 = strtok_r( ptr , "," , & saveptr2 ) ;
		if( ptr2 == NULL )
		{
			fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction_line->content );
			return -1;
		}
		
		fprintf( fp_dsc_ESQL_ec , "		INTO	:%s_%s :%s_%s_id" , pstruct->struct_name , ptr2 , pstruct->struct_name , ptr2 );
		
		while( ( ptr2 = strtok_r( NULL , "," , & saveptr2 ) ) )
		{
			fprintf( fp_dsc_ESQL_ec , ", :%s_%s :%s_%s_id" , pstruct->struct_name , ptr2 , pstruct->struct_name , ptr2 );
		}
		
		fprintf( fp_dsc_ESQL_ec , "\n" );
	}
	
	fprintf( fp_dsc_ESQL_ec , "		;\n" );
	fprintf( fp_dsc_ESQL_ec , "	if( SQLCODE )\n" );
	fprintf( fp_dsc_ESQL_ec , "		return;\n" );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	DSCVTOS_%s( pst );\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	return;\n" );
	fprintf( fp_dsc_ESQL_ec , "}\n" );
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCSQLACTION_CLOSE_CURSOR_%s()\n" , cursor_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
	fprintf( fp_dsc_ESQL_ec , "		CLOSE	%s_%s\n" , pstruct->struct_name , cursor_name );
	fprintf( fp_dsc_ESQL_ec , "		;\n" );
	fprintf( fp_dsc_ESQL_ec , "	return;\n" );
	fprintf( fp_dsc_ESQL_ec , "}\n" );
	
	return 0;
}

static int GenerateECCode_ec_SQL( struct CommandParameter *pcp , struct StructInfo *pstruct , FILE *fp_dsc_ESQL_ec )
{
	struct FieldInfo	*pfield = NULL ;
	struct SqlActionOutput	*sqlaction_line ;
	
	int			nret = 0 ;
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "EXEC SQL BEGIN DECLARE SECTION ;\n" );
	for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
	{
		if( STRCMP( pfield->field_type , == , "INT" ) )
		{
			if( pfield->field_len == 1 )
			{
				fprintf( fp_dsc_ESQL_ec , "	char %s_%s ;	short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 2 )
			{
				fprintf( fp_dsc_ESQL_ec , "	short %s_%s ;	short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 4 )
			{
				fprintf( fp_dsc_ESQL_ec , "	int %s_%s ;	short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 8 )
			{
				fprintf( fp_dsc_ESQL_ec , "	longlong %s_%s ;	short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
		}
		else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
		{
			if( pfield->field_len == 4 )
			{
				fprintf( fp_dsc_ESQL_ec , "	float %s_%s ;	short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 8 )
			{
				fprintf( fp_dsc_ESQL_ec , "	double %s_%s ;	short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
		}
		else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
		{
			fprintf( fp_dsc_ESQL_ec , "	double %s_%s ;	short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
		{
			fprintf( fp_dsc_ESQL_ec , "	double %s_%s ;	short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "STRING" ) )
		{
			fprintf( fp_dsc_ESQL_ec , "	char %s_%s[ %d + 1 ] ;	short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pfield->field_len , pstruct->struct_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "BOOL" ) )
		{
			if( pfield->field_len == 1 )
			{
				fprintf( fp_dsc_ESQL_ec , "	char %s_%s ;	short %s_%s_id ;\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
		}
	}
	fprintf( fp_dsc_ESQL_ec , "EXEC SQL END DECLARE SECTION ;\n" );
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCINITV_%s()\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
	{
		if( STRCMP( pfield->field_type , == , "INT" ) )
		{
			if( pfield->field_len == 1 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = 0 ;\n" , pstruct->struct_name , pfield->field_name );
				fprintf( fp_dsc_ESQL_ec , "	%s_%s_id = 0 ;\n" , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 2 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = 0 ;\n" , pstruct->struct_name , pfield->field_name );
				fprintf( fp_dsc_ESQL_ec , "	%s_%s_id = 0 ;\n" , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 4 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = 0 ;\n" , pstruct->struct_name , pfield->field_name );
				fprintf( fp_dsc_ESQL_ec , "	%s_%s_id = 0 ;\n" , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 8 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = 0 ;\n" , pstruct->struct_name , pfield->field_name );
				fprintf( fp_dsc_ESQL_ec , "	%s_%s_id = 0 ;\n" , pstruct->struct_name , pfield->field_name );
			}
		}
		else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
		{
			if( pfield->field_len == 4 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = 0.0 ;\n" , pstruct->struct_name , pfield->field_name );
				fprintf( fp_dsc_ESQL_ec , "	%s_%s_id = 0 ;\n" , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 8 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = 0.00 ;\n" , pstruct->struct_name , pfield->field_name );
				fprintf( fp_dsc_ESQL_ec , "	%s_%s_id = 0 ;\n" , pstruct->struct_name , pfield->field_name );
			}
		}
		else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
		{
			fprintf( fp_dsc_ESQL_ec , "	%s_%s = 0.00 ;\n" , pstruct->struct_name , pfield->field_name );
			fprintf( fp_dsc_ESQL_ec , "	%s_%s_id = 0 ;\n" , pstruct->struct_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
		{
			fprintf( fp_dsc_ESQL_ec , "	%s_%s = 0.00 ;\n" , pstruct->struct_name , pfield->field_name );
			fprintf( fp_dsc_ESQL_ec , "	%s_%s_id = 0 ;\n" , pstruct->struct_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "STRING" ) )
		{
			fprintf( fp_dsc_ESQL_ec , "	memset( %s_%s , 0x00 , sizeof(%s_%s) );\n" , pstruct->struct_name , pfield->field_name , pstruct->struct_name , pfield->field_name );
			fprintf( fp_dsc_ESQL_ec , "	%s_%s_id = 0 ;\n" , pstruct->struct_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "BOOL" ) )
		{
			if( pfield->field_len == 1 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = 0 ;\n" , pstruct->struct_name , pfield->field_name );
				fprintf( fp_dsc_ESQL_ec , "	%s_%s_id = 0 ;\n" , pstruct->struct_name , pfield->field_name );
			}
		}
	}
	fprintf( fp_dsc_ESQL_ec , "	return;\n" );
	fprintf( fp_dsc_ESQL_ec , "}\n" );
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCVTOS_%s( %s *pst )\n" , pstruct->struct_name , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
	{
		if( STRCMP( pfield->field_type , == , "INT" ) )
		{
			if( pfield->field_len == 1 )
			{
				fprintf( fp_dsc_ESQL_ec , "	pst->%s = %s_%s ;\n" , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 2 )
			{
				fprintf( fp_dsc_ESQL_ec , "	pst->%s = %s_%s ;\n" , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 4 )
			{
				fprintf( fp_dsc_ESQL_ec , "	pst->%s = %s_%s ;\n" , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 8 )
			{
				fprintf( fp_dsc_ESQL_ec , "	pst->%s = %s_%s ;\n" , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
		}
		else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
		{
			if( pfield->field_len == 4 )
			{
				fprintf( fp_dsc_ESQL_ec , "	pst->%s = %s_%s ;\n" , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
			else if( pfield->field_len == 8 )
			{
				fprintf( fp_dsc_ESQL_ec , "	pst->%s = %s_%s ;\n" , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
		}
		else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
		{
			fprintf( fp_dsc_ESQL_ec , "	pst->%s = %s_%s ;\n" , pfield->field_name , pstruct->struct_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
		{
			fprintf( fp_dsc_ESQL_ec , "	pst->%s = %s_%s ;\n" , pfield->field_name , pstruct->struct_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "STRING" ) )
		{
			fprintf( fp_dsc_ESQL_ec , "	strcpy( pst->%s , %s_%s );\n" , pfield->field_name , pstruct->struct_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "BOOL" ) )
		{
			if( pfield->field_len == 1 )
			{
				fprintf( fp_dsc_ESQL_ec , "	pst->%s = %s_%s ;\n" , pfield->field_name , pstruct->struct_name , pfield->field_name );
			}
		}
	}
	fprintf( fp_dsc_ESQL_ec , "	return;\n" );
	fprintf( fp_dsc_ESQL_ec , "}\n" );
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCSTOV_%s( %s *pst )\n" , pstruct->struct_name , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
	{
		if( STRCMP( pfield->field_type , == , "INT" ) )
		{
			if( pfield->field_len == 1 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = pst->%s ;\n" , pstruct->struct_name , pfield->field_name , pfield->field_name );
			}
			else if( pfield->field_len == 2 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = pst->%s ;\n" , pstruct->struct_name , pfield->field_name , pfield->field_name );
			}
			else if( pfield->field_len == 4 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = pst->%s ;\n" , pstruct->struct_name , pfield->field_name , pfield->field_name );
			}
			else if( pfield->field_len == 8 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = pst->%s ;\n" , pstruct->struct_name , pfield->field_name , pfield->field_name );
			}
		}
		else if( STRCMP( pfield->field_type , == , "FLOAT" ) )
		{
			if( pfield->field_len == 4 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = pst->%s ;\n" , pstruct->struct_name , pfield->field_name , pfield->field_name );
			}
			else if( pfield->field_len == 8 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = pst->%s ;\n" , pstruct->struct_name , pfield->field_name , pfield->field_name );
			}
		}
		else if( STRCMP( pfield->field_type , == , "NUMERIC" ) )
		{
			fprintf( fp_dsc_ESQL_ec , "	%s_%s = pst->%s ;\n" , pstruct->struct_name , pfield->field_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "AMOUNT" ) )
		{
			fprintf( fp_dsc_ESQL_ec , "	%s_%s = pst->%s ;\n" , pstruct->struct_name , pfield->field_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "STRING" ) )
		{
			fprintf( fp_dsc_ESQL_ec , "	strcpy( %s_%s , pst->%s );\n" , pstruct->struct_name , pfield->field_name , pfield->field_name );
		}
		else if( STRCMP( pfield->field_type , == , "BOOL" ) )
		{
			if( pfield->field_len == 1 )
			{
				fprintf( fp_dsc_ESQL_ec , "	%s_%s = pst->%s ;\n" , pstruct->struct_name , pfield->field_name , pfield->field_name );
			}
		}
	}
	fprintf( fp_dsc_ESQL_ec , "	return;\n" );
	fprintf( fp_dsc_ESQL_ec , "}\n" );
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "#ifndef TRIM\n" );
	fprintf( fp_dsc_ESQL_ec , "#define TRIM(_str_) {char *p=(_str_)+strlen(_str_)-1; for(;((*p)==' ')&&(p>=(_str_));p--){(*p)='\\0';}}\n" );
	fprintf( fp_dsc_ESQL_ec , "#endif\n" );
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCTRIM_%s( %s *pst )\n" , pstruct->struct_name , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	for( pfield = pstruct->first_field ; pfield ; pfield = pfield->next_field )
	{
		if( STRCMP( pfield->field_type , == , "STRING" ) )
		{
			fprintf( fp_dsc_ESQL_ec , "	TRIM( pst->%s )\n" , pfield->field_name );
		}
	}
	fprintf( fp_dsc_ESQL_ec , "	return;\n" );
	fprintf( fp_dsc_ESQL_ec , "}\n" );
	
	for( sqlaction_line = pstruct->first_sqlaction_line ; sqlaction_line ; sqlaction_line = sqlaction_line->next_line )
	{
		if( STRNCMP( sqlaction_line->content , == , "CURSOR" , 6 ) )
		{
			nret = GenerateECCode_ec_SQL_ACTION_CURSOR( pstruct , sqlaction_line , fp_dsc_ESQL_ec ) ;
			if( nret )
				return nret;
		}
		else if( STRNCMP( sqlaction_line->content , == , "SELECT" , 6 ) )
		{
			nret = GenerateECCode_ec_SQL_ACTION_SELECT( pstruct , sqlaction_line , fp_dsc_ESQL_ec ) ;
			if( nret )
				return nret;
		}
		else if( STRNCMP( sqlaction_line->content , == , "INSERT" , 6 ) )
		{
			nret = GenerateECCode_ec_SQL_ACTION_INSERT( pstruct , sqlaction_line , fp_dsc_ESQL_ec ) ;
			if( nret )
				return nret;
		}
		else if( STRNCMP( sqlaction_line->content , == , "UPDATE" , 6 ) )
		{
			nret = GenerateECCode_ec_SQL_ACTION_UPDATE( pstruct , sqlaction_line , fp_dsc_ESQL_ec ) ;
			if( nret )
				return nret;
		}
		else if( STRNCMP( sqlaction_line->content , == , "DELETE" , 6 ) )
		{
			nret = GenerateECCode_ec_SQL_ACTION_DELETE( pstruct , sqlaction_line , fp_dsc_ESQL_ec ) ;
			if( nret )
				return nret;
		}
	}
	
	if( pstruct->sqlconn == 1 )
	{
		struct SqlConnectionName	*pconnection = NULL ;
		
		fprintf( fp_dsc_ESQL_ec , "\n" );
		fprintf( fp_dsc_ESQL_ec , "EXEC SQL BEGIN DECLARE SECTION ;\n" );
		fprintf( fp_dsc_ESQL_ec , "	char    DBNAME[ 1024 + 1 ] ;\n" );
		fprintf( fp_dsc_ESQL_ec , "	char    DBUSER[ 128 + 1 ] ;\n" );
		fprintf( fp_dsc_ESQL_ec , "	char    DBPASS[ 128 + 1 ] ;\n" );
		fprintf( fp_dsc_ESQL_ec , "EXEC SQL END DECLARE SECTION ;\n" );
		
		for( pconnection = pstruct->first_connection_name_line ; pconnection ; pconnection = pconnection->next_line )
		{
			fprintf( fp_dsc_ESQL_ec , "\n" );
			if( pconnection->connection_name )
				fprintf( fp_dsc_ESQL_ec , "void DSCDBCONN_%s( char *host , int port , char *dbname , char *user , char *pass )\n" , pconnection->connection_name );
			else
				fprintf( fp_dsc_ESQL_ec , "void DSCDBCONN( char *host , int port , char *dbname , char *user , char *pass )\n" );
			fprintf( fp_dsc_ESQL_ec , "{\n" );
			if( pcp->output_ec_pgsql_flag == 1 )
			{
				fprintf( fp_dsc_ESQL_ec , "	strcpy( DBNAME , dbname );\n" );
				fprintf( fp_dsc_ESQL_ec , "	if( host )\n" );
				fprintf( fp_dsc_ESQL_ec , "	{\n" );
				fprintf( fp_dsc_ESQL_ec , "		strcat( DBNAME , \"@\" );\n" );
				fprintf( fp_dsc_ESQL_ec , "		strcat( DBNAME , host );\n" );
				fprintf( fp_dsc_ESQL_ec , "	}\n" );
				fprintf( fp_dsc_ESQL_ec , "	if( port )\n" );
				fprintf( fp_dsc_ESQL_ec , "	{\n" );
				fprintf( fp_dsc_ESQL_ec , "		strcat( DBNAME , \":\" );\n" );
				fprintf( fp_dsc_ESQL_ec , "		sprintf( DBNAME + strlen(DBNAME) , \"%%d\" , port );\n" );
				fprintf( fp_dsc_ESQL_ec , "	}\n" );
				fprintf( fp_dsc_ESQL_ec , "	strcpy( DBUSER , user );\n" );
				fprintf( fp_dsc_ESQL_ec , "	strcpy( DBPASS , pass );\n" );
				fprintf( fp_dsc_ESQL_ec , "	\n" );
				fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
				fprintf( fp_dsc_ESQL_ec , "		CONNECT TO	:DBNAME\n" );
				if( pconnection->connection_name )
				{
				fprintf( fp_dsc_ESQL_ec , "		AS		%s\n" , pconnection->connection_name );
				}
				fprintf( fp_dsc_ESQL_ec , "		USER		:DBUSER\n" );
				fprintf( fp_dsc_ESQL_ec , "		IDENTIFIED BY	:DBPASS ;\n" );
			}
			else if( pcp->output_ec_oracle_flag == 1 )
			{
				fprintf( fp_dsc_ESQL_ec , "	strcpy( DBUSER , user );\n" );
				fprintf( fp_dsc_ESQL_ec , "	strcpy( DBPASS , pass );\n" );
				fprintf( fp_dsc_ESQL_ec , "	\n" );
				fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
				fprintf( fp_dsc_ESQL_ec , "		CONNECT		:DBUSER\n" );
				if( pconnection->connection_name )
				{
				fprintf( fp_dsc_ESQL_ec , "		AS		%s\n" , pconnection->connection_name );
				}
				fprintf( fp_dsc_ESQL_ec , "		IDENTIFIED BY	:DBPASS ;\n" );
			}
			fprintf( fp_dsc_ESQL_ec , "	\n" );
			fprintf( fp_dsc_ESQL_ec , "	return;\n" );
			fprintf( fp_dsc_ESQL_ec , "}\n" );
		}
		
		fprintf( fp_dsc_ESQL_ec , "void DSCDBDISCONN()\n" );
		fprintf( fp_dsc_ESQL_ec , "{\n" );
		if( pcp->output_ec_pgsql_flag == 1 )
		{
			fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
			fprintf( fp_dsc_ESQL_ec , "		DISCONNECT ALL ;\n" );
		}
		fprintf( fp_dsc_ESQL_ec , "	\n" );
		fprintf( fp_dsc_ESQL_ec , "	return;\n" );
		fprintf( fp_dsc_ESQL_ec , "}\n" );
		
		fprintf( fp_dsc_ESQL_ec , "void DSCDBBEGINWORK()\n" );
		fprintf( fp_dsc_ESQL_ec , "{\n" );
		if( pcp->output_ec_pgsql_flag == 1 )
		{
			fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
			fprintf( fp_dsc_ESQL_ec , "		BEGIN WORK ;\n" );
		}
		fprintf( fp_dsc_ESQL_ec , "	\n" );
		fprintf( fp_dsc_ESQL_ec , "	return;\n" );
		fprintf( fp_dsc_ESQL_ec , "}\n" );
		
		fprintf( fp_dsc_ESQL_ec , "void DSCDBCOMMIT()\n" );
		fprintf( fp_dsc_ESQL_ec , "{\n" );
		fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
		fprintf( fp_dsc_ESQL_ec , "		COMMIT WORK ;\n" );
		fprintf( fp_dsc_ESQL_ec , "	\n" );
		fprintf( fp_dsc_ESQL_ec , "	return;\n" );
		fprintf( fp_dsc_ESQL_ec , "}\n" );
		
		fprintf( fp_dsc_ESQL_ec , "void DSCDBROLLBACK()\n" );
		fprintf( fp_dsc_ESQL_ec , "{\n" );
		fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
		fprintf( fp_dsc_ESQL_ec , "		ROLLBACK WORK ;\n" );
		fprintf( fp_dsc_ESQL_ec , "	\n" );
		fprintf( fp_dsc_ESQL_ec , "	return;\n" );
		fprintf( fp_dsc_ESQL_ec , "}\n" );
	}
	
	return 0;
}

int GenerateECCode_PGSQL( struct CommandParameter *pcp , struct StructInfo *pstruct , FILE *fp_dsc_ESQL_eh , FILE *fp_dsc_ESQL_ec )
{
	int		nret = 0 ;
	
	/* Generate *.dsc.ESQL.eh */
	fprintf( fp_dsc_ESQL_eh , "/* It had generated by DirectStruct v%s */\n" , __DIRECTSTRUCT_VERSION );
	fprintf( fp_dsc_ESQL_eh , "#ifndef _H_%s_ESQL_\n" , pstruct->next_struct->struct_name );
	fprintf( fp_dsc_ESQL_eh , "#define _H_%s_ESQL_\n" , pstruct->next_struct->struct_name );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#define _PQSQL\n" );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#include <ecpglib.h>\n" );
	fprintf( fp_dsc_ESQL_eh , "#include <ecpgerrno.h>\n" );
	fprintf( fp_dsc_ESQL_eh , "#include <sqlca.h>\n" );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#include \"%s.h\"\n" , pcp->pathfilename );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#undef SQLCODE\n" );
	fprintf( fp_dsc_ESQL_eh , "#define SQLCODE		(int)(sqlca.sqlcode)\n" );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#undef SQLSTATE\n" );
	fprintf( fp_dsc_ESQL_eh , "#define SQLSTATE	sqlca.sqlstate\n" );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#undef SQLDESC\n" );
	fprintf( fp_dsc_ESQL_eh , "#define SQLDESC		sqlca.sqlerrm.sqlerrmc\n" );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#ifndef SQLNOTFOUND\n" );
	fprintf( fp_dsc_ESQL_eh , "#define SQLNOTFOUND	100\n" );
	fprintf( fp_dsc_ESQL_eh , "#endif\n" );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	nret = GenerateECCode_eh_SQL( pcp , pstruct->next_struct , fp_dsc_ESQL_eh ) ;
	if( nret )
		return nret;
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#endif\n" );
	
	/* Generate *.dsc.ESQL.ec */
	fprintf( fp_dsc_ESQL_ec , "/* It had generated by DirectStruct v%s */\n" , __DIRECTSTRUCT_VERSION );
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "#include \"%s.ESQL.eh\"\n" , pcp->pathfilename );
	nret = GenerateECCode_ec_SQL( pcp , pstruct->next_struct , fp_dsc_ESQL_ec ) ;
	if( nret )
		return nret;
	
	return 0;
}

int GenerateECCode_ORACLE( struct CommandParameter *pcp , struct StructInfo *pstruct , FILE *fp_dsc_ESQL_eh , FILE *fp_dsc_ESQL_ec )
{
	int		nret = 0 ;
	
	/* Generate *.dsc.ESQL.eh */
	fprintf( fp_dsc_ESQL_eh , "/* It had generated by DirectStruct v%s */\n" , __DIRECTSTRUCT_VERSION );
	fprintf( fp_dsc_ESQL_eh , "#ifndef _H_%s_ESQL_\n" , pstruct->next_struct->struct_name );
	fprintf( fp_dsc_ESQL_eh , "#define _H_%s_ESQL_\n" , pstruct->next_struct->struct_name );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#include \"%s.h\"\n" , pcp->pathfilename );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#define _ORACLE\n" );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#include <sqlca.h>\n" );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#ifndef SQLCODE\n" );
	fprintf( fp_dsc_ESQL_eh , "#define SQLCODE		(int)(sqlca.sqlcode)\n" );
	fprintf( fp_dsc_ESQL_eh , "#define SQLSTATE	\"\"\n" );
	fprintf( fp_dsc_ESQL_eh , "#define SQLDESC		\"\"\n" );
	fprintf( fp_dsc_ESQL_eh , "#endif\n" );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#ifndef SQLNOTFOUND\n" );
	fprintf( fp_dsc_ESQL_eh , "#define SQLNOTFOUND	1403\n" );
	fprintf( fp_dsc_ESQL_eh , "#endif\n" );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	nret = GenerateECCode_eh_SQL( pcp , pstruct->next_struct , fp_dsc_ESQL_eh ) ;
	if( nret )
		return nret;
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#endif\n" );
	
	/* Generate *.dsc.ESQL.ec */
	fprintf( fp_dsc_ESQL_ec , "/* It had generated by DirectStruct v%s */\n" , __DIRECTSTRUCT_VERSION );
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "#include \"%s.ESQL.eh\"\n" , pcp->pathfilename );
	nret = GenerateECCode_ec_SQL( pcp , pstruct->next_struct , fp_dsc_ESQL_ec ) ;
	if( nret )
		return nret;
	
	return 0;
}

int dsc( struct CommandParameter *pcmdparam )
{
	FILE			*fp_dsc = NULL ;
	int			offset ;
	int			lineno ;
	struct StructInfo	si ;
	int			struct_len ;
	
	char			output_pathfilename[ 256 + 1 ] ;
	
	int			nret = 0 ;
	
	fp_dsc = fopen( pcmdparam->pathfilename , "r" ) ;
	if( fp_dsc == NULL )
	{
		fprintf( stderr , "file[%s] can't read\n" , pcmdparam->pathfilename );
		return -1;
	}
	offset = 0 ;
	lineno = 0 ;
	memset( & si , 0x00 , sizeof(struct StructInfo) );
	while( !feof(fp_dsc) )
	{
		struct_len = ReadDscFile( pcmdparam , 0 , & offset , pcmdparam->pathfilename , 0 , fp_dsc , & lineno , & si ) ;
		if( struct_len < 0 )
			return -struct_len;
	}
	fclose( fp_dsc );
	printf( "ok!\n" );
	
	if( pcmdparam->output_c_flag == 1 )
	{
		FILE	*fp_dsc_h = NULL ;
		FILE	*fp_dsc_c = NULL ;
		FILE	*fp_dsc_LOG_c = NULL ;
		
		memset( output_pathfilename , 0x00 , sizeof(output_pathfilename) );
		SNPRINTF( output_pathfilename , sizeof(output_pathfilename)-1 , "%s.h" , pcmdparam->pathfilename );
		fp_dsc_h = fopen( output_pathfilename , "w" ) ;
		if( fp_dsc_h == NULL )
		{
			fprintf( stderr , "file[%s] can't write\n" , output_pathfilename );
			return -1;
		}
		
		memset( output_pathfilename , 0x00 , sizeof(output_pathfilename) );
		SNPRINTF( output_pathfilename , sizeof(output_pathfilename)-1 , "%s.c" , pcmdparam->pathfilename );
		fp_dsc_c = fopen( output_pathfilename , "w" ) ;
		if( fp_dsc_c == NULL )
		{
			fprintf( stderr , "file[%s] can't write\n" , output_pathfilename );
			fclose( fp_dsc_h );
			return -1;
		}
		
		memset( output_pathfilename , 0x00 , sizeof(output_pathfilename) );
		SNPRINTF( output_pathfilename , sizeof(output_pathfilename)-1 , "%s.LOG.c" , pcmdparam->pathfilename );
		fp_dsc_LOG_c = fopen( output_pathfilename , "w" ) ;
		if( fp_dsc_LOG_c == NULL )
		{
			fprintf( stderr , "file[%s] can't write\n" , output_pathfilename );
			fclose( fp_dsc_h );
			return -1;
		}
		
		nret = GenerateCCode( pcmdparam , & si , fp_dsc_h , fp_dsc_c , fp_dsc_LOG_c ) ;
		if( nret )
			return -nret;
		
		fclose( fp_dsc_h );
		fclose( fp_dsc_c );
		fclose( fp_dsc_LOG_c );
	}
	
	if( pcmdparam->output_sql_pgsql_flag == 1 || pcmdparam->output_sql_oracle_flag == 1 )
	{
		FILE	*fp_dsc_create_sql = NULL ;
		FILE	*fp_dsc_drop_sql = NULL ;
		
		memset( output_pathfilename , 0x00 , sizeof(output_pathfilename) );
		SNPRINTF( output_pathfilename , sizeof(output_pathfilename)-1 , "%s.create.sql" , pcmdparam->pathfilename );
		fp_dsc_create_sql = fopen( output_pathfilename , "w" ) ;
		if( fp_dsc_create_sql == NULL )
		{
			fprintf( stderr , "file[%s] can't write\n" , output_pathfilename );
			return -1;
		}
		
		memset( output_pathfilename , 0x00 , sizeof(output_pathfilename) );
		SNPRINTF( output_pathfilename , sizeof(output_pathfilename)-1 , "%s.drop.sql" , pcmdparam->pathfilename );
		fp_dsc_drop_sql = fopen( output_pathfilename , "w" ) ;
		if( fp_dsc_drop_sql == NULL )
		{
			fprintf( stderr , "file[%s] can't write\n" , output_pathfilename );
			return -1;
		}
		
		nret = GenerateSqlCode( pcmdparam , & si , fp_dsc_create_sql , fp_dsc_drop_sql ) ;
		if( nret )
			return -nret;
		
		fclose( fp_dsc_create_sql );
		fclose( fp_dsc_drop_sql );
	}
	
	if( pcmdparam->output_ec_pgsql_flag == 1 )
	{
		FILE	*fp_dsc_ESQL_eh = NULL ;
		FILE	*fp_dsc_ESQL_ec = NULL ;
		
		memset( output_pathfilename , 0x00 , sizeof(output_pathfilename) );
		SNPRINTF( output_pathfilename , sizeof(output_pathfilename)-1 , "%s.ESQL.eh" , pcmdparam->pathfilename );
		fp_dsc_ESQL_eh = fopen( output_pathfilename , "w" ) ;
		if( fp_dsc_ESQL_eh == NULL )
		{
			fprintf( stderr , "file[%s] can't write\n" , output_pathfilename );
			return -1;
		}
		
		memset( output_pathfilename , 0x00 , sizeof(output_pathfilename) );
		SNPRINTF( output_pathfilename , sizeof(output_pathfilename)-1 , "%s.ESQL.ec" , pcmdparam->pathfilename );
		fp_dsc_ESQL_ec = fopen( output_pathfilename , "w" ) ;
		if( fp_dsc_ESQL_ec == NULL )
		{
			fprintf( stderr , "file[%s] can't write\n" , output_pathfilename );
			return -1;
		}
		
		nret = GenerateECCode_PGSQL( pcmdparam , & si , fp_dsc_ESQL_eh , fp_dsc_ESQL_ec ) ;
		if( nret )
			return -nret;
		
		fclose( fp_dsc_ESQL_eh );
		fclose( fp_dsc_ESQL_ec );
	}
	else if( pcmdparam->output_ec_oracle_flag == 1 )
	{
		FILE	*fp_dsc_ESQL_eh = NULL ;
		FILE	*fp_dsc_ESQL_ec = NULL ;
		
		memset( output_pathfilename , 0x00 , sizeof(output_pathfilename) );
		SNPRINTF( output_pathfilename , sizeof(output_pathfilename)-1 , "%s.ESQL.eh" , pcmdparam->pathfilename );
		fp_dsc_ESQL_eh = fopen( output_pathfilename , "w" ) ;
		if( fp_dsc_ESQL_eh == NULL )
		{
			fprintf( stderr , "file[%s] can't write\n" , output_pathfilename );
			return -1;
		}
		
		memset( output_pathfilename , 0x00 , sizeof(output_pathfilename) );
		SNPRINTF( output_pathfilename , sizeof(output_pathfilename)-1 , "%s.ESQL.ec" , pcmdparam->pathfilename );
		fp_dsc_ESQL_ec = fopen( output_pathfilename , "w" ) ;
		if( fp_dsc_ESQL_ec == NULL )
		{
			fprintf( stderr , "file[%s] can't write\n" , output_pathfilename );
			return -1;
		}
		
		nret = GenerateECCode_ORACLE( pcmdparam , & si , fp_dsc_ESQL_eh , fp_dsc_ESQL_ec ) ;
		if( nret )
			return -nret;
		
		fclose( fp_dsc_ESQL_eh );
		fclose( fp_dsc_ESQL_ec );
	}
	
	return 0;
}

static void version()
{
	printf( "dsc v%s - DirectSrtuct Compiler\n" , __DIRECTSTRUCT_VERSION );
	printf( "Copyright by calvin<calvinwilliams@163.com><calvinwilliams.c@gmail.com> 2014,2015\n" );
}

static void usage()
{
	printf( "USAGE : dsc -f .dsc [ -c ] [ -c-LOG ]\n" );
	printf( "                    [ -c-order ]\n" );
	printf( "                    [ -c-compact ] [ -c-compress ]\n" );
	printf( "                    [ -c-xml | -c-xml-compact ]\n" );
	printf( "                    [ -c-json | -c-json-compact ]\n" );
	printf( "                    [ -sql-pgsql ] [ -ec-pgsql ]\n" );
	printf( "                    [ -sql-oracle ] [ -ec-oracle ]\n" );
	printf( "                    [ -c-ALL ]\n" );
}

int main( int argc , char *argv[] )
{
	int			c ;
	struct CommandParameter	cp ;
	
	if( argc == 1 )
	{
		version();
		usage();
		exit(0);
	}
	
	memset( & cp , 0x00 , sizeof(struct CommandParameter) );
	for( c = 1 ; c < argc ; c++ )
	{
		if( strcmp( argv[c] , "-v" ) == 0 )
		{
			version();
			return 0;
		}
		else if( strcmp( argv[c] , "-f" ) == 0 && c + 1 < argc )
		{
			c++;
			cp.pathfilename = argv[c] ;
		}
		else if( strcmp( argv[c] , "-c" ) == 0 )
		{
			cp.output_c_flag = 1 ;
		}
		else if( strcmp( argv[c] , "-c-order" ) == 0 )
		{
			cp.output_c_flag = 1 ;
			cp.output_c_order_flag = 1 ;
		}
		else if( strcmp( argv[c] , "-c-compact" ) == 0 )
		{
			cp.output_c_flag = 1 ;
			cp.output_c_order_flag = 1 ;
			cp.output_c_compact_flag = 1 ;
		}
		else if( strcmp( argv[c] , "-c-compress" ) == 0 )
		{
			cp.output_c_flag = 1 ;
			cp.output_c_order_flag = 1 ;
			cp.output_c_compress_flag = 1 ;
		}
		else if( strcmp( argv[c] , "-c-xml" ) == 0 )
		{
			cp.output_c_flag = 1 ;
			cp.output_c_xml_flag = 1 ;
		}
		else if( strcmp( argv[c] , "-c-xml-compact" ) == 0 )
		{
			cp.output_c_flag = 1 ;
			cp.output_c_xml_compact_flag = 1 ;
		}
		else if( strcmp( argv[c] , "-c-json" ) == 0 )
		{
			cp.output_c_flag = 1 ;
			cp.output_c_json_flag = 1 ;
		}
		else if( strcmp( argv[c] , "-c-json-compact" ) == 0 )
		{
			cp.output_c_flag = 1 ;
			cp.output_c_json_compact_flag = 1 ;
		}
		else if( strcmp( argv[c] , "-c-LOG" ) == 0 )
		{
			cp.output_c_flag = 1 ;
			cp.output_c_LOG_flag = 1 ;
		}
		else if( strcmp( argv[c] , "-sql-pgsql" ) == 0 )
		{
			cp.output_sql_pgsql_flag = 1 ;
		}
		else if( strcmp( argv[c] , "-ec-pgsql" ) == 0 )
		{
			cp.output_c_flag = 1 ;
			cp.output_ec_pgsql_flag = 1 ;
		}
		else if( strcmp( argv[c] , "-sql-oracle" ) == 0 )
		{
			cp.output_sql_oracle_flag = 1 ;
		}
		else if( strcmp( argv[c] , "-ec-oracle" ) == 0 )
		{
			cp.output_c_flag = 1 ;
			cp.output_ec_oracle_flag = 1 ;
		}
		else if( strcmp( argv[c] , "-c-ALL" ) == 0 )
		{
			cp.output_c_flag = 1 ;
			cp.output_c_order_flag = 1 ;
			cp.output_c_compact_flag = 1 ;
			cp.output_c_compress_flag = 1 ;
			cp.output_c_xml_flag = 1 ;
			cp.output_c_LOG_flag = 1 ;
		}
		else
		{
			printf( "Invalid parameter[%s]\n" , argv[c] );
			usage();
			exit(7);
		}
	}
	
	if( cp.pathfilename == NULL )
	{
		usage();
		exit(7);
	}
	
	return -dsc( & cp ) ;
}

