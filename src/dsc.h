#ifndef _H_DSC_
#define _H_DSC_

/*
 * dsc - DirectSrtuct compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 * LastVersion	: v1.0.2
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

int ClearLeft( char *str );
int ClearRight( char *str );
char *StringNoEnter( char *str );
void fprintabs( FILE *fp , int depth );

/*
 * main
 */

struct CommandParameter
{
	char	*pathfilename ;
	int	output_c_flag ;
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

struct FieldInfo
{
	char			field_type[ 10 + 1 ] ;
	unsigned int		field_len ;
	unsigned int		field_offset ;
	char			field_name[ 64 + 1 ] ;
	
	char			init_default[ 1024 + 1 ] ;
	
	struct FieldInfo	*next_field ;
	
	struct StructInfo	*struct_list ;
} ;

struct StructInfo
{
	char			struct_name[ 64 + 1 ] ;
	unsigned int		struct_length ;
	unsigned int		field_count ;
	unsigned int		array_size ;
	
	struct FieldInfo	*field_list ;
	struct FieldInfo	*last_field ;
	
	struct StructInfo	*sub_struct_list ;
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

int ReadDscFile( struct CommandParameter *pcmdparam , int depth , int *p_offset , char *dsc_pathfilename , int flag , FILE *fp_dsc , long lineno , struct StructInfo *pmsginfo );

/*
 * GenerateCCode
 */

int GenerateCCode( struct CommandParameter *pcmdparam , struct StructInfo *pmsginfo , FILE *fp_dsc_h , FILE *fp_dsc_c , FILE *fp_dsc_LOG_c );

#endif
