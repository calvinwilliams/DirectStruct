#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "IDL_userinfo.dsc.h"
#include "IDL_userinfo.dsc.ESQL.eh"
#include "IDL_userinfo.dsc.LOG.c"

int recvn( int sock , char *recvbuffer , int *p_recvlen )
{
	int		l , needlen , readedlen ;
	
	needlen = (*p_recvlen) ;
	readedlen = 0 ;
	
	while( readedlen < needlen )
	{
		l = recv( sock , recvbuffer + readedlen , needlen - readedlen , 0 ) ;
		if( l == 0 )
		{
			(*p_recvlen) = readedlen ;
			
			return -2;
		}
		else if( l < 0 )
		{
			(*p_recvlen) = readedlen ;
			return -1;
		}
		
		readedlen += l ;
	}
	
	(*p_recvlen) = readedlen ;
	
	return 0;
}

int test_server()
{
	int			listen_sock ;
	struct sockaddr_in	listen_addr ;
	int			on ;
	int			accept_sock ;
	struct sockaddr_in	accept_addr ;
	socklen_t		addrlen ;
	
	char			commheader_buffer[ 4 + 1 ] ;
	int			commheader_len ;
	char			xml_buffer[ 1024 + 1 ] ;
	int			xml_len ;
	userinfo		u ;
	
	int			nret ;
	
	listen_sock = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
	if( listen_sock == -1 )
	{
		printf( "socket failed[%d] , errno[%d]\n" , listen_sock , errno );
		return -1;
	}
	
	on = 1 ;
	setsockopt( listen_sock , SOL_SOCKET , SO_REUSEADDR , (void *) & on , sizeof(on) );
	
	memset( & listen_addr , 0x00 , sizeof(struct sockaddr_in) );
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	listen_addr.sin_port = htons( (unsigned short)7878 );
	
	nret = bind( listen_sock , (struct sockaddr *) & listen_addr, sizeof(struct sockaddr) ) ;
	if( nret == -1 )
	{
		printf( "bind failed[%d] , errno[%d]\n" , nret , errno );
		return -1;
	}
	
	nret = listen( listen_sock , 1024 ) ;
	if( nret == -1 )
	{
		printf( "listen failed[%d] , errno[%d]\n" , nret , errno );
		return -1;
	}
	
	addrlen = sizeof(struct sockaddr) ;
	accept_sock = accept( listen_sock , (struct sockaddr *) & accept_addr, & addrlen );
	if( accept_sock == -1 )
	{
		printf( "accept failed[%d] , errno[%d]\n" , accept_sock , errno );
		return -1;
	}
	
	memset( commheader_buffer , 0x00 , sizeof(commheader_buffer) );
	commheader_len = 4 ;
	nret = recvn( accept_sock , commheader_buffer , & commheader_len ) ;
	if( nret == -1 )
	{
		printf( "recvn failed[%d] , errno[%d]\n" , nret , errno );
		return -1;
	}
	
	xml_len = atoi(commheader_buffer) ;
	memset( xml_buffer , 0x00 , sizeof(xml_buffer) );
	nret = recvn( accept_sock , xml_buffer , & xml_len ) ;
	if( nret == -1 )
	{
		printf( "recvn failed[%d] , errno[%d]\n" , nret , errno );
		return -1;
	}
	
	close( accept_sock );
	close( listen_sock );
	
	printf( "recv[%d][%.*s]\n" , xml_len , xml_len , xml_buffer );
	
	memset( & u , 0x00 , sizeof(userinfo) );
	nret = DSCDESERIALIZE_XML_userinfo( "GBK" , xml_buffer , & xml_len , & u ) ;
	if( nret )
	{
		printf( "DSCDESERIALIZE_XML_userinfo failed[%d] , errno[%d]\n" , nret , errno );
		return -1;
	}
	
	DSCLOG_userinfo( & u );
	
	EXEC SQL
		CONNECT TO	'calvin@127.0.0.1:18432'
		USER		'calvin'
		IDENTIFIED BY	'calvin' ;
	if( SQLCODE )
	{
		printf( "CONNECT failed[%ld][%s]\n" , SQLCODE , SQLSTATE );
		return 1;
	}
	else
	{
		printf( "CONNECT ok\n" );
	}
	
	EXEC SQL
		BEGIN WORK ;
	if( SQLCODE )
	{
		printf( "BEGIN WORK failed[%ld][%s]\n" , SQLCODE , SQLSTATE );
		return 1;
	}
	else
	{
		printf( "BEGIN WORK ok\n" );
	}
	
	DSCINITV_userinfo();
	DSCSTOV_userinfo( & u );
	
	EXEC SQL
		INSERT
		INTO	userinfo
		VALUES	( DBVLIST_userinfo ) ;
	if( SQLCODE )
	{
		printf( "INSERT failed[%ld][%s]\n" , SQLCODE , SQLSTATE );
	}
	else
	{
		printf( "INSERT ok\n" );
	}
	
	if( SQLCODE )
	{
		EXEC SQL
			ROLLBACK WORK ;
		if( SQLCODE )
		{
			printf( "ROLLBACK WORK failed[%ld][%s]\n" , SQLCODE , SQLSTATE );
		}
		else
		{
			printf( "ROLLBACK WORK ok\n" );
		}
	}
	else
	{
		EXEC SQL
			COMMIT WORK ;
		if( SQLCODE )
		{
			printf( "COMMIT WORK failed[%ld][%s]\n" , SQLCODE , SQLSTATE );
		}
		else
		{
			printf( "COMMIT WORK ok\n" );
		}
	}
	
	EXEC SQL
		DISCONNECT ;
	if( SQLCODE )
	{
		printf( "DISCONNECT failed[%ld][%s]\n" , SQLCODE , SQLSTATE );
		return 1;
	}
	else
	{
		printf( "DISCONNECT ok\n" );
	}
	
	return 0;
}

int main()
{
	return -test_server();
}

