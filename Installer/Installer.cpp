// Installer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <Odbcinst.h>


int main()
{
    LPCWSTR lpszDriver = L"ODBC IPFS\0Driver=odbc-ipfs.DLL\0\0";
    LPCWSTR lpszPathIn = L"D:/ODBCIPFSFOLDER"; //i tried \ but it caused a warning
    LPWSTR lpszPathOut = new TCHAR[100];
    WORD cbPathOutMax = 100;
    WORD* pcbPathOut = NULL;
    WORD fRequest = ODBC_INSTALL_COMPLETE;
    LPDWORD lpdwUsageCount = 0;
    SQLInstallDriverEx(lpszDriver, lpszPathIn, lpszPathOut, cbPathOutMax, pcbPathOut, fRequest, lpdwUsageCount);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
