#pragma once
#include<windows.h>
#include <iostream>
#include<stdio.h>

#include <sql.h>
#include <sqlext.h>

void listSQLDrivers();
void listDataSources();
SQLHDBC conn(SQLHENV* env);
void getData(SQLHDBC dbc, int inputInt, char* inputStr);