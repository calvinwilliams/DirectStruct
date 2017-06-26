STRUCT	userinfo
{
	INT	4	user_id		DEFAULT	1	NOTNULL
	STRING	16	user_name	DEFAULT	'0000'
	STRING	128	email				NULL
	
	CREATE_SQL	"CREATE UNIQUE INDEX userinfo_idx1 ON userinfo ( user_id ) ;"
	DROP_SQL	"DROP INDEX testable_idx1 ;"
	
	SQLCONN
	
	SQLACTION	"SELECT user_name,email FROM userinfo WHERE user_id ="
	SQLACTION	"SELECT * FROM userinfo WHERE user_id = AND user_name ="
	SQLACTION	"INSERT INTO userinfo"
	SQLACTION	"UPDATE userinfo SET * WHERE user_id ="
	SQLACTION	"UPDATE userinfo SET email"
	SQLACTION	"DELETE FROM userinfo WHERE user_id >="
	SQLACTION	"DELETE FROM userinfo"
	SQLACTION	"CURSOR mycursor SELECT * FROM userinfo"
	SQLACTION	"CURSOR mycursor1 SELECT user_name FROM userinfo WHERE user_id >="
	SQLACTION	"CURSOR mycursor2 SELECT user_name,email FROM userinfo ORDER BY user_id ASC"
	SQLACTION	"CURSOR mycursor3 SELECT * FROM userinfo WHERE user_id >= ORDER BY user_id ASC"
	
	SQLCONN		db_master
	SQLCONN		db_slave
	
	SQLACTION	"SELECT * FROM userinfo WHERE user_id = AND user_name =" db_slave
	SQLACTION	"INSERT INTO userinfo" db_master
	SQLACTION	"UPDATE userinfo SET * WHERE user_id =" db_master
	SQLACTION	"DELETE FROM userinfo WHERE user_id >=" db_master
	SQLACTION	"CURSOR mycursor11 SELECT user_name FROM userinfo WHERE user_id >=" db_slave
}
