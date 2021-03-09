// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "core.h"
#include <iostream>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:

        OutputDebugString(L"DLLMain loaded\n"); // for DBMON

        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:

        OutputDebugString(L"DLLMain unloaded\n"); // for DBMON

        break;
    }

    return TRUE;
}

