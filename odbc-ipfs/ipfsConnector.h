#pragma once
#include "core.h"
//#include "sqlite64/sqlite3.h"
#include <iostream>

SQLRETURN SQL_API connect(SQLHENV EnvironmentHandle, SQLHDBC* ConnectionHandle);
SQLRETURN SQL_API disconnect(SQLHDBC ConnectionHandle);
SQLRETURN SQL_API execute(SQLHSTMT sqlstmt, SQLWCHAR* StatementText);
SQLRETURN SQL_API fetch(SQLHSTMT sqlstmt);
