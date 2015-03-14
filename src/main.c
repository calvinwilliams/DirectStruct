#include "dsc.h"

/*
 * dsc - DirectSrtuct compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

static void version()
{
	printf( "dsc v%s - DirectSrtuct Compiler\n" , __DIRECTSTRUCT_VERSION );
	printf( "Copyright by calvin<calvinwilliams.c@gmail.com> 2014\n" );
}

static void usage()
{
	printf( "USAGE : dsc -f .dsc [ -c ] [ -c-LOG ]\n" );
	printf( "                    [ -c-order ]\n" );
	printf( "                    [ -c-compact ] [ -c-compress ]\n" );
	printf( "                    [ -c-xml | -c-xml-compact ]\n" );
	printf( "                    [ -c-json | -c-json-compact ]\n" );
	printf( "                    [ -sql-pgsql | -ec-pgsql ] [ -ec-pgsql | -ec-oracle ]\n" );
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
