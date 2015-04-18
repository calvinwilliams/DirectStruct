#include "IDL_testable.dsc.ESQL.eh"

#include "IDL_testable.dsc.LOG.c"

int test_delete()
{
	EXEC SQL
		DELETE
		FROM	testable
		WHERE	n2 IN ( 100 , 101 , 102 ) ;
	if( SQLCODE == 100 )
	{
		printf( "DELETE ok\n" );
	}
	else if( SQLCODE )
	{
		printf( "DELETE failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "DELETE ok\n" );
	}
	
	return 0;
}

int test_insert()
{
	testable	t ;
	
	memset( & t , 0x00 , sizeof(testable) );
	t.n2 = 100 ;
	t.n4 = 56789 ;
	t.f4 = 1.2 ;
	t.f8 = 456.789 ;
	strcpy( t.str32 , "hi" );
	strcpy( t.str1024 , "hello world" );
	t.b1 = DSCFALSE ;
	
	DSCLOG_testable( & t );
	
	DSCINITV_testable();
	DSCSTOV_testable( & t );
	
	EXEC SQL
		INSERT
		INTO	testable
		VALUES	( DBVLIST_testable ) ;
	if( SQLCODE )
	{
		printf( "INSERT failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "INSERT ok\n" );
	}
	
	memset( & t , 0x00 , sizeof(testable) );
	t.n2 = 101 ;
	t.n4 = 56789 ;
	t.f4 = 1.2 ;
	t.f8 = 456.789 ;
	strcpy( t.str32 , "hi" );
	strcpy( t.str1024 , "hello earth" );
	t.b1 = DSCTRUE ;
	
	DSCLOG_testable( & t );
	
	DSCINITV_testable();
	DSCSTOV_testable( & t );
	
	EXEC SQL
		INSERT
		INTO	testable
		VALUES	( DBVLIST_testable ) ;
	if( SQLCODE )
	{
		printf( "INSERT failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "INSERT ok\n" );
	}
	
	memset( & t , 0x00 , sizeof(testable) );
	t.n2 = 102 ;
	t.n4 = 56789 ;
	t.f4 = 1.2 ;
	t.f8 = 456.789 ;
	strcpy( t.str32 , "hi" );
	strcpy( t.str1024 , "hello march" );
	t.b1 = DSCTRUE ;
	
	DSCLOG_testable( & t );
	
	DSCINITV_testable();
	DSCSTOV_testable( & t );
	
	EXEC SQL
		INSERT
		INTO	testable
		VALUES	( DBVLIST_testable ) ;
	if( SQLCODE )
	{
		printf( "INSERT failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "INSERT ok\n" );
	}
	
	return 0;
}

int test_update()
{
	testable	t ;
	
	memset( & t , 0x00 , sizeof(testable) );
	t.n2 = 101 ;
	
	DSCLOG_testable( & t );
	
	DSCINITV_testable();
	DSCSTOV_testable( & t );
	
	EXEC SQL
		SELECT	TFLIST_testable
		INTO	DBVLLIST_testable
		FROM	testable
		WHERE	n2 = :testable_n2 ;
	if( SQLCODE )
	{
		printf( "SELECT failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "SELECT ok\n" );
	}
	
	DSCVTOS_testable( & t );
	
	strcpy( t.str1024 , "hello sun" );
	t.b1 = DSCFALSE ;
	
	DSCLOG_testable( & t );
	
	DSCINITV_testable();
	DSCSTOV_testable( & t );
	
	EXEC SQL
		UPDATE	testable
		SET	str1024 = :testable_str1024
		WHERE	n2 = :testable_n2 ;
	if( SQLCODE )
	{
		printf( "UPDATE failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "UPDATE ok\n" );
	}
	
	return 0;
}

int test_cursor()
{
	testable	t ;
	
	int		nret = 0 ;
	
	EXEC SQL
		DECLARE 	testable_cursor CURSOR FOR
		SELECT		TFLIST_testable
		FROM		testable
		ORDER BY	n2 ASC ;
	if( SQLCODE )
	{
		printf( "DECLARE failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "DECLARE ok\n" );
	}
	
	EXEC SQL
		OPEN	testable_cursor ;
	if( SQLCODE )
	{
		printf( "OPEN failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "OPEN ok\n" );
	}
	
	while(1)
	{
		EXEC SQL
			FETCH	testable_cursor
			INTO	DBVLLIST_testable ;
		if( SQLCODE == 100 )
		{
			break;
		}
		else if( SQLCODE )
		{
			printf( "FETCH failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
			nret = SQLCODE ;
		}
		else
		{
			printf( "FETCH ok\n" );
		}
		
		memset( & t , 0x00 , sizeof(testable) );
		DSCVTOS_testable( & t );
		
		DSCLOG_testable( & t );
	}
	
	EXEC SQL
		CLOSE	testable_cursor ;
	if( SQLCODE )
	{
		printf( "CLOSE failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "CLOSE ok\n" );
	}
	
	if( nret )
		return nret;
	
	return 0;
}

EXEC SQL BEGIN DECLARE SECTION ;
char	sql[] = "DELETE FROM testable WHERE n2 = 102 ;" ;
EXEC SQL END DECLARE SECTION ;

int test_immediate()
{
	EXEC SQL
		EXECUTE IMMEDIATE :sql ;
	if( SQLCODE )
	{
		printf( "EXECUTE failed[%d][%s]\n" , SQLCODE , SQLSTATE );
		return SQLCODE;
	}
	else
	{
		printf( "EXECUTE ok\n" );
	}
	
	return 0;
}

EXEC SQL BEGIN DECLARE SECTION ;
char	sql2[] = "SELECT * FROM testable WHERE n2 >= ? ;" ;
EXEC SQL END DECLARE SECTION ;

int test_prepare()
{
	testable	t ;
	
	int		nret = 0 ;
	
	EXEC SQL
		PREPARE prepare FROM :sql2 ;
	if( SQLCODE )
	{
		printf( "PREPARE failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "PREPARE ok\n" );
	}
	
	testable_n2 = 101 ;
	
	EXEC SQL
		EXECUTE		prepare
		INTO		DBVLLIST_testable
		USING		:testable_n2 ;
	if( SQLCODE )
	{
		printf( "EXECUTE failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "EXECUTE ok\n" );
	}
	
	memset( & t , 0x00 , sizeof(testable) );
	DSCVTOS_testable( & t );
	
	DSCLOG_testable( & t );
	
	testable_n2 = 100 ;
	
	EXEC SQL
		DECLARE testable_cursor2 CURSOR FOR prepare ;
	if( SQLCODE )
	{
		printf( "DECLARE failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "DECLARE ok\n" );
	}
	
	EXEC SQL
		OPEN	testable_cursor2
		USING	:testable_n2 ;
	if( SQLCODE )
	{
		printf( "OPEN failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "OPEN ok\n" );
	}
	
	while(1)
	{
		EXEC SQL
			FETCH	testable_cursor2
			INTO	DBVLLIST_testable ;
		if( SQLCODE == 100 )
		{
			break;
		}
		else if( SQLCODE )
		{
			printf( "FETCH failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
			nret = SQLCODE ;
		}
		else
		{
			printf( "FETCH ok\n" );
		}
		
		memset( & t , 0x00 , sizeof(testable) );
		DSCVTOS_testable( & t );
		
		DSCLOG_testable( & t );
	}
	
	EXEC SQL
		CLOSE	testable_cursor2 ;
	if( SQLCODE )
	{
		printf( "CLOSE failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return SQLCODE;
	}
	else
	{
		printf( "CLOSE ok\n" );
	}
	
	if( nret )
		return nret;
	
	EXEC SQL
		DEALLOCATE PREPARE prepare ;
	
	return 0;
}

int main()
{
	int		nret = 0 ;
	
	EXEC SQL
		CONNECT TO	'calvin@127.0.0.1:18432'
		USER		'calvin'
		IDENTIFIED BY	'calvin' ;
	if( SQLCODE )
	{
		printf( "CONNECT failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
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
		printf( "BEGIN WORK failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return 1;
	}
	else
	{
		printf( "BEGIN WORK ok\n" );
	}
	
	do
	{
		printf( "--- delete ---------\n" );
		nret = test_delete() ;
		if( nret )
			break;
		
		printf( "--- insert ---------\n" );
		nret = test_insert() ;
		if( nret )
			break;
		
		printf( "--- update ---------\n" );
		nret = test_update() ;
		if( nret )
			break;
		
		printf( "--- cursor ---------\n" );
		nret = test_cursor() ;
		if( nret )
			break;
		
		printf( "--- immediate ---------\n" );
		nret = test_immediate() ;
		if( nret )
			break;
		
		printf( "--- prepare ---------\n" );
		nret = test_prepare() ;
		if( nret )
			break;
	}
	while(0);
	if( nret )
	{
		EXEC SQL
			ROLLBACK WORK ;
		if( SQLCODE )
		{
			printf( "ROLLBACK WORK failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
			return 1;
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
			printf( "COMMIT WORK failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
			return 1;
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
		printf( "DISCONNECT failed[%d][%s][%s]\n" , SQLCODE , SQLSTATE , SQLDESC );
		return 1;
	}
	else
	{
		printf( "DISCONNECT ok\n" );
	}
	
	return 0;
}

