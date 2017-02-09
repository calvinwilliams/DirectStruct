STRUCT	testable
{
	INT	2	n2
	INT	4	n4
	FLOAT	4	f4
	FLOAT	8	f8
	NUMERIC	12,6	n8
	AMOUNT	16	a8
	STRING	32	str32
	STRING	1024	str1024
	BOOL	1	b1
	
	CREATE_SQL	"CREATE INDEX testable_idx1 ON testable ( n2 ) ;"
	CREATE_SQL	"CREATE UNIQUE INDEX testable_idx2 ON testable ( str32 , str1024 ) ;"
	DROP_SQL	"DROP INDEX testable_idx1 ;"
	DROP_SQL	"DROP INDEX testable_idx2 ;"
}
