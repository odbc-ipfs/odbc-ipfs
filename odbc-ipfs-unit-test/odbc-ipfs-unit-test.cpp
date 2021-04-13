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
			// method initialization code
			/*
			LPCSTR    lpszDriver = "odbc-ipfs";        // Input
			LPCSTR    lpszPathIn = NULL;               // Input
			LPSTR     lpszPathOut;                     // Output
			WORD      cbPathOutMax;                    // Input
			WORD*     pcbPathOut;                      // Output
			WORD      fRequest = ODBC_INSTALL_INQUIRY; // Input (ODBC_INSTALL_INQUIRY, ODBC_INSTALL_COMPLETE)
			LPDWORD   lpdwUsageCount;                  // Output
			*/




			ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLAllocHandle SQL_HANDLE_ENV  Failed");

			ret = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLSetEnvAttr SQL_ATTR_ODBC_VERSION  Failed");

			ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLAllocHandle SQL_HANDLE_DBC Failed");


			SQLWCHAR str_ret[1024];
			SQLSMALLINT str_ret_len;

			wchar_t str_dsn[] = L"FILEDSN=C:/Program Files/Common Files/Odbc/Data Sources/odbc-ipfs.dsn;";

			ret = SQLDriverConnect(dbc, NULL, str_dsn, SQL_NTS, str_ret, sizeof(4096), &str_ret_len, SQL_DRIVER_COMPLETE);

			Assert::AreEqual((SQLRETURN)SQL_SUCCESS, ret, L"SQLDriverConnect Failed");
		}

		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual(1, 1);




			Logger::WriteMessage("Testing");

			//SQLDisconnect(dbc);

			//SQLFreeHandle(SQL_HANDLE_DBC, dbc);
			//SQLFreeHandle(SQL_HANDLE_ENV, env);

		}
	};
}