#pragma once
#include<windows.h>
#include <iostream>
#include<stdio.h>

#include <sql.h>
#include <sqlext.h>

void listSQLDrivers();
void listDataSources();
SQLHDBC conn();
void getInfo(SQLHDBC dbc);
void getData(SQLHDBC dbc);

void printSQLCharString(SQLWCHAR* sqlCharString, SQLSMALLINT length);
SQLWCHAR* stringToSQLWCHAR(char* str);