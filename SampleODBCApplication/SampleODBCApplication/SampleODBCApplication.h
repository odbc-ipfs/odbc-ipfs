#pragma once
#include<windows.h>
#include <iostream>
#include<stdio.h>

#include <sql.h>
#include <sqlext.h>

void listSQLDrivers();
void listDataSources();
SQLHDBC conn(SQLHENV* env);
void getInfo(SQLHDBC dbc);
void getData(SQLHDBC dbc);
void getData2(SQLHDBC dbc);

void printSQLCharString(SQLWCHAR* sqlCharString, SQLSMALLINT length);
SQLWCHAR* stringToSQLWCHAR(char* str);