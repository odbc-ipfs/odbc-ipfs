#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include "connector.h"
#include "cgo-connector.h"



SQLRETURN SQL_API execute(SQLHSTMT sqlstmt, SQLWCHAR* StatementText) {

	OutputDebugString(L"connector execute\n");

	char text[512]; 
	sprintf_s(text, "%ws", (wchar_t*)StatementText);


	Query(text);


	return SQL_SUCCESS;
}




SQLRETURN connect(SQLHENV EnvironmentHandle, SQLHDBC* ConnectionHandle) {

	OutputDebugString(L"connector connect\n");

	CreateShell();
	

	return SQL_SUCCESS;
}

SQLRETURN disconnect(SQLHDBC ConnectionHandle) {

	OutputDebugString(L"connector disconnect\n");

	CloseShell();
	

	return SQL_SUCCESS;

}