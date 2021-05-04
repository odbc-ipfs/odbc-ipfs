// SampleODBCApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SampleODBCApplication.h"
#include <wchar.h>


//NOTE: make sure you are building for the right system
// x86 or x64
// x86 applications will NOT find x64 drivers and vice versa
int main()
{
    SQLHDBC dbc;
    SQLHENV env;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);


    dbc = conn(&env);


    getData(dbc);

    SQLDisconnect(dbc);

    

    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);

}

SQLHDBC conn(SQLHENV *env) {
    SQLHDBC dbc;

    SQLWCHAR str_ret[1024];
    SQLSMALLINT str_ret_len;


    wchar_t str_dsn[] = L"FILEDSN=C:/Program Files/Common Files/Odbc/Data Sources/odbc-ipfs.dsn";

    SQLAllocHandle(SQL_HANDLE_DBC, *env, &dbc);

    SQLRETURN ret = SQLDriverConnect(dbc, NULL, str_dsn, SQL_NTS, str_ret, sizeof(4096), &str_ret_len, SQL_DRIVER_COMPLETE);

    

    if (ret == SQL_SUCCESS) {
        std::cout << "Connected to database\n\n";
    }
    else {
        std::cout << "Failed to connect\n\n";
    }

    return dbc;
}

void getData(SQLHDBC dbc) {
    SQLHSTMT stmt;
    SQLSMALLINT columns = 0;
    int row = 0;

    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    SQLRETURN ret = SQLExecDirect(stmt, (SQLWCHAR*) L"CREATE TABLE IF NOT EXISTS `new_table1` (`id` INT NOT NULL,`name` VARCHAR(45) NULL);", SQL_NTS); // , PRIMARY KEY(`id`)

    ret = SQLExecDirect(stmt, (SQLWCHAR*)L"INSERT INTO `new_table1` (`id`, `name`) VALUES ('49595', 'Hello ECE49595');", SQL_NTS);
    ret = SQLExecDirect(stmt, (SQLWCHAR*)L"INSERT INTO `new_table1` (`id`, `name`) VALUES ('40', 'Value of 40');", SQL_NTS);


    ret = SQLExecDirect(stmt, (SQLWCHAR*)L"SELECT * FROM `new_table1`;", SQL_NTS);

    SQLUINTEGER a = -1;
    char aStr[512];

    SQLLEN alenorind = 2;
    SQLLEN astrlenorind = 2;

    SQLBindCol(stmt, 1, SQL_C_ULONG, &a, 0, &alenorind);
    SQLBindCol(stmt, 2, SQL_C_CHAR, &aStr[0], 0, &astrlenorind);

    ret = SQLFetch(stmt);

    while (ret != SQL_ERROR) {
        std::cout << "a: " << a << "\n";
        std::cout << "aStr: " << aStr << "\n";
        ret = SQLFetch(stmt);
    }
   











}
