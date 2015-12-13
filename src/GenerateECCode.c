#include "dsc.h"

/*
 * dsc - DirectSrtuct compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

static int GenerateECCode_eh_SQL( struct CommandParameter *pcp , struct StructInfo *pstruct , FILE *fp_dsc_ESQL_eh )
{
	struct FieldInfo	*pfield = NULL ;
	struct SqlActionOutput	*sqlaction_line ;
	
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "EXEC SQL BEGIN DECLARE SECTION ;\n" );
	for( pfield = pstruct->field_list ; pfield ; pfield = pfield->next_field )
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
	for( pfield = pstruct->field_list ; pfield ; pfield = pfield->next_field )
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
	for( pfield = pstruct->field_list ; pfield ; pfield = pfield->next_field )
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
	for( pfield = pstruct->field_list ; pfield ; pfield = pfield->next_field )
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

static int GenerateECCode_ec_SQL_ACTION_SELECT( struct StructInfo *pstruct , char *sqlaction , char *sqlaction_funcname , FILE *fp_dsc_ESQL_ec )
{
	char		*ptr = NULL , *saveptr = NULL ;
	char		*ptr2 = NULL , *saveptr2 = NULL ;
	
	saveptr = NULL ;
	ptr = strtok_r( strdup( sqlaction ) , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCSQLACTION_%s( %s *pst )\n" , sqlaction_funcname , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	fprintf( fp_dsc_ESQL_ec , "	DSCSTOV_%s( pst );\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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
			fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	if( STRCMP( ptr , != , "FROM" ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	if( STRCMP( ptr , != , pstruct->struct_name ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
				return -1;
			}
			
			ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
			if( ptr12 == NULL )
			{
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
				return -1;
			}
			
			fprintf( fp_dsc_ESQL_ec , "%s %s :%s_%s" , ptr11 , ptr12 , pstruct->struct_name , ptr11 );
			
			while( ( ptr10 = strtok_r( NULL , " \t" , & saveptr ) ) )
			{
				ptr11 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr11 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
					return -1;
				}
				
				ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr12 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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

static int GenerateECCode_ec_SQL_ACTION_INSERT( struct StructInfo *pstruct , char *sqlaction , char *sqlaction_funcname , FILE *fp_dsc_ESQL_ec )
{
	char		*ptr = NULL , *saveptr = NULL ;
	
	saveptr = NULL ;
	ptr = strtok_r( strdup( sqlaction ) , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCSQLACTION_%s( %s *pst )\n" , sqlaction_funcname , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	fprintf( fp_dsc_ESQL_ec , "	DSCSTOV_%s( pst );\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	if( STRCMP( ptr , != , "INTO" ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	if( STRCMP( ptr , != , pstruct->struct_name ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "		INSERT INTO	%s\n" , ptr );
	fprintf( fp_dsc_ESQL_ec , "		VALUES ( DBVLLIST_%s )\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "		;\n" );
	fprintf( fp_dsc_ESQL_ec , "	return;\n" );
	fprintf( fp_dsc_ESQL_ec , "}\n" );
	
	return 0;
}

static int GenerateECCode_ec_SQL_ACTION_UPDATE( struct StructInfo *pstruct , char *sqlaction , char *sqlaction_funcname , FILE *fp_dsc_ESQL_ec )
{
	char			*ptr = NULL , *saveptr = NULL ;
	char			*ptr2 = NULL , *saveptr2 = NULL ;
	struct FieldInfo	*pfield = NULL ;
	
	saveptr = NULL ;
	ptr = strtok_r( strdup( sqlaction ) , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCSQLACTION_%s( %s *pst )\n" , sqlaction_funcname , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	fprintf( fp_dsc_ESQL_ec , "	DSCSTOV_%s( pst );\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	if( STRCMP( ptr , != , pstruct->struct_name ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "		UPDATE	%s\n" , ptr );
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	if( STRCMP( ptr , != , "SET" ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	if( STRCMP( ptr , == , "*" ) )
	{
		fprintf( fp_dsc_ESQL_ec , "		SET	" );
		for( pfield = pstruct->field_list ; pfield ; pfield = pfield->next_field )
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
			fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
				return -1;
			}
			
			ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
			if( ptr12 == NULL )
			{
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
				return -1;
			}
			
			fprintf( fp_dsc_ESQL_ec , "%s %s :%s_%s" , ptr11 , ptr12 , pstruct->struct_name , ptr11 );
			
			while( ( ptr10 = strtok_r( NULL , " \t" , & saveptr ) ) )
			{
				ptr11 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr11 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
					return -1;
				}
				
				ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr12 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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

static int GenerateECCode_ec_SQL_ACTION_DELETE( struct StructInfo *pstruct , char *sqlaction , char *sqlaction_funcname , FILE *fp_dsc_ESQL_ec )
{
	char		*ptr = NULL , *saveptr = NULL ;
	
	saveptr = NULL ;
	ptr = strtok_r( strdup( sqlaction ) , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCSQLACTION_%s( %s *pst )\n" , sqlaction_funcname , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	fprintf( fp_dsc_ESQL_ec , "	DSCSTOV_%s( pst );\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	if( STRCMP( ptr , != , "FROM" ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	if( STRCMP( ptr , != , pstruct->struct_name ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
				return -1;
			}
			
			ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
			if( ptr12 == NULL )
			{
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
				return -1;
			}
			
			fprintf( fp_dsc_ESQL_ec , "%s %s :%s_%s" , ptr11 , ptr12 , pstruct->struct_name , ptr11 );
			
			while( ( ptr10 = strtok_r( NULL , " \t" , & saveptr ) ) )
			{
				ptr11 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr11 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
					return -1;
				}
				
				ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr12 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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

static int GenerateECCode_ec_SQL_ACTION_CURSOR( struct StructInfo *pstruct , char *sqlaction , char *sqlaction_funcname , FILE *fp_dsc_ESQL_ec )
{
	char		*cursor_name = NULL ;
	char		*ptr = NULL , *saveptr = NULL ;
	char		*ptr2 = NULL , *saveptr2 = NULL ;
	
	saveptr = NULL ;
	ptr = strtok_r( strdup( sqlaction ) , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	cursor_name = ptr ;
	
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "void DSCSQLACTION_OPEN_%s( %s *pst )\n" , sqlaction_funcname , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "{\n" );
	fprintf( fp_dsc_ESQL_ec , "	DSCSTOV_%s( pst );\n" , pstruct->struct_name );
	fprintf( fp_dsc_ESQL_ec , "	\n" );
	fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
	fprintf( fp_dsc_ESQL_ec , "		DECLARE	%s_%s CURSOR FOR\n" , pstruct->struct_name , cursor_name );
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	if( STRCMP( ptr , != , "SELECT" ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	if( STRCMP( ptr , != , pstruct->struct_name ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
				return -1;
			}
			
			ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
			if( ptr12 == NULL )
			{
				fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
					return -1;
				}
				
				ptr12 = strtok_r( NULL , " \t" , & saveptr ) ;
				if( ptr12 == NULL )
				{
					fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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
	ptr = strtok_r( strdup( sqlaction ) , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	if( STRCMP( ptr , != , "SELECT" ) )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
		return -1;
	}
	
	ptr = strtok_r( NULL , " \t" , & saveptr ) ;
	if( ptr == NULL )
	{
		fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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
			fprintf( stderr , "*** ERROR : invalid sqlaction [%s]\n" , sqlaction );
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
	for( pfield = pstruct->field_list ; pfield ; pfield = pfield->next_field )
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
	for( pfield = pstruct->field_list ; pfield ; pfield = pfield->next_field )
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
	for( pfield = pstruct->field_list ; pfield ; pfield = pfield->next_field )
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
	for( pfield = pstruct->field_list ; pfield ; pfield = pfield->next_field )
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
	for( pfield = pstruct->field_list ; pfield ; pfield = pfield->next_field )
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
			nret = GenerateECCode_ec_SQL_ACTION_CURSOR( pstruct , sqlaction_line->content , sqlaction_line->funcname , fp_dsc_ESQL_ec ) ;
			if( nret )
				return nret;
		}
		else if( STRNCMP( sqlaction_line->content , == , "SELECT" , 6 ) )
		{
			nret = GenerateECCode_ec_SQL_ACTION_SELECT( pstruct , sqlaction_line->content , sqlaction_line->funcname , fp_dsc_ESQL_ec ) ;
			if( nret )
				return nret;
		}
		else if( STRNCMP( sqlaction_line->content , == , "INSERT" , 6 ) )
		{
			nret = GenerateECCode_ec_SQL_ACTION_INSERT( pstruct , sqlaction_line->content , sqlaction_line->funcname , fp_dsc_ESQL_ec ) ;
			if( nret )
				return nret;
		}
		else if( STRNCMP( sqlaction_line->content , == , "UPDATE" , 6 ) )
		{
			nret = GenerateECCode_ec_SQL_ACTION_UPDATE( pstruct , sqlaction_line->content , sqlaction_line->funcname , fp_dsc_ESQL_ec ) ;
			if( nret )
				return nret;
		}
		else if( STRNCMP( sqlaction_line->content , == , "DELETE" , 6 ) )
		{
			nret = GenerateECCode_ec_SQL_ACTION_DELETE( pstruct , sqlaction_line->content , sqlaction_line->funcname , fp_dsc_ESQL_ec ) ;
			if( nret )
				return nret;
		}
	}
	
	if( pstruct->sqlconn == 1 )
	{
		fprintf( fp_dsc_ESQL_ec , "\n" );
		fprintf( fp_dsc_ESQL_ec , "EXEC SQL BEGIN DECLARE SECTION ;\n" );
		fprintf( fp_dsc_ESQL_ec , "	char    DBNAME[ 1024 + 1 ] ;\n" );
		fprintf( fp_dsc_ESQL_ec , "	char    DBUSER[ 128 + 1 ] ;\n" );
		fprintf( fp_dsc_ESQL_ec , "	char    DBPASS[ 128 + 1 ] ;\n" );
		fprintf( fp_dsc_ESQL_ec , "EXEC SQL END DECLARE SECTION ;\n" );
		
		fprintf( fp_dsc_ESQL_ec , "\n" );
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
			fprintf( fp_dsc_ESQL_ec , "		IDENTIFIED BY	:DBPASS ;\n" );
		}
		fprintf( fp_dsc_ESQL_ec , "	\n" );
		fprintf( fp_dsc_ESQL_ec , "	return;\n" );
		fprintf( fp_dsc_ESQL_ec , "}\n" );
		
		fprintf( fp_dsc_ESQL_ec , "void DSCDBDISCONN()\n" );
		fprintf( fp_dsc_ESQL_ec , "{\n" );
		if( pcp->output_ec_pgsql_flag == 1 )
		{
			fprintf( fp_dsc_ESQL_ec , "	EXEC SQL\n" );
			fprintf( fp_dsc_ESQL_ec , "		DISCONNECT ;\n" );
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

