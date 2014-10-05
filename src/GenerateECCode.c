#include "dsc.h"

/*
 * dsc - DirectSrtuct compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

static int GenerateECCode_h_SQL( struct CommandParameter *pcp , struct StructInfo *psi , FILE *fp_dsc_ESQL_eh )
{
	struct FieldInfo	*pfi = NULL ;
	
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "EXEC SQL BEGIN DECLARE SECTION ;\n" );
	for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
	{
		if( STRCMP( pfi->field_type , == , "INT" ) )
		{
			if( pfi->field_len == 1 )
			{
				fprintf( fp_dsc_ESQL_eh , "	extern char %s_%s ;	extern short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 2 )
			{
				fprintf( fp_dsc_ESQL_eh , "	extern short %s_%s ;	extern short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 4 )
			{
				fprintf( fp_dsc_ESQL_eh , "	extern int %s_%s ;	extern short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 8 )
			{
				fprintf( fp_dsc_ESQL_eh , "	extern longlong %s_%s ;	extern short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
			}
		}
		else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
		{
			if( pfi->field_len == 4 )
			{
				fprintf( fp_dsc_ESQL_eh , "	extern float %s_%s ;	extern short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 8 )
			{
				fprintf( fp_dsc_ESQL_eh , "	extern double %s_%s ;	extern short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
			}
		}
		else if( STRCMP( pfi->field_type , == , "STRING" ) )
		{
			fprintf( fp_dsc_ESQL_eh , "	extern char %s_%s[ %d + 1 ] ;	extern short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , pfi->field_len , psi->struct_name , pfi->field_name );
		}
	}
	fprintf( fp_dsc_ESQL_eh , "EXEC SQL END DECLARE SECTION ;\n" );
	
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#define	TFLIST_%s \\\n" , psi->struct_name );
	for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
	{
		fprintf( fp_dsc_ESQL_eh , "	%s" , pfi->field_name );
		
		if( pfi->next_field )
		{
			fprintf( fp_dsc_ESQL_eh , " , \\\n" );
		}
		else
		{
			fprintf( fp_dsc_ESQL_eh , "\n" );
		}
	}
	
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#define	DBVLIST_%s \\\n" , psi->struct_name );
	for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
	{
		fprintf( fp_dsc_ESQL_eh , "	:%s_%s" , psi->struct_name , pfi->field_name );
		
		if( pfi->next_field )
		{
			fprintf( fp_dsc_ESQL_eh , " , \\\n" );
		}
		else
		{
			fprintf( fp_dsc_ESQL_eh , "\n" );
		}
	}
	
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#define	DBVLLIST_%s \\\n" , psi->struct_name );
	for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
	{
		fprintf( fp_dsc_ESQL_eh , "	:%s_%s :%s_%s_id" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
		
		if( pfi->next_field )
		{
			fprintf( fp_dsc_ESQL_eh , " , \\\n" );
		}
		else
		{
			fprintf( fp_dsc_ESQL_eh , "\n" );
		}
	}
	
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "void DSCINITV_%s();\n" , psi->struct_name );
	fprintf( fp_dsc_ESQL_eh , "void DSCVTOS_%s( %s *pst );\n" , psi->struct_name , psi->struct_name );
	fprintf( fp_dsc_ESQL_eh , "void DSCSTOV_%s( %s *pst );\n" , psi->struct_name , psi->struct_name );
	fprintf( fp_dsc_ESQL_eh , "void DSCTRIM_%s( %s *pst );\n" , psi->struct_name , psi->struct_name );
	
	return 0;
}

static int GenerateECCode_ec_SQL( struct CommandParameter *pcp , struct StructInfo *psi , FILE *fp_dsc_ec )
{
	struct FieldInfo	*pfi = NULL ;
	
	fprintf( fp_dsc_ec , "\n" );
	fprintf( fp_dsc_ec , "EXEC SQL BEGIN DECLARE SECTION ;\n" );
	for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
	{
		if( STRCMP( pfi->field_type , == , "INT" ) )
		{
			if( pfi->field_len == 1 )
			{
				fprintf( fp_dsc_ec , "	char %s_%s ;	short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 2 )
			{
				fprintf( fp_dsc_ec , "	short %s_%s ;	short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 4 )
			{
				fprintf( fp_dsc_ec , "	int %s_%s ;	short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 8 )
			{
				fprintf( fp_dsc_ec , "	longlong %s_%s ;	short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
			}
		}
		else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
		{
			if( pfi->field_len == 4 )
			{
				fprintf( fp_dsc_ec , "	float %s_%s ;	short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 8 )
			{
				fprintf( fp_dsc_ec , "	double %s_%s ;	short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
			}
		}
		else if( STRCMP( pfi->field_type , == , "STRING" ) )
		{
			fprintf( fp_dsc_ec , "	char %s_%s[ %d + 1 ] ;	short %s_%s_id ;\n" , psi->struct_name , pfi->field_name , pfi->field_len , psi->struct_name , pfi->field_name );
		}
	}
	fprintf( fp_dsc_ec , "EXEC SQL END DECLARE SECTION ;\n" );
	
	fprintf( fp_dsc_ec , "\n" );
	fprintf( fp_dsc_ec , "void DSCINITV_%s()\n" , psi->struct_name );
	fprintf( fp_dsc_ec , "{\n" );
	for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
	{
		if( STRCMP( pfi->field_type , == , "INT" ) )
		{
			if( pfi->field_len == 1 )
			{
				fprintf( fp_dsc_ec , "	%s_%s = 0 ;\n" , psi->struct_name , pfi->field_name );
				fprintf( fp_dsc_ec , "	%s_%s_id = 0 ;\n" , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 2 )
			{
				fprintf( fp_dsc_ec , "	%s_%s = 0 ;\n" , psi->struct_name , pfi->field_name );
				fprintf( fp_dsc_ec , "	%s_%s_id = 0 ;\n" , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 4 )
			{
				fprintf( fp_dsc_ec , "	%s_%s = 0 ;\n" , psi->struct_name , pfi->field_name );
				fprintf( fp_dsc_ec , "	%s_%s_id = 0 ;\n" , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 8 )
			{
				fprintf( fp_dsc_ec , "	%s_%s = 0 ;\n" , psi->struct_name , pfi->field_name );
				fprintf( fp_dsc_ec , "	%s_%s_id = 0 ;\n" , psi->struct_name , pfi->field_name );
			}
		}
		else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
		{
			if( pfi->field_len == 4 )
			{
				fprintf( fp_dsc_ec , "	%s_%s = 0.0 ;\n" , psi->struct_name , pfi->field_name );
				fprintf( fp_dsc_ec , "	%s_%s_id = 0 ;\n" , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 8 )
			{
				fprintf( fp_dsc_ec , "	%s_%s = 0.00 ;\n" , psi->struct_name , pfi->field_name );
				fprintf( fp_dsc_ec , "	%s_%s_id = 0 ;\n" , psi->struct_name , pfi->field_name );
			}
		}
		else if( STRCMP( pfi->field_type , == , "STRING" ) )
		{
			fprintf( fp_dsc_ec , "	memset( %s_%s , 0x00 , sizeof(%s_%s) );\n" , psi->struct_name , pfi->field_name , psi->struct_name , pfi->field_name );
			fprintf( fp_dsc_ec , "	%s_%s_id = 0 ;\n" , psi->struct_name , pfi->field_name );
		}
	}
	fprintf( fp_dsc_ec , "	return;\n" );
	fprintf( fp_dsc_ec , "}\n" );
	
	fprintf( fp_dsc_ec , "\n" );
	fprintf( fp_dsc_ec , "void DSCVTOS_%s( %s *pst )\n" , psi->struct_name , psi->struct_name );
	fprintf( fp_dsc_ec , "{\n" );
	for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
	{
		if( STRCMP( pfi->field_type , == , "INT" ) )
		{
			if( pfi->field_len == 1 )
			{
				fprintf( fp_dsc_ec , "	pst->%s = %s_%s ;\n" , pfi->field_name , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 2 )
			{
				fprintf( fp_dsc_ec , "	pst->%s = %s_%s ;\n" , pfi->field_name , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 4 )
			{
				fprintf( fp_dsc_ec , "	pst->%s = %s_%s ;\n" , pfi->field_name , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 8 )
			{
				fprintf( fp_dsc_ec , "	pst->%s = %s_%s ;\n" , pfi->field_name , psi->struct_name , pfi->field_name );
			}
		}
		else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
		{
			if( pfi->field_len == 4 )
			{
				fprintf( fp_dsc_ec , "	pst->%s = %s_%s ;\n" , pfi->field_name , psi->struct_name , pfi->field_name );
			}
			else if( pfi->field_len == 8 )
			{
				fprintf( fp_dsc_ec , "	pst->%s = %s_%s ;\n" , pfi->field_name , psi->struct_name , pfi->field_name );
			}
		}
		else if( STRCMP( pfi->field_type , == , "STRING" ) )
		{
			fprintf( fp_dsc_ec , "	strcpy( pst->%s , %s_%s );\n" , pfi->field_name , psi->struct_name , pfi->field_name );
		}
	}
	fprintf( fp_dsc_ec , "	return;\n" );
	fprintf( fp_dsc_ec , "}\n" );
	
	fprintf( fp_dsc_ec , "\n" );
	fprintf( fp_dsc_ec , "void DSCSTOV_%s( %s *pst )\n" , psi->struct_name , psi->struct_name );
	fprintf( fp_dsc_ec , "{\n" );
	for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
	{
		if( STRCMP( pfi->field_type , == , "INT" ) )
		{
			if( pfi->field_len == 1 )
			{
				fprintf( fp_dsc_ec , "	%s_%s = pst->%s ;\n" , psi->struct_name , pfi->field_name , pfi->field_name );
			}
			else if( pfi->field_len == 2 )
			{
				fprintf( fp_dsc_ec , "	%s_%s = pst->%s ;\n" , psi->struct_name , pfi->field_name , pfi->field_name );
			}
			else if( pfi->field_len == 4 )
			{
				fprintf( fp_dsc_ec , "	%s_%s = pst->%s ;\n" , psi->struct_name , pfi->field_name , pfi->field_name );
			}
			else if( pfi->field_len == 8 )
			{
				fprintf( fp_dsc_ec , "	%s_%s = pst->%s ;\n" , psi->struct_name , pfi->field_name , pfi->field_name );
			}
		}
		else if( STRCMP( pfi->field_type , == , "FLOAT" ) )
		{
			if( pfi->field_len == 4 )
			{
				fprintf( fp_dsc_ec , "	%s_%s = pst->%s ;\n" , psi->struct_name , pfi->field_name , pfi->field_name );
			}
			else if( pfi->field_len == 8 )
			{
				fprintf( fp_dsc_ec , "	%s_%s = pst->%s ;\n" , psi->struct_name , pfi->field_name , pfi->field_name );
			}
		}
		else if( STRCMP( pfi->field_type , == , "STRING" ) )
		{
			fprintf( fp_dsc_ec , "	strcpy( %s_%s , pst->%s );\n" , psi->struct_name , pfi->field_name , pfi->field_name );
		}
	}
	fprintf( fp_dsc_ec , "	return;\n" );
	fprintf( fp_dsc_ec , "}\n" );
	
	fprintf( fp_dsc_ec , "\n" );
	fprintf( fp_dsc_ec , "#ifndef TRIM\n" );
	fprintf( fp_dsc_ec , "#define TRIM(_str_) {char *p=(_str_)+strlen(_str_)-1; for(;((*p)==' ')&&(p>=(_str_));p--){(*p)='\\0';}}\n" );
	fprintf( fp_dsc_ec , "#endif\n" );
	fprintf( fp_dsc_ec , "\n" );
	fprintf( fp_dsc_ec , "void DSCTRIM_%s( %s *pst )\n" , psi->struct_name , psi->struct_name );
	fprintf( fp_dsc_ec , "{\n" );
	for( pfi = psi->field_list ; pfi ; pfi = pfi->next_field )
	{
		if( STRCMP( pfi->field_type , == , "STRING" ) )
		{
			fprintf( fp_dsc_ec , "	TRIM( pst->%s )\n" , pfi->field_name );
		}
	}
	fprintf( fp_dsc_ec , "	return;\n" );
	fprintf( fp_dsc_ec , "}\n" );
	
	return 0;
}

int GenerateCCode_h( struct CommandParameter *pcp , int depth , struct StructInfo *first_pmsginfo , FILE *fp_dsc_h );

int GenerateECCode_PQSQL( struct CommandParameter *pcp , struct StructInfo *psi , FILE *fp_dsc_ESQL_eh , FILE *fp_dsc_ESQL_ec )
{
	int		nret = 0 ;
	
	/* Generate *.dsc.ESQL.eh */
	fprintf( fp_dsc_ESQL_eh , "/* It had generated by DirectStruct v%s */\n" , DIRECTSTRUCT_VERSION );
	fprintf( fp_dsc_ESQL_eh , "#ifndef _H_%s_ESQL_\n" , psi->next_struct->struct_name );
	fprintf( fp_dsc_ESQL_eh , "#define _H_%s_ESQL_\n" , psi->next_struct->struct_name );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#include <ecpglib.h>\n" );
	fprintf( fp_dsc_ESQL_eh , "#include <ecpgerrno.h>\n" );
	fprintf( fp_dsc_ESQL_eh , "#include <sqlca.h>\n" );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#include \"%s.h\"\n" , pcp->pathfilename );
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#ifndef SQLCODE\n" );
	fprintf( fp_dsc_ESQL_eh , "#define SQLCODE		sqlca.sqlcode\n" );
	fprintf( fp_dsc_ESQL_eh , "#define SQLSTATE	sqlca.sqlstate\n" );
	fprintf( fp_dsc_ESQL_eh , "#endif\n" );
	nret = GenerateECCode_h_SQL( pcp , psi->next_struct , fp_dsc_ESQL_eh ) ;
	if( nret )
		return nret;
	fprintf( fp_dsc_ESQL_eh , "\n" );
	fprintf( fp_dsc_ESQL_eh , "#endif\n" );
	
	/* Generate *.dsc.ESQL.ec */
	fprintf( fp_dsc_ESQL_ec , "/* It had generated by DirectStruct v%s */\n" , DIRECTSTRUCT_VERSION );
	fprintf( fp_dsc_ESQL_ec , "\n" );
	fprintf( fp_dsc_ESQL_ec , "#include \"%s.ESQL.eh\"\n" , pcp->pathfilename );
	nret = GenerateECCode_ec_SQL( pcp , psi->next_struct , fp_dsc_ESQL_ec ) ;
	if( nret )
		return nret;
	
	return 0;
}

