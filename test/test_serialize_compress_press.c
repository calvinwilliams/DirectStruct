#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "IDL_AllTypes2.dsc.h"
#include "IDL_AllTypes2.dsc.LOG.c"

static void dump( char *buf , int len )
{
	int		i , j ;
	
	for( i = 0 ; i < len / 16 + 1 ; i++ )
	{
		printf( "0x%010X" , i );
		
		printf( " " );
		
		for( j = 0 ; j < 16 ; j++ )
		{
			if( i*16+j<len )
			{
				printf( " %02x" , (unsigned char)*(buf+i*16+j) );
			}
			else
			{
				printf( "   " );
			}
		}
		
		printf( " " );
		
		for( j = 0 ; j < 16 ; j++ )
		{
			if( i*16+j<len )
			{
				if( isprint(*(buf+i*16+j)) )
					putchar( *(buf+i*16+j) );
				else
					putchar( '.' );
			}
			else
			{
				putchar( ' ' );
			}
		}
		
		putchar( '\n' );
	}
	
	return;
}

int test_serialize_compress_press( long count )
{
	AllTypes2	st ;
	char		buf[ DSCSERIALIZE_COMPRESS_BUFSIZE_AllTypes2 + 1 ] ;
	int		len ;
	
	long		c ;
	long		t1 , t2 , dt ;
	
	int		nret = 0 ;
	
	memset( & st , 0x00 , sizeof(AllTypes2) );
	
	st.n4 = -0x11223344 ;
	st.n8 = LL(-0x1122334455667788) ;
	st.u4 = 0x11223344 ;
	st.u8 = ULL(0x1122334455667788) ;
	st.f4 = 100.00 ;
	st.f8 = 10000.00 ;
	strcpy( st.str32 , "calvin" );
	strcpy( st.str1024 , "XXXXXXXXXXXXXXXX" );
	
	DSCLOG_AllTypes2( & st );
	
	printf( "Press begin\n" );
	time( & t1 );
	
	for( c = 0 ; c < count ; c++ )
	{
		memset( buf , 0x00 , sizeof(buf) );
		nret = DSCSERIALIZE_COMPRESS_AllTypes2( & st , buf , & len ) ;
		if( nret )
		{
			printf( "DSCSERIALIZE_COMPRESS_AllTypes2 failed[%d]\n" , nret );
			return nret;
		}
		
		memset( & st , 0x00 , sizeof(AllTypes2) );
		nret = DSCDESERIALIZE_COMPRESS_AllTypes2( buf , & len , & st ) ;
		if( nret )
		{
			printf( "DSCDESERIALIZE_COMPRESS_AllTypes2 failed[%d]\n" , nret );
			return nret;
		}
	}
	
	printf( "Press end\n" );
	time( & t2 );
	dt = t2 - t1 ;
	printf( "data compress size[%d] - compact size[%d]\n" , len , DSCSERIALIZE_COMPACT_BUFSIZE_AllTypes2 );
	printf( "Elapse %ld seconds\n" , dt );
	
	dump( buf , len );
	
	DSCLOG_AllTypes2( & st );
	
	return 0;
}

void usage()
{
	printf( "USAGE : test_serialize_compress_press count\n" );
	return;
}

int main( int argc , char *argv[] )
{
	if( argc == 1 + 1 )
	{
		return -test_serialize_compress_press( atol(argv[1]) );
	}
	else
	{
		usage();
		exit(7);
	}
}
