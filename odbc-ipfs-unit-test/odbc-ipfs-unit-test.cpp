#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

//#include "..\odbc-ipfs\core.h"
#include <Odbcinst.h>


namespace odbcipfsunittest
{
	TEST_CLASS(odbcipfsunittest)
	{
	public:

		SQLHDBC dbc;
		SQLHENV env;
		SQLRETURN ret;

		TEST_METHOD_INITIALIZE(TestMethodInit)
		{			
			ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLAllocHandle SQL_HANDLE_ENV  Failed");

			ret = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLSetEnvAttr SQL_ATTR_ODBC_VERSION  Failed");

			ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLAllocHandle SQL_HANDLE_DBC Failed");


			SQLWCHAR str_ret[1024];
			SQLSMALLINT str_ret_len;

			wchar_t str_dsn[] = L"FILEDSN=C:/Program Files/Common Files/Odbc/Data Sources/odbc-ipfs.dsn";

			ret = SQLDriverConnect(dbc, NULL, str_dsn, SQL_NTS, str_ret, sizeof(4096), &str_ret_len, SQL_DRIVER_COMPLETE);

			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLDriverConnect Failed");
		}

		TEST_METHOD_CLEANUP(TestMethodCleanup)
		{
			// test method cleanup  code
			//ret = SQLDisconnect(dbc);
			//Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLDisconnect Failed");

			//ret = SQLFreeHandle(SQL_HANDLE_DBC, dbc);
			//Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLFreeHandle(SQL_HANDLE_DBC) Failed");
			
			//ret = SQLFreeHandle(SQL_HANDLE_ENV, env);
			//Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLFreeHandle(SQL_HANDLE_ENV) Failed");
		}
		
		TEST_METHOD(TestSQLAllocHandle)
		{
			SQLHSTMT stmt;

			ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
			//Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLAllocHandle(SQL_HANDLE_STMT) Failed");

			//ret = SQLFreeHandle(SQL_HANDLE_ENV, env);
			//Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLFreeHandle(SQL_HANDLE_ENV) Failed");

		}

		
		TEST_METHOD(TestMethod1) //TestSQLExecDirect
		{	

			

			SQLHSTMT stmt;

			SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

			ret = SQLExecDirect(stmt, (SQLWCHAR*)L"CREATE TABLE IF NOT EXISTS `new_table1` (`id` INT NOT NULL,`name` VARCHAR(45) NULL);", SQL_NTS); // , PRIMARY KEY(`id`)
			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLExecDirect Failed");

			ret = SQLExecDirect(stmt, (SQLWCHAR*)L"INSERT INTO `new_table1` (`id`, `name`) VALUES ('555', 'Hello ECE49595');", SQL_NTS);
			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLExecDirect Failed");
			ret = SQLExecDirect(stmt, (SQLWCHAR*)L"INSERT INTO `new_table1` (`id`, `name`) VALUES ('40', 'dsfdssdfaf');", SQL_NTS);
			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLExecDirect Failed");

			ret = SQLExecDirect(stmt, (SQLWCHAR*)L"SELECT * FROM `new_table1`;", SQL_NTS);
			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLExecDirect Failed");
		}

		

	};
}