#pragma once
#include "core.h"
#include "sqlite64/sqlite3.h"
#include <iostream>

SQLRETURN SQL_API connect(_Out_ SQLHDBC* ConnectionHandle);
SQLRETURN SQL_API disconnect(SQLHDBC ConnectionHandle);