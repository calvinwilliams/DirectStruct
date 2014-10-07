#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "IDL_userinfo.dsc.h"
#include "IDL_userinfo.dsc.LOG.c"

int sendn( int sock , char *sendbuffer , int sendlen )
{
	int		l , needlen , sendedlen ;
	
	needlen = sendlen ;
	sendedlen = 0 ;
	
	do
	{
		l = send( sock , sendbuffer + sendedlen , needlen - sendedlen , 0 ) ;
		if( l < 0 )
			return -1;
		
		sendedlen += l;
	}
	while( sendedlen != needlen ) ;
	
	return 0;
}

int test_client()
{
	userinfo		u ;
	char			xml_buffer[ 1024 + 1 ] ;
	int			xml_len ;
	
	int			connect_sock ;
	struct sockaddr_in	connect_addr ;
	
	char			commheader_buffer[ 4 + 1 ] ;
	int			commheader_len ;
	
	int			nret ;
	
	memset( & u , 0x00 , sizeof(userinfo) );
	u.user_id = 101 ;
	strcpy( u.user_name , "calvin" );
	strcpy( u.email , "calvinwilliams.c@gmail.com" );
	
	DSCLOG_userinfo( & u );
	
	memset( xml_buffer , 0x00 , sizeof(xml_buffer) );
	xml_len = sizeof(xml_buffer) ;
	nret = DSCSERIALIZE_XML_userinfo( & u , "GBK" , xml_buffer , & xml_len ) ;
	if( nret )
	{
		printf( "DSCSERIALIZE_XML_userinfo failed[%d] , errno[%d]\n" , nret , errno );
		return -1;
	}
	
	printf( "send[%d][%.*s]\n" , xml_len , xml_len , xml_buffer );
	
	connect_sock = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
	if( connect_sock == -1 )
	{
		printf( "socket failed[%d] , errno[%d]\n" , connect_sock , errno );
		return -1;
	}
	
	memset( & connect_addr , 0x00 , sizeof(struct sockaddr_in) );
	connect_addr.sin_family = AF_INET;
	connect_addr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	connect_addr.sin_port = htons( (unsigned short)7878 );
	
	nret = connect( connect_sock , (struct sockaddr *) & connect_addr, sizeof(struct sockaddr) );
	if( nret == -1 )
	{
		printf( "connect failed[%d] , errno[%d]\n" , nret , errno );
		return -1;
	}
	
	sprintf( commheader_buffer , "%04d" , xml_len );
	commheader_len = 4 ;
	nret = sendn( connect_sock , commheader_buffer , commheader_len ) ;
	if( nret < 0 )
	{
		printf( "sendn failed[%d] , errno[%d]\n" , nret , errno );
		return -1;
	}
	
	nret = sendn( connect_sock , xml_buffer , xml_len ) ;
	if( nret < 0 )
	{
		printf( "sendn failed[%d] , errno[%d]\n" , nret , errno );
		return -1;
	}
	
	close( connect_sock );
	
	return 0;
}

int main()
{
	return -test_client();
}

