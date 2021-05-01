#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

//#include "..\odbc-ipfs\core.h"
#include <Odbcinst.h>

#include "../odbc-ipfs/core.h"
#define BUFFERSIZE 1024
#include <assert.h>


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
		TEST_METHOD(TestMethod2) // Test SQLBINDCOL
		{
			SQLHSTMT stmt;

			SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

			STMT* s = (STMT*)stmt;
			SQLLEN column1;
			SQLLEN column4;
			SQLLEN column6;

			SQLPOINTER Column1 = malloc(sizeof(unsigned char) * BUFFERSIZE);
			SQLPOINTER Column4 = malloc(sizeof(float) * BUFFERSIZE);
			SQLPOINTER Column6 = malloc(sizeof(double) * BUFFERSIZE);

			//assert();


			ret = SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLPOINTER)Column1, sizeof(Column1), &column1);
			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLBINDCOL Failed");
			switch (s->bindcols[1].type) {
			case SQL_C_CHAR:
				//Assert::AreEqual(SQL_C_CHAR, stmt->bincols[1].type, L"SQLBINDCOL TYPE Failed");
				break;
			case SQL_UNKNOWN_TYPE:
				assert(SQL_C_CHAR == s->bindcols[1].type);
				//Assert::AreEqual(SQL_C_CHAR, s->bindcols[1].type, L"SQLBINDCOL TYPE Failed");
				break;
			}
			assert(BUFFERSIZE == s->bindcols[1].BufferLength);
			//Assert::AreEqual(BUFFERSIZE, s->bindcols[1].BufferLength, L"BufferLengths not equal");

			ret = SQLBindCol(stmt, 4, SQL_C_FLOAT, (SQLPOINTER)Column4, sizeof(Column4), &column4);
			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLBINDCOL Failed");
			switch (s->bindcols[4].type) {
			case SQL_C_FLOAT:
				//Assert::AreEqual(SQL_C_CHAR, stmt->bincols[1].type, L"SQLBINDCOL TYPE Failed");
				break;
			case SQL_UNKNOWN_TYPE:
				assert(SQL_C_FLOAT == s->bindcols[4].type);
				//Assert::AreEqual(SQL_C_CHAR, s->bindcols[4].type, L"SQLBINDCOL TYPE Failed");
				break;
			}
			assert(BUFFERSIZE == s->bindcols[4].BufferLength);
			//Assert::AreEqual(BUFFERSIZE, s->bindcols[4].BufferLength, L"BufferLengths not equal");

			ret = SQLBindCol(stmt, 6, SQL_C_DOUBLE, (SQLPOINTER)Column6, sizeof(Column6), &column6);
			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLBINDCOL Failed");
			switch (s->bindcols[6].type) {
			case SQL_C_DOUBLE:
				//Assert::AreEqual(SQL_C_CHAR, stmt->bincols[1].type, L"SQLBINDCOL TYPE Failed");
				break;
			case SQL_UNKNOWN_TYPE:
				assert(SQL_C_DOUBLE == s->bindcols[6].type);
				//Assert::AreEqual(SQL_C_CHAR, s->bindcols[6].type, L"SQLBINDCOL TYPE Failed");
				break;
			}
			assert(BUFFERSIZE == s->bindcols[6].BufferLength);
			//Assert::AreEqual(BUFFERSIZE, s->bindcols[6].BufferLength, L"BufferLengths not equal");



			Assert::AreEqual(6, s->nbindcols, L"Number of Bound Columns not equal");

		}

		

	};
}