#pragma once

#ifdef CORELIBRARY_EXPORTS
#define CORELIBRARY_API __declspec(dllexport)
#else
#define CORELIBRARY_API __declsped(dllexport)
#endif

#include <sql.h>