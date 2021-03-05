#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include "core.h"
#include "connector.h"
#include <iostream>

//////////////////////////////////////////////////////
#define CHECK_HANDLE(h) if (h == NULL) return SQL_INVALID_HANDLE
#define strmake(dst, src, max, lenp) { \
    int len = strlen(src); \
    int cnt = min(len + 1, max); \
    strncpy_s(dst, max, src, cnt); \
    *lenp = (cnt > len) ? len : cnt; \
}
////////////////////////////////////////////////////////

/*
allocates ConnectionHandle and links EnvironmentHandle to it then replaces the input pointer to the allocated pointer*/
SQLRETURN  SQL_API SQLAllocConnect(SQLHENV EnvironmentHandle,
    _Out_ SQLHDBC* ConnectionHandle) {
    DBC* dbc;
    ENV* env;

    if (EnvironmentHandle == NULL) {
        return SQL_INVALID_HANDLE;
    }
    //if (ConnectionHandle == NULL) {
    //    return SQL_INVALID_HANDLE;
    //}

    env = (ENV*)EnvironmentHandle;
    dbc = (DBC*)malloc(sizeof(DBC));

    //if (dbc == NULL) {
    //   *ConnectionHandle = SQL_NULL_HDBC;
    //   return SQL_ERROR;
    //}

    dbc->env = env;
    *ConnectionHandle = (SQLHDBC)dbc;

    //connect(EnvironmentHandle, (SQLHDBC*) dbc);

    return SQL_SUCCESS;
}

/*
allocates EnvironmentHandle and replaces input pointer to the allocated pointer*/
SQLRETURN  SQL_API SQLAllocEnv(_Out_ SQLHENV* EnvironmentHandle) {
    ENV* e;
    if (EnvironmentHandle == NULL) {
        return SQL_INVALID_HANDLE;
    }

    e = (ENV*) malloc(sizeof(ENV));
    if (e == NULL) {
        *EnvironmentHandle = SQL_NULL_HENV;
        return SQL_ERROR;
    }
    
    *EnvironmentHandle = (SQLHENV)e;

    
        
    return SQL_SUCCESS;
}


/*
SQLAllocHandle replaces SQLAllocConnect, SQLAllocEnv, and SQLAllocStmt

calls on different allocation depending on the HandleType*/
#if (ODBCVER >= 0x0300)
SQLRETURN  SQL_API SQLAllocHandle(SQLSMALLINT HandleType,
    SQLHANDLE InputHandle, _Out_ SQLHANDLE* OutputHandle) {
    SQLRETURN error = SQL_ERROR;

    switch (HandleType) {
        case SQL_HANDLE_ENV:
            error = SQLAllocEnv(OutputHandle);
            break;
        case SQL_HANDLE_DBC:
            error = SQLAllocConnect(InputHandle, OutputHandle);
            break;
        case SQL_HANDLE_STMT:
            error = SQLAllocStmt(InputHandle, OutputHandle);
            break;
        /*
        case SQL_HANDLE_DESC:
            break;
        */
        default:
            return SQL_ERROR;
    }

    return error;
}
#endif

SQLRETURN  SQL_API SQLAllocStmt(SQLHDBC ConnectionHandle,
    _Out_ SQLHSTMT* StatementHandle) {
    DBC* dbc;
    STMT* stmt;

    if (ConnectionHandle == NULL) {
        return SQL_INVALID_HANDLE;
    }
    if (StatementHandle == NULL) {
        return SQL_INVALID_HANDLE;
    }
    
    dbc = (DBC*)ConnectionHandle;
    
    stmt = (STMT*)malloc(sizeof(STMT));
    
    if (stmt == NULL) {
        *StatementHandle = SQL_NULL_HSTMT;
        return SQL_ERROR;
    }

    stmt->dbc = dbc;
    *StatementHandle = (SQLHSTMT)stmt;

    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLBindCol(SQLHSTMT StatementHandle,
    SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType,
    _Inout_updates_opt_(_Inexpressible_(BufferLength)) SQLPOINTER TargetValue,
    SQLLEN BufferLength, _Inout_opt_ SQLLEN* StrLen_or_Ind) {
    return SQL_SUCCESS;
}

#if (ODBCVER >= 0x0300)
__declspec(deprecated("ODBC API: SQLBindParam is deprecated. Please use SQLBindParameter instead."))
SQLRETURN  SQL_API SQLBindParam(SQLHSTMT StatementHandle,
    SQLUSMALLINT ParameterNumber, SQLSMALLINT ValueType,
    SQLSMALLINT ParameterType, SQLULEN LengthPrecision,
    SQLSMALLINT ParameterScale, SQLPOINTER ParameterValue,
    SQLLEN* StrLen_or_Ind) {
    return SQL_SUCCESS;
}
#endif

SQLRETURN  SQL_API SQLCancel(SQLHSTMT StatementHandle) {
    return SQL_SUCCESS;
}

#if (ODBCVER >= 0x0380)
SQLRETURN  SQL_API SQLCancelHandle(SQLSMALLINT HandleType, SQLHANDLE InputHandle) {
    return SQL_SUCCESS;
}
#endif // ODBCVER >= 0x0380

#if (ODBCVER >= 0x0300)
SQLRETURN  SQL_API SQLCloseCursor(SQLHSTMT StatementHandle) {
    return SQL_SUCCESS;
}

#ifdef _WIN64
// SQLRETURN  SQL_API SQLColAttribute(SQLHSTMT StatementHandle,
//     SQLUSMALLINT ColumnNumber, SQLUSMALLINT FieldIdentifier,
//     _Out_writes_bytes_opt_(BufferLength) SQLPOINTER CharacterAttribute, SQLSMALLINT BufferLength,
//     _Out_opt_ SQLSMALLINT* StringLength, _Out_opt_ SQLLEN* NumericAttribute) {
//     return SQL_SUCCESS;
// }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  Retrieve an attribute of a column in a result set.
  @param[in]  StatementHandle          Handle to statement
  @param[in]  ColumnNumber       The column to retrieve data for, indexed from 1
  @param[in]  FieldIdentifier        The attribute to be retrieved
  @param[out] CharacterAttributePtr      Pointer to a string pointer for returning strings
                             (caller must make their own copy)
  @param[out] NumericAttributePtr       Pointer to an integer to return the value if the
                             @a attrib corresponds to a numeric type
  @since ODBC 1.0
*/
// It isn't clear if sqlite3_exec allocates memory for the parameters of callback or whether we have to do it ourselves whenever the STMT class is created
// using table with structure of 
//          col1|col2|col3
// attr1 |     |    |
// attr2 |     |    |
// attr3 |     |    |
//
SQLRETURN SQL_API SQLColAttribute (SQLHSTMT  StatementHandle, SQLUSMALLINT ColumnNumber, SQLUSMALLINT FieldIdentifier, SQLPOINTER CharacterAttributePtr, SQLSMALLINT BufferLength, SQLSMALLINT *StringLengthPtr, SQLLEN *NumericAttributePtr){


    CHECK_HANDLE(StatementHandle);
    SQLINTEGER len= SQL_NTS;
    STMT *stmt= (STMT *)StatementHandle;

    if(FieldIdentifier == NULL){   // convert the field identifier to a string if not null in case the switch cases below don't work
        printf("No Attribute specified\n");
    }
    if(stmt->argc <= 0){ // This isn't like main, there should only be columns here and no executable
        printf("No columns in result from database\n");
        return SQL_SUCCESS;
        return SQL_ERROR; // If thats what we want
    }
    if(ColumnNumber < 0 || ColumnNumber >= stmt->argc){ // Not valid index
        printf("Not a valid column index\n");
        return SQL_ERROR;
    }

    if(stmt->argv[ColumnNumber] == NULL){ // there is no allocated memory for that column //If an element of a result row is NULL then the corresponding string pointer for the sqlite3_exec() callback is a NULL pointer.
        printf("Empty result row\n");
        return SQL_SUCCESS;
        return SQL_ERROR; // If thats what we want
    }

    switch(FieldIdentifier)
    {
    case SQL_DESC_TYPE:
        if(ColumnNumber == 0)
            *(SQLINTEGER *)NumericAttributePtr = SQL_INTEGER; // Did this because of MYSQL but the windows website says there is more
        return SQL_SUCCESS;   
        break;
    case SQL_DESC_COUNT: 
        *(SQLINTEGER *)NumericAttributePtr = stmt->argc;
        return SQL_SUCCESS;
        break;
    case SQL_DESC_NAME:
        if(CharacterAttributePtr && BufferLength > 1)  // If memory has been allocated for the results
            strmake((char *)CharacterAttributePtr, (char *)stmt->colName[ColumnNumber], BufferLength - 1,&len); // use strmake like this whenever moving the data
        break;
    }
    
    if(len > BufferLength-1) //the length of the value is greater than the buffer size
        return SQL_ERROR;

    if (StringLengthPtr) // If memory was allocated for the stringlenghtptr
        *StringLengthPtr = (SQLSMALLINT)len;

    return SQL_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
SQLRETURN  SQL_API SQLColAttribute(SQLHSTMT StatementHandle,
    SQLUSMALLINT ColumnNumber, SQLUSMALLINT FieldIdentifier,
    _Out_writes_bytes_opt_(BufferLength) SQLPOINTER CharacterAttribute, SQLSMALLINT BufferLength,
    _Out_opt_ SQLSMALLINT* StringLength, _Out_opt_ SQLPOINTER NumericAttribute) {
    return SQL_SUCCESS;
}
#endif
#endif


SQLRETURN  SQL_API SQLColumns(SQLHSTMT StatementHandle,
    _In_reads_opt_(NameLength1) SQLCHAR* CatalogName, SQLSMALLINT NameLength1,
    _In_reads_opt_(NameLength2) SQLCHAR* SchemaName, SQLSMALLINT NameLength2,
    _In_reads_opt_(NameLength3) SQLCHAR* TableName, SQLSMALLINT NameLength3,
    _In_reads_opt_(NameLength4) SQLCHAR* ColumnName, SQLSMALLINT NameLength4) {
    return SQL_SUCCESS;
}

#if (ODBCVER >= 0x0380)
SQLRETURN SQL_API SQLCompleteAsync(SQLSMALLINT    HandleType,
    SQLHANDLE      Handle,
    _Out_ RETCODE* AsyncRetCodePtr) {
    return SQL_SUCCESS;
}
#endif

SQLRETURN  SQL_API SQLConnect(SQLHDBC ConnectionHandle,
    _In_reads_(NameLength1) SQLCHAR* ServerName, SQLSMALLINT NameLength1,
    _In_reads_(NameLength2) SQLCHAR* UserName, SQLSMALLINT NameLength2,
    _In_reads_(NameLength3) SQLCHAR* Authentication, SQLSMALLINT NameLength3) {

    DBC* dbc;
    if (ConnectionHandle == NULL) {
        return SQL_INVALID_HANDLE;
    }

    dbc = (DBC*)ConnectionHandle;
    dbc->serverName = ServerName;
    dbc->serverLength = NameLength1;
    dbc->userName = UserName;
    dbc->userLength = NameLength2;
    dbc->auth = Authentication;
    dbc->authLength = NameLength3;

    return SQL_SUCCESS;
}

#if (ODBCVER >= 0x0300)
SQLRETURN  SQL_API SQLCopyDesc(SQLHDESC SourceDescHandle,
    SQLHDESC TargetDescHandle) {
    return SQL_SUCCESS;
}
#endif

SQLRETURN  SQL_API SQLDataSources(SQLHENV EnvironmentHandle,
    SQLUSMALLINT Direction, _Out_writes_opt_(BufferLength1) SQLCHAR* ServerName,
    SQLSMALLINT BufferLength1, _Out_opt_ SQLSMALLINT* NameLength1Ptr,
    _Out_writes_opt_(BufferLength2) SQLCHAR* Description, SQLSMALLINT BufferLength2,
    _Out_opt_ SQLSMALLINT* NameLength2Ptr) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLDescribeCol(SQLHSTMT StatementHandle,
    SQLUSMALLINT ColumnNumber, _Out_writes_opt_(BufferLength) SQLCHAR* ColumnName,
    SQLSMALLINT BufferLength, _Out_opt_ SQLSMALLINT* NameLength,
    _Out_opt_ SQLSMALLINT* DataType, _Out_opt_ SQLULEN* ColumnSize,
    _Out_opt_ SQLSMALLINT* DecimalDigits, _Out_opt_ SQLSMALLINT* Nullable) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLDisconnect(SQLHDBC ConnectionHandle) {
    return SQL_SUCCESS;
}

#if (ODBCVER >= 0x0300)
SQLRETURN  SQL_API SQLEndTran(SQLSMALLINT HandleType, SQLHANDLE Handle,
    SQLSMALLINT CompletionType) {
    return SQL_SUCCESS;
}
#endif

SQLRETURN  SQL_API SQLError(SQLHENV EnvironmentHandle,
    SQLHDBC ConnectionHandle, SQLHSTMT StatementHandle,
    _Out_writes_(6) SQLCHAR* Sqlstate, _Out_opt_ SQLINTEGER* NativeError,
    _Out_writes_opt_(BufferLength) SQLCHAR* MessageText, SQLSMALLINT BufferLength,
    _Out_opt_ SQLSMALLINT* TextLength) {
    return SQL_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
SQLRETURN SQL_API
SQLExecDirect(SQLHSTMT stmt, SQLCHAR *query, SQLINTEGER queryLen)
{
    SQLRETURN ret;

    CHECK_HANDLE(stmt);
    // if ((ret= prepare(stmt, query, query_len))) // will be used after prepare has been written
    //     return ret;
    // ret = execute(stmt, (char *) query);

	// ret = execute(stmt, (char *) query);
    ret = SQL_SUCCESS;
    return ret;
}
	
//////////////////////////////////////////////////////////////

SQLRETURN  SQL_API SQLExecute(SQLHSTMT StatementHandle) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLFetch(SQLHSTMT StatementHandle) {
    return SQL_SUCCESS;
}

#if (ODBCVER >= 0x0300)
SQLRETURN  SQL_API SQLFetchScroll(SQLHSTMT StatementHandle,
    SQLSMALLINT FetchOrientation, SQLLEN FetchOffset) {
    return SQL_SUCCESS;
}
#endif

SQLRETURN  SQL_API SQLFreeConnect(SQLHDBC ConnectionHandle) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLFreeEnv(SQLHENV EnvironmentHandle) {
    return SQL_SUCCESS;
}

#if (ODBCVER >= 0x0300)
SQLRETURN  SQL_API SQLFreeHandle(SQLSMALLINT HandleType, SQLHANDLE Handle) {
    return SQL_SUCCESS;
}
#endif

SQLRETURN  SQL_API SQLFreeStmt(SQLHSTMT StatementHandle,
    SQLUSMALLINT Option) {
    return SQL_SUCCESS;
}

#if (ODBCVER >= 0x0300)
SQLRETURN  SQL_API SQLGetConnectAttr(SQLHDBC ConnectionHandle,
    SQLINTEGER Attribute, _Out_writes_opt_(_Inexpressible_(BufferLength)) SQLPOINTER Value,
    SQLINTEGER BufferLength, _Out_opt_ SQLINTEGER* StringLengthPtr) {
    return SQL_SUCCESS;
}
__declspec(deprecated("ODBC API: SQLGetConnectOption is deprecated. Please use SQLGetConnectAttr instead."))
#endif

SQLRETURN  SQL_API SQLGetConnectOption(SQLHDBC ConnectionHandle,
    SQLUSMALLINT Option, SQLPOINTER Value) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLGetCursorName
(
    SQLHSTMT StatementHandle,
    _Out_writes_opt_(BufferLength) SQLCHAR* CursorName,
    SQLSMALLINT BufferLength,
    _Out_opt_
    SQLSMALLINT* NameLengthPtr
) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLGetData(SQLHSTMT StatementHandle,
    SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType,
    _Out_writes_opt_(_Inexpressible_(BufferLength)) SQLPOINTER TargetValue, SQLLEN BufferLength,
    _Out_opt_ SQLLEN* StrLen_or_IndPtr) {
    return SQL_SUCCESS;
}

#if (ODBCVER >= 0x0300)
SQLRETURN  SQL_API SQLGetDescField(SQLHDESC DescriptorHandle,
    SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier,
    _Out_writes_opt_(_Inexpressible_(BufferLength)) SQLPOINTER Value, SQLINTEGER BufferLength,
    _Out_opt_ SQLINTEGER* StringLength) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLGetDescRec(SQLHDESC DescriptorHandle,
    SQLSMALLINT RecNumber, _Out_writes_opt_(BufferLength) SQLCHAR* Name,
    SQLSMALLINT BufferLength, _Out_opt_ SQLSMALLINT* StringLengthPtr,
    _Out_opt_ SQLSMALLINT* TypePtr, _Out_opt_ SQLSMALLINT* SubTypePtr,
    _Out_opt_ SQLLEN* LengthPtr, _Out_opt_ SQLSMALLINT* PrecisionPtr,
    _Out_opt_ SQLSMALLINT* ScalePtr, _Out_opt_ SQLSMALLINT* NullablePtr) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLGetDiagField(SQLSMALLINT HandleType, SQLHANDLE Handle,
    SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier,
    _Out_writes_opt_(_Inexpressible_(BufferLength)) SQLPOINTER DiagInfo, SQLSMALLINT BufferLength,
    _Out_opt_ SQLSMALLINT* StringLength) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLGetDiagRec
(
    SQLSMALLINT HandleType,
    SQLHANDLE Handle,
    SQLSMALLINT RecNumber,
    _Out_writes_opt_(6) SQLCHAR* Sqlstate,
    SQLINTEGER* NativeError,
    _Out_writes_opt_(BufferLength) SQLCHAR* MessageText,
    SQLSMALLINT BufferLength,
    _Out_opt_
    SQLSMALLINT* TextLength
) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLGetEnvAttr(SQLHENV EnvironmentHandle,
    SQLINTEGER Attribute, _Out_writes_(_Inexpressible_(BufferLength)) SQLPOINTER Value,
    SQLINTEGER BufferLength, _Out_opt_ SQLINTEGER* StringLength) {
    return SQL_SUCCESS;
}
#endif  /* ODBCVER >= 0x0300 */

SQLRETURN  SQL_API SQLGetFunctions(SQLHDBC ConnectionHandle,
    SQLUSMALLINT FunctionId,
    _Out_writes_opt_(_Inexpressible_("Buffer length pfExists points to depends on fFunction value."))
    SQLUSMALLINT* Supported) {
    return SQL_SUCCESS;
}

_Success_(return == SQL_SUCCESS)
SQLRETURN  SQL_API SQLGetInfo(SQLHDBC ConnectionHandle,
    SQLUSMALLINT InfoType, _Out_writes_bytes_opt_(BufferLength) SQLPOINTER InfoValue,
    SQLSMALLINT BufferLength, _Out_opt_ SQLSMALLINT * StringLengthPtr) {
    return SQL_SUCCESS;
}

#if (ODBCVER >= 0x0300)
SQLRETURN  SQL_API SQLGetStmtAttr(SQLHSTMT StatementHandle,
    SQLINTEGER Attribute, _Out_writes_opt_(_Inexpressible_(BufferLength)) SQLPOINTER Value,
    SQLINTEGER BufferLength, _Out_opt_ SQLINTEGER * StringLength);
__declspec(deprecated("ODBC API: SQLGetStmtOption is deprecated. Please use SQLGetStmtAttr instead."))
#endif  /* ODBCVER >= 0x0300 */

SQLRETURN  SQL_API SQLGetStmtOption(SQLHSTMT StatementHandle,
    SQLUSMALLINT Option, SQLPOINTER Value) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLGetTypeInfo(SQLHSTMT StatementHandle,
    SQLSMALLINT DataType) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLNumResultCols(SQLHSTMT StatementHandle,
    _Out_ SQLSMALLINT * ColumnCount) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLParamData(SQLHSTMT StatementHandle,
    _Out_opt_ SQLPOINTER * Value) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLPrepare
(
    SQLHSTMT StatementHandle,
    _In_reads_(TextLength) SQLCHAR * StatementText,
    SQLINTEGER TextLength
) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLPutData(SQLHSTMT StatementHandle,
    _In_reads_(_Inexpressible_(StrLen_or_Ind)) SQLPOINTER Data, SQLLEN StrLen_or_Ind);

SQLRETURN  SQL_API SQLRowCount(_In_ SQLHSTMT StatementHandle,
    _Out_ SQLLEN * RowCount) {
    return SQL_SUCCESS;
}

#if (ODBCVER >= 0x0300)
SQLRETURN  SQL_API SQLSetConnectAttr(SQLHDBC ConnectionHandle,
    SQLINTEGER Attribute, _In_reads_bytes_opt_(StringLength) SQLPOINTER Value,
    SQLINTEGER StringLength) {
    return SQL_SUCCESS;
}
__declspec(deprecated("ODBC API: SQLSetConnectOption is deprecated. Please use SQLSetConnectAttr instead."))
#endif /* ODBCVER >= 0x0300 */

SQLRETURN  SQL_API SQLSetConnectOption(SQLHDBC ConnectionHandle,
    SQLUSMALLINT Option, SQLULEN Value) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLSetCursorName
(
    SQLHSTMT StatementHandle,
    _In_reads_(NameLength) SQLCHAR * CursorName,
    SQLSMALLINT NameLength
) {
    return SQL_SUCCESS;
}

#if (ODBCVER >= 0x0300)
SQLRETURN  SQL_API SQLSetDescField(SQLHDESC DescriptorHandle,
    SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier,
    _In_reads_(_Inexpressible_(BufferLength)) SQLPOINTER Value, SQLINTEGER BufferLength);

SQLRETURN  SQL_API SQLSetDescRec(SQLHDESC DescriptorHandle,
    SQLSMALLINT RecNumber, SQLSMALLINT Type,
    SQLSMALLINT SubType, SQLLEN Length,
    SQLSMALLINT Precision, SQLSMALLINT Scale,
    _Inout_updates_bytes_opt_(Length) SQLPOINTER Data, _Inout_opt_ SQLLEN * StringLength,
    _Inout_opt_ SQLLEN * Indicator) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLSetEnvAttr(SQLHENV EnvironmentHandle,
    SQLINTEGER Attribute, _In_reads_bytes_opt_(StringLength) SQLPOINTER Value,
    SQLINTEGER StringLength) {
    return SQL_SUCCESS;
}
#endif /* ODBCVER >= 0x0300 */

__declspec(deprecated("ODBC API: SQLSetParam is deprecated. Please use SQLBindParameter instead."))
SQLRETURN  SQL_API SQLSetParam(SQLHSTMT StatementHandle,
    SQLUSMALLINT ParameterNumber, SQLSMALLINT ValueType,
    SQLSMALLINT ParameterType, SQLULEN LengthPrecision,
    SQLSMALLINT ParameterScale, _In_reads_(_Inexpressible_(*StrLen_or_Ind)) SQLPOINTER ParameterValue,
    _Inout_ SQLLEN * StrLen_or_Ind) {
    return SQL_SUCCESS;
}

#if (ODBCVER >= 0x0300)
SQLRETURN  SQL_API SQLSetStmtAttr(SQLHSTMT StatementHandle,
    SQLINTEGER Attribute, _In_reads_(_Inexpressible_(StringLength)) SQLPOINTER Value,
    SQLINTEGER StringLength) {
    return SQL_SUCCESS;
}
__declspec(deprecated("ODBC API: SQLSetStmtOption is deprecated. Please use SQLSetStmtAttr instead."))
#endif

SQLRETURN  SQL_API SQLSetStmtOption(SQLHSTMT StatementHandle,
    SQLUSMALLINT Option, SQLULEN Value) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLSpecialColumns(SQLHSTMT StatementHandle,
    SQLUSMALLINT IdentifierType,
    _In_reads_opt_(NameLength1) SQLCHAR * CatalogName, SQLSMALLINT NameLength1,
    _In_reads_opt_(NameLength2) SQLCHAR * SchemaName, SQLSMALLINT NameLength2,
    _In_reads_opt_(NameLength3) SQLCHAR * TableName, SQLSMALLINT NameLength3,
    SQLUSMALLINT Scope, SQLUSMALLINT Nullable) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLStatistics(SQLHSTMT StatementHandle,
    _In_reads_opt_(NameLength1) SQLCHAR * CatalogName, SQLSMALLINT NameLength1,
    _In_reads_opt_(NameLength2) SQLCHAR * SchemaName, SQLSMALLINT NameLength2,
    _In_reads_opt_(NameLength3) SQLCHAR * TableName, SQLSMALLINT NameLength3,
    SQLUSMALLINT Unique, SQLUSMALLINT Reserved) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLTables(SQLHSTMT StatementHandle,
    _In_reads_opt_(NameLength1) SQLCHAR * CatalogName, SQLSMALLINT NameLength1,
    _In_reads_opt_(NameLength2) SQLCHAR * SchemaName, SQLSMALLINT NameLength2,
    _In_reads_opt_(NameLength3) SQLCHAR * TableName, SQLSMALLINT NameLength3,
    _In_reads_opt_(NameLength4) SQLCHAR * TableType, SQLSMALLINT NameLength4) {
    return SQL_SUCCESS;
}

SQLRETURN  SQL_API SQLTransact(SQLHENV EnvironmentHandle,
    SQLHDBC ConnectionHandle, SQLUSMALLINT CompletionType) {
    return SQL_SUCCESS;
}