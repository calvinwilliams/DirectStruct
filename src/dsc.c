#include "dsc.h"

/*
 * dsc - DirectSrtuct compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

int dsc( struct CommandParameter *pcmdparam )
{
	FILE			*fp_dsc = NULL ;
	struct StructInfo	st ;
	int			struct_len ;
	int			offset ;
	
	char			output_pathfilename[ 256 + 1 ] ;
	
	int			nret = 0 ;
	
	fp_dsc = fopen( pcmdparam->pathfilename , "r" ) ;
	if( fp_dsc == NULL )
	{
		fprintf( stderr , "file[%s] can't read\n" , pcmdparam->pathfilename );
		return -1;
	}
	offset = 0 ;
	memset( & st , 0x00 , sizeof(struct StructInfo) );
	struct_len = ReadDscFile( pcmdparam , 0 , & offset , pcmdparam->pathfilename , 0 , fp_dsc , 0 , & st ) ;
	fclose( fp_dsc );
	if( struct_len < 0 )
		return -struct_len;
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
		
		nret = GenerateCCode( pcmdparam , & st , fp_dsc_h , fp_dsc_c , fp_dsc_LOG_c ) ;
		if( nret )
			return -nret;
		
		fclose( fp_dsc_h );
		fclose( fp_dsc_c );
		fclose( fp_dsc_LOG_c );
	}
	
	return 0;
}
