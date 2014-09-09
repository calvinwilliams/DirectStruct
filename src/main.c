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
	printf( "dsc v%s - DirectSrtuct Compiler\n" , DIRECTSTRUCT_VERSION );
	printf( "Copyright by calvin<calvinwilliams.c@gmail.com> 2014\n" );
}

static void usage()
{
	printf( "USAGE : dsc -f .dsc [ -c ]\n" );
}

int main( int argc , char *argv[] )
{
	int			c ;
	struct CommandParameter	cmdparam ;
	
	if( argc == 1 )
	{
		version();
		usage();
		exit(0);
	}
	
	memset( & cmdparam , 0x00 , sizeof(struct CommandParameter) );
	for( c = 1 ; c < argc ; c++ )
	{
		if( strcmp( argv[c] , "-f" ) == 0 && c + 1 < argc )
		{
			c++;
			cmdparam.pathfilename = argv[c] ;
		}
		else if( strcmp( argv[c] , "-c" ) == 0 )
		{
			cmdparam.output_c_flag = 1 ;
		}
		else
		{
			usage();
			exit(7);
		}
	}
	
	if( cmdparam.pathfilename == NULL )
	{
		usage();
		exit(7);
	}
	
	return -dsc( & cmdparam ) ;
}
