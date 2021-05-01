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
#define MY_PRINTF(...) {char cad[512]; sprintf(cad, __VA_ARGS__);  OutputDebugString(cad);}
SQLRETURN SQL_API fetch(SQLHSTMT sqlstmt) {
	wchar_t debug[512];

	void** ptrs;
	int* typeArr;

	STMT* stmt = (STMT*)sqlstmt;
	
	ptrs = (void**) malloc(sizeof(void*) * stmt->nbindcols);

	if (ptrs == NULL) {
		printf("Malloc error");
		return SQL_ERROR;
	}

	typeArr = (int*)malloc(sizeof(int) * stmt->nbindcols);

	if (typeArr == NULL) {
		printf("Malloc error");
		return SQL_ERROR;
	}
	OutputDebugString(L"fetch before loop\n");
	for (int i = 0; i < stmt->nbindcols; i++) {
		BINDCOL boundCol = stmt->bindcols[i+1];
		ptrs[i] = (void*) boundCol.TargetValueptr;

		swprintf_s(debug, L"ptrs[i] = %p\n", ptrs[i]);
		OutputDebugString(debug);
		
		swprintf_s(debug, L"boundCol.type = %d\n", boundCol.type);
		OutputDebugString(debug);

		if (boundCol.type == 0) {
			typeArr[i] = 0;
			OutputDebugString(L"fetch is int\n");
			swprintf_s(debug, L"int value = %d\n", ptrs[i]);
			OutputDebugString(debug);
		}
		else if (boundCol.type == 1){
			typeArr[i] = 1;
			OutputDebugString(L"fetch is char*\n");
		}
		else {
			typeArr[i] = 0;
			
			
			
		}
	}
	OutputDebugString(L"fetch after loop\n");
	GoSlice dataAddList = { &ptrs[0], stmt->nbindcols, stmt->nbindcols};
	GoSlice typeSlice = { &typeArr[0], stmt->nbindcols, stmt->nbindcols };

	OutputDebugString(L"fetch calling cgo\n");
	Fetch(dataAddList, typeSlice, stmt->nbindcols);
	OutputDebugString(L"fetch end cgo\n");
	free(ptrs);
	free(typeArr);
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