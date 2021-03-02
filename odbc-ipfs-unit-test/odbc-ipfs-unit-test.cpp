#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#include <Windows.h>
#include "..\odbc-ipfs\core.h"
#include <sql.h>


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

			ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);

			Assert::AreEqual(ret, (SQLRETURN) SQL_SUCCESS);

			//SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

			ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

			char res[50];
			sprintf_s(res, "Ret: %d", ret);

			Logger::WriteMessage(res);

			//Assert::AreEqual(ret, (SQLRETURN) SQL_SUCCESS);


			//Logger::WriteMessage("Testing");
			


		}
	};
}
