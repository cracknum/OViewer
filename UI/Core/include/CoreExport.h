//
// Created by 87068 on 25-10-13.
//

#ifndef CORE_EXPORT_H
#define CORE_EXPORT_H

#ifdef _WIN32
    #ifdef CORE_EXPORTS
        #define CORE_API __declspec(dllexport)
    #else
        #define CORE_API __declspec(dllimport)
    #endif
#else
    #define MYLIB_API __attribute__((visibility("default")))
#endif

#endif //CORE_EXPORT_H
