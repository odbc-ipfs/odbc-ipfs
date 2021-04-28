#pragma once

#ifdef CORELIBRARY_EXPORTS
#define CORELIBRARY_API __declspec(dllexport)
#else
#define CORELIBRARY_API __declsped(dllexport)
#endif

#include <Odbcinst.h>
#include <sql.h>
#include <Sqlext.h>
#include <Sqltypes.h>
#include <Sqlucode.h>
#include <Msdasql.h>
#include <Msdadc.h>



typedef struct {
	SQLSMALLINT type;	/**< ODBC type */
	SQLINTEGER BufferLength;	/**< Max. size of value buffer */
	SQLLEN* StrLen_or_Ind;	/**< Value return, actual size of value buffer */
	SQLPOINTER TargetValueptr;	/**< Value buffer */
	int index;		/**< Index of column in result */
	int offs;		/**< Byte offset for SQLGetData() */ // Not sure we need this
	//int columnNumber /** Column number of the newly bound column **/
} BINDCOL;
/*Driver internal structure for environment (HENV).

https://docs.microsoft.com/en-us/sql/odbc/reference/develop-app/environment-handles?view=sql-server-ver15
Things suggested by ms:

	The environment's state

	The current environment-level diagnostics

	The handles of connections currently allocated on the environment

	The current settings of each environment attribute
*/

typedef struct {
	int version;
} ENV;

/*Driver internal structure for database connection (HDBC).

https://docs.microsoft.com/en-us/sql/odbc/reference/develop-app/connection-handles?view=sql-server-ver15
Things suggested by ms:

	The state of the connection

	The current connection-level diagnostics

	The handles of statements and descriptors currently allocated on the connection

	The current settings of each connection attribute*/
typedef struct {
	ENV* env;
	void* handle;
	int temp;
	SQLCHAR* serverName;
	SQLSMALLINT serverLength;
	SQLCHAR* userName;
	SQLSMALLINT userLength;
	SQLCHAR* auth;
	SQLSMALLINT authLength;
} DBC;

/*Driver internal structure representing SQL statement (HSTMT).

https://docs.microsoft.com/en-us/sql/odbc/reference/develop-app/statement-handles?view=sql-server-ver15
Things suggested by ms:

	The statement's state

	The current statement-level diagnostics

	The addresses of the application variables bound to the statement's parameters and result set columns

	The current settings of each statement attribute
*/
typedef struct {
	DBC* dbc;
	int temp;
	int argc; //the number of columns in the result
	char** argv; //an array of pointers to strings obtained
	char** colName; // an array of pointers to strings where each entry represents the name of corresponding result column as obtained
	BINDCOL* bindcols;		/**< Array of bound columns */
	int nbindcols;		/**< Number of entries in bindcols */
	//int* boundcolumns  // The column numbers for the bound columns
} STMT;




