STRUCT	userinfo
{
	INT	4	user_id
	STRING	16	user_name
	STRING	128	email
	
	CREATE_SQL	"CREATE UNIQUE INDEX userinfo_idx1 ON userinfo ( user_id ) ;"
	DROP_SQL	"DROP INDEX testable_idx1 ;"
}
