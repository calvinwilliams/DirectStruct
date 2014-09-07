#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include <string>
using namespace std;
#include "PressProtobuf.pb.h"

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

int test_protobuf_press( int count )
{
	PressProtobuf	press ;
	PressProtobuf	press2 ;
	string		str ;
	int		len ;
	int		c ;
	
	press.set_n4( -0x11223344 );
	press.set_n8( -0x1122334455667788LL );
	press.set_un4( 0x11223344 );
	press.set_un8( 0x1122334455667788LL );
	press.set_f4( 100.00 );
	press.set_f8( 10000.00 );
	press.set_str32( "calvin" );
	press.set_str1024( "XXXXXXXXXXXXXXXX" );
	
	printf( "[%d]\n" , press.n4() );
	printf( "[%lld]\n" , press.n8() );
	printf( "[%d]\n" , press.un4() );
	printf( "[%lld]\n" , press.un8() );
	printf( "[%f]\n" , press.f4() );
	printf( "[%lf]\n" , press.f8() );
	printf( "[%s]\n" , press.str32().c_str() );
	printf( "[%s]\n" , press.str1024().c_str() );
	
	for( c = 0 ; c < count ; c++ )
	{
		str = press.SerializeAsString() ;
		press2.ParseFromString( str );
	}
	
	len = str.length() ;
	printf( "[%d]\n" , len );
	dump( (char*)str.c_str() , len );
	
	printf( "[%d]\n" , press2.n4() );
	printf( "[%lld]\n" , press2.n8() );
	printf( "[%d]\n" , press2.un4() );
	printf( "[%lld]\n" , press2.un8() );
	printf( "[%f]\n" , press2.f4() );
	printf( "[%lf]\n" , press2.f8() );
	printf( "[%s]\n" , press2.str32().c_str() );
	printf( "[%s]\n" , press2.str1024().c_str() );
	
	return 0;
}

static void usage()
{
	printf( "USAGE : test_protobuf_press count\n" );
	return;
}

int main( int argc , char *argv[] )
{
	if( argc == 1 + 1 )
	{
		return -test_protobuf_press( atoi(argv[1]) );
	}
	else
	{
		usage();
		exit(7);
	}
}

