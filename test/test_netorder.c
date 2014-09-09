#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "IDL_AllTypes.dsc.h"
#include "IDL_AllTypes.dsc.LOG.c"

static void dump( char *buf , int buflen )
{
	int		i , j ;
	
	for( i = 0 ; i < buflen / 16 + 1 ; i++ )
	{
		printf( "0x%010X" , i );
		
		printf( " " );
		
		for( j = 0 ; j < 16 ; j++ )
		{
			if( i*16+j<buflen )
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
			if( i*16+j<buflen )
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

int test_netorder()
{
	AllTypes	st ;
	int		nret = 0 ;
	
	/* client code */
	memset( & st , 0x00 , sizeof(AllTypes) );
	
	st.n1 = -0x11 ;
	st.n2 = -0x1122 ;
	st.n4 = -0x11223344 ;
	st.n8 = LL(-0x1122334455667788) ;
	st.u1 = 0x11 ;
	st.u2 = 0x1122 ;
	st.u4 = 0x11223344 ;
	st.u8 = ULL(0x1122334455667788) ;
	st.f4 = 100.00 ;
	st.f8 = 10000.00 ;
	st.ch = 'A' ;
	st.uch = 'B' ;
	strcpy( st.str32 , "calvin" );
	strcpy( st.str1024 , "XXXXXXXXXXXXXXXX" );
	
	DSCLOG_AllTypes( & st );
	
	nret = DSCNETORDER_AllTypes( & st ) ;
	if( nret )
	{
		printf( "DSCNETORDER_AllTypes failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCNETORDER_AllTypes ok\n" );
	}
	
	dump( (char *) & st , sizeof(st) );
	
	/* ... client send communication struct to server ... */
	
	/* server code */
	nret = DSCHOSTORDER_AllTypes( & st ) ;
	if( nret )
	{
		printf( "DSCHOSTORDER_AllTypes failed[%d]\n" , nret );
		return nret;
	}
	else
	{
		printf( "DSCHOSTORDER_AllTypes ok\n" );
	}
	
	DSCLOG_AllTypes( & st );
	
	return 0;
}

int main()
{
	return -test_netorder();
}
