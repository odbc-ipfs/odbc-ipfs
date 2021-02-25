#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include "connector.h"

SQLRETURN connect(SQLHENV EnvironmentHandle, _Out_ SQLHDBC* ConnectionHandle) {


	//Should NOT Malloc
	//See: https://docs.microsoft.com/en-us/cpp/code-quality/understanding-sal?view=msvc-160
	//Caller provides space to write to

	//SQLHDBC* dbc = (SQLHDBC*)calloc(1, sizeof(SQLHDBC));

	//if (dbc == NULL) return SQL_ERROR;

	sqlite3* db;
	char* err = 0;
	int rc;

	rc = sqlite3_open("test.db", &db);

	if (rc) {
		sqlite3_close(db);
		return SQL_ERROR;
	}
	
	//*dbc = db;

	//*ConnectionHandle = &dbc;

	*ConnectionHandle = db;

	return SQL_SUCCESS;
}

SQLRETURN disconnect(SQLHDBC ConnectionHandle) {

	sqlite3* db = (sqlite3*) ConnectionHandle;

	sqlite3_close(db);

	return SQL_SUCCESS;

}
