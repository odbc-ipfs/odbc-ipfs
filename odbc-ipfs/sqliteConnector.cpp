#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include "connector.h"

static int callback(void* unused, int argc, char** argv, char** colName);

//SQLHSTMT StatementHandle, SQLCHAR* StatementText, SQLINTEGER   TextLength


SQLRETURN SQL_API execute(SQLHSTMT sqlstmt, SQLWCHAR* StatementText) {

	OutputDebugString(L"connector execute\n");

	STMT* stmt = (STMT*)sqlstmt;


	char text[512]; //Need to change this.
	sprintf_s(text, "%ws", (wchar_t*)StatementText);

	DBC* dbc = stmt->dbc;

	if (dbc == NULL) {
		OutputDebugString(L"execute dbc == NULL\n");

		return SQL_ERROR;
	}

	sqlite3* db = (sqlite3*)dbc->handle;

	if (db == NULL) {
		OutputDebugString(L"execute db == NULL\n");

		return SQL_ERROR;
	}

	char* err = 0;
	int rc;
	rc = sqlite3_exec(db, text, callback, stmt, &err);



	if (rc != SQLITE_OK) {
		OutputDebugString(L"execute SQL_ERROR\n");
		OutputDebugString(StatementText);

		return SQL_ERROR;
	}


	OutputDebugString(L"execute SQL_SUCCESS\n");

	return SQL_SUCCESS;
}



static int callback(void* voidstmt, int argc, char** argv, char** colName) {

	OutputDebugString(L"connector callback\n");

	char str[100];
	wchar_t  ws[100];

	if (voidstmt == NULL) {
		OutputDebugString(L"connector ERROR stmt == NULL\n");
		return 0;
	}

	STMT* stmt = (STMT*)voidstmt;

	int i;
	for (i = 0; i < argc; i++) {
		sprintf_s(str, "%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");

		swprintf(ws, 100, L"%hs", str);
		OutputDebugString(ws);

	}

	stmt->argc = argc;
	stmt->argv = argv; // This probably doesn't work
	stmt->colName = colName; // This probably doesn't work

	return 0;
}

SQLRETURN connect(SQLHENV EnvironmentHandle, SQLHDBC* ConnectionHandle) {

	OutputDebugString(L"connector connect\n");

	//SQLHDBC* dbc = (SQLHDBC*)calloc(1, sizeof(SQLHDBC));

	//if (dbc == NULL) return SQL_ERROR;

	DBC* dbc = (DBC*)*ConnectionHandle;


	sqlite3* db;
	char* err = 0;
	int rc;

	rc = sqlite3_open("test2.db", &db);

	if (rc) {
		sqlite3_close(db);
		OutputDebugString(L"connector SQL_ERROR\n");

		return SQL_ERROR;
	}

	//*dbc = db;

	//*ConnectionHandle = &dbc;

	dbc->handle = db;

	OutputDebugString(L"connector SQL_SUCCESS\n");

	return SQL_SUCCESS;
}

SQLRETURN disconnect(SQLHDBC ConnectionHandle) {

	OutputDebugString(L"connector disconnect\n");

	DBC* dbc = (DBC*)ConnectionHandle;

	sqlite3* db = (sqlite3*)dbc->handle;

	if (db == NULL) {
		OutputDebugString(L"disconnect db == NULL\n");

		return SQL_ERROR;
	}

	sqlite3_close(db);

	return SQL_SUCCESS;

}