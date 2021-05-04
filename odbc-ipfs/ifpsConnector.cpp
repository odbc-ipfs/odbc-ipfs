#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include "ipfsConnector.h"
#include "cgo-connector.h"
#include <stdio.h>


SQLRETURN SQL_API execute(SQLHSTMT sqlstmt, SQLWCHAR* StatementText) {

	OutputDebugString(L"connector execute\n");

	char text[512]; 
	sprintf_s(text, "%ws", (wchar_t*)StatementText);


	Query(text);


	return SQL_SUCCESS;
}
SQLRETURN SQL_API fetch(SQLHSTMT sqlstmt) {
	wchar_t debug[512];

	void** ptrs;
	int* typeArr;
	int ret;

	STMT* stmt = (STMT*)sqlstmt;
	
	ptrs = (void**) malloc(sizeof(void*) * stmt->nbindcols);

	if (ptrs == NULL) {
		return SQL_ERROR;
	}

	typeArr = (int*)malloc(sizeof(int) * stmt->nbindcols);

	if (typeArr == NULL) {
		return SQL_ERROR;
	}
	for (int i = 0; i < stmt->nbindcols; i++) {
		BINDCOL boundCol = stmt->bindcols[i+1];
		ptrs[i] = (void*) boundCol.TargetValueptr;


		if (boundCol.type == SQL_C_ULONG) {
			typeArr[i] = 0;

		}
		else if (boundCol.type == SQL_C_CHAR){
			typeArr[i] = 1;

		}
		else {
			
			return SQL_ERROR;
		}
	}
	GoSlice dataAddList = { ptrs, stmt->nbindcols, stmt->nbindcols};
	GoSlice typeSlice = { &typeArr[0], stmt->nbindcols, stmt->nbindcols };

	ret = Fetch(dataAddList, typeSlice, stmt->nbindcols);

	/*
	 	ERROR    = int32(0)
	QUERY    = int32(1)
	FETCHEND = int32(2)
	 */

	

	free(ptrs);
	free(typeArr);

	if (ret == 0) {
		return SQL_ERROR;
	}

	return SQL_SUCCESS;
}


SQLRETURN SQL_API connect(SQLHENV EnvironmentHandle, SQLHDBC* ConnectionHandle) {

	OutputDebugString(L"connector connect\n");

	CreateShell();
	

	return SQL_SUCCESS;
}

SQLRETURN SQL_API disconnect(SQLHDBC ConnectionHandle) {

	OutputDebugString(L"connector disconnect\n");

	CloseShell();
	

	return SQL_SUCCESS;

}