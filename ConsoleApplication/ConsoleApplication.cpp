// ConsoleApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "..\odbc-ipfs\core.h"

int main()
{
    std::cout << "Hello World!\n";
	OutputDebugString(L"Hello World!\n");
	
	SQLHDBC dbc;
	SQLHENV env;
	SQLRETURN ret = -1;

	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);

	if (ret != (SQLRETURN)SQL_SUCCESS) {
		std::cout << "SQLAllocHandle fail 1\n";
		return 0;
	}

	//Assert::AreEqual(ret, (SQLRETURN)SQL_SUCCESS);

	//Assert::IsNotNull(env);

	//SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

	ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

	if (ret != (SQLRETURN)SQL_SUCCESS) {
		std::cout << "SQLAllocHandle fail 2\n";
		return 0;
	}

	char res[50];
	sprintf_s(res, "Ret: %d", ret);

	//Logger::WriteMessage(res);
	return 0;
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
