// SampleODBCApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SampleODBCApplication.h"
#include "sqlite64/sqlite3.h"


SQLHENV env;

SQLWCHAR driver[256];
SQLWCHAR attr[256];

SQLWCHAR dsn[256];
SQLWCHAR desc[256];

SQLSMALLINT driver_ret;
SQLSMALLINT attr_ret;

SQLSMALLINT dsn_ret;
SQLSMALLINT desc_ret;

SQLUSMALLINT direction;
SQLRETURN ret;

//https://www.easysoft.com/developer/languages/c/odbc_tutorial.html

//https://docs.microsoft.com/en-us/sql/odbc/reference/syntax/sqlexecdirect-function?view=sql-server-ver15
//https://docs.microsoft.com/en-us/sql/odbc/reference/syntax/sqltables-function?view=sql-server-ver15


//NOTE: make sure you are building for the right system
// x86 or x64
// x86 applications will NOT find x64 drivers and vice versa
int main()
{

    SQLHDBC dbc;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    listDataSources();

    listSQLDrivers();

    dbc = conn();

    getInfo(dbc);

    getData(dbc);

    SQLDisconnect(dbc);

    sqlite3* db;
    char* zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);

    if (rc) {
        printf("\nError, can't open databse\n");
        sqlite3_close(db);
    }
    else {
        printf("\nOpened sqlite database successfully\n");
    }

    sqlite3_close(db);

    
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);

}

void getData(SQLHDBC dbc) {
    SQLHSTMT stmt;
    SQLSMALLINT columns = 0;
    int row = 0;

    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    //char str[] = "TABLE";

    //SQLWCHAR str_wchar[1024];

    //int i = 0;
    //for (i = 0; i < strlen(str); i++) {
    //    str_wchar[i] = str[i];
    //}

    wchar_t catalog_name[] = L"";
    wchar_t schema_name[] = L"test_schema";
    wchar_t table_name[] = L""; 
    wchar_t table_type[] = L""; //TABLE


    //SQLTables(stmt, NULL, 0, NULL, 0, NULL, 0, type, SQL_NTS);
    //(SQLWCHAR*) SQL_ALL_CATALOGS
    
    
    SQLTables(stmt, catalog_name, SQL_NTS, schema_name, SQL_NTS, table_name, SQL_NTS, table_type, SQL_NTS);

    printf("\nGetting data\n");

    SQLNumResultCols(stmt, &columns);

    printf("Cols: %d\n", columns);

    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        SQLUSMALLINT i;
        printf("Row %d\n", row++);

        for (i = 1; i < columns; i++) {
            //SQLINTEGER indicator;
            SQLLEN indicator;
            char buff[512];
            ret = SQLGetData(stmt, i, SQL_C_CHAR, buff, sizeof(buff), &indicator);
            if (SQL_SUCCEEDED(ret)) {
                if (indicator == SQL_NULL_DATA) {
                    strcpy_s(buff, "NULL");
                }

                printf(" Column %d : %s\n", i, buff);

            }
        }

    }

    printf("Ret: %d\n", ret);

    printf("End getting data\n\n");

    
    //wchar_t prep[] = L"INSERT INTO `test_schema`.`new_table` (`idnew_table`, `new_tablecol`, `new_tablecol1`) VALUES ('12', 'f', 'ss');";
    wchar_t prep[] = L"SELECT * FROM `test_schema`.`new_table`;";

    ret = SQLExecDirect(stmt, prep, SQL_NTS);

    if (SQL_SUCCEEDED(ret)) {
        printf("Select * succeeded\n");
        SQLNumResultCols(stmt, &columns);
        printf("Columns: %d\n", columns);
    }
    else {
        printf("Select * failed\n");
        printf("Ret: %d\n", ret);
    }


    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

void getInfo(SQLHDBC dbc) {
    SQLWCHAR dbms_name[256], dbms_ver[256];
    SQLUINTEGER getdata_support;
    SQLUSMALLINT max_concur_act;
    SQLSMALLINT string_len;

    SQLGetInfo(dbc, SQL_DBMS_NAME, (SQLPOINTER)dbms_name, sizeof(dbms_name), NULL);
    SQLGetInfo(dbc, SQL_DBMS_VER, (SQLPOINTER)dbms_ver, sizeof(dbms_ver), NULL);
    SQLGetInfo(dbc, SQL_GETDATA_EXTENSIONS, (SQLPOINTER)&getdata_support, 0, 0);
    SQLGetInfo(dbc, SQL_MAX_CONCURRENT_ACTIVITIES, &max_concur_act, 0, 0);


    printf("DBMS Name: ");
    printSQLCharString(dbms_name, sizeof(dbms_name));
    printf("\n");

    printf("DBMS Version: ");
    printSQLCharString(dbms_ver, sizeof(dbms_ver));
    printf("\n");

    if (max_concur_act == 0) {
        printf("SQL_MAX_CONCURRENT_ACTIVITIES - no limit or undefined\n");
    }
    else {
        printf("SQL_MAX_CONCURRENT_ACTIVITIES = %u\n", max_concur_act);
    }
    if (getdata_support & SQL_GD_ANY_ORDER)
        printf("SQLGetData - columns can be retrieved in any order\n");
    else
        printf("SQLGetData - columns must be retrieved in order\n");
    if (getdata_support & SQL_GD_ANY_COLUMN)
        printf("SQLGetData - can retrieve columns before last bound one\n");
    else
        printf("SQLGetData - columns must be retrieved after last bound one\n");

}



SQLHDBC conn() {
    SQLHDBC dbc;

    SQLWCHAR str_ret[1024];
    SQLSMALLINT str_ret_len;

    
    wchar_t str_dsn[] = L"DSN=ThomasDevODBC;";


    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

    ret = SQLDriverConnect(dbc, NULL, str_dsn, SQL_NTS, str_ret, sizeof(str_ret), &str_ret_len, SQL_DRIVER_COMPLETE);

    if (SQL_SUCCEEDED(ret)) {
        std::cout << "Connected to database\n\n";
    }else {
        std::cout << "Failed to connect\n\n";
    }

    return dbc;
 }

void listDataSources() {
    std::cout << "-----[ODBC Data Sources]-----\n";
    direction = SQL_FETCH_FIRST;
    while (SQL_SUCCEEDED(ret = SQLDataSources(env, direction,
        dsn, sizeof(dsn), &dsn_ret,
        desc, sizeof(desc), &desc_ret))) {
        
        direction = SQL_FETCH_NEXT;

        printSQLCharString(dsn, dsn_ret);
        std::cout << " - ";

        printSQLCharString(desc, desc_ret);
        
        std::cout << "\n";
    }

    std::cout << "\n";
    
}

void listSQLDrivers() {
    std::cout << "-----[ODBC Drivers]-----\n";
    
    direction = SQL_FETCH_FIRST;
    while (SQL_SUCCEEDED(ret = SQLDrivers(env, direction,
        driver, sizeof(driver), &driver_ret,
        attr, sizeof(attr), &attr_ret))) {
        
        direction = SQL_FETCH_NEXT;
        printSQLCharString(driver, driver_ret);
        std::cout << " - ";

        printSQLCharString(attr, attr_ret);
        
        std::cout << "\n";

    }
    
    std::cout << "\n";
}

void printSQLCharString(SQLWCHAR* sqlCharString, SQLSMALLINT length) {
    for (int i = 0; i < length; i++) {
        if (sqlCharString[i] == '\0') break;
        printf("%c", sqlCharString[i]);
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
