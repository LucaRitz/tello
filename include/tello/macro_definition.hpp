#pragma once
#if defined(_MSC_VER)
// Windows
    #ifdef MAKE_DLL
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT __declspec(dllimport)
    #endif
#elif defined(__GNUC__)
//  GCC
    #define EXPORT __attribute__((visibility("default")))
#else
    //  do nothing and hope for the best?
    #define EXPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif