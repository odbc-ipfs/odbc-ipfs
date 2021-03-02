#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#include <Windows.h>
#include "..\odbc-ipfs\core.h"



namespace odbcipfsunittest
{
	TEST_CLASS(odbcipfsunittest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			//Assert::AreEqual(1, 1);
			
			SQLHDBC dbc;
			SQLHENV env;
			SQLRETURN ret;

			SQLRETURN success = SQL_SUCCESS;


			ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
			//SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

			Assert::AreEqual(ret, success);


		}
	};
}
