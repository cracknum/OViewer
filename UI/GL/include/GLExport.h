//
// Created by 87068 on 25-10-13.
//

#ifndef GL_EXPORT_H
#define GL_EXPORT_H

#ifdef _WIN32
    #ifdef GL_EXPORTS
        #define GL_API __declspec(dllexport)
    #else
        #define GL_API __declspec(dllimport)
    #endif
#else
    #define MYLIB_API __attribute__((visibility("default")))
#endif

#endif // ELEMENTS_EXPORT_H
