#ifndef _H_DSC_
#define _H_DSC_

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

/*
 * util
 */

#if ( defined _WIN32 )
char *strtok_r (char *s, const char *delim, char **save_ptr);
#endif

char *strtok2( char **base );
int ClearLeft( char *str );
int ClearRight( char *str );
char *StringNoEnter( char *str );
void fprintabs( FILE *fp , int depth );

/*
 * main
 */

extern char	*__DIRECTSTRUCT_VERSION ;

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

/*
 * dsc
 */

int dsc( struct CommandParameter *pcmdparam );

/*
 * ReadDscFile
 */

#define DSC_MAXDEPTH		10

struct StructInfo ;

#define DSC_FIELD_NULL		1
#define DSC_FIELD_NOTNULL	2

struct FieldInfo
{
	char			field_type[ 10 + 1 ] ;
	int			field_len ;
	int			field_offset ;
	char			field_name[ 64 + 1 ] ;
	
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
} ;

struct StructInfo
{
	char			struct_name[ 64 + 1 ] ;
	int			struct_length ;
	int			field_count ;
	int			array_size ;
	
	struct HeaderOutput	*first_line ;
	struct HeaderOutput	*last_line ;
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

/*
 * ReadDscFile
 */

int ReadDscFile( struct CommandParameter *pcmdparam , int depth , int *p_offset , char *dsc_pathfilename , int flag , FILE *fp_dsc , int *p_lineno , struct StructInfo *first_struct );

/*
 * GenerateCCode
 */

int GenerateCCode( struct CommandParameter *pcmdparam , struct StructInfo *first_struct , FILE *fp_dsc_h , FILE *fp_dsc_c , FILE *fp_dsc_LOG_c );
int GenerateSqlCode( struct CommandParameter *pcp , struct StructInfo *pstruct , FILE *fp_dsc_create_sql , FILE *fp_dsc_drop_sql );
int GenerateECCode_PGSQL( struct CommandParameter *pcp , struct StructInfo *pstruct , FILE *fp_dsc_ESQL_eh , FILE *fp_dsc_ESQL_ec );
int GenerateECCode_ORACLE( struct CommandParameter *pcp , struct StructInfo *pstruct , FILE *fp_dsc_ESQL_eh , FILE *fp_dsc_ESQL_ec );

#endif
