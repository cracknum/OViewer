//
// Created by 87068 on 25-10-13.
//

#ifndef WIDGETS_EXPORT_H
#define WIDGETS_EXPORT_H

#ifdef _WIN32
    #ifdef WIDGETS_EXPORTS
        #define WIDGETS_API __declspec(dllexport)
    #else
        #define WIDGETS_API __declspec(dllimport)
    #endif
#else
    #define MYLIB_API __attribute__((visibility("default")))
#endif

#endif // ELEMENTS_EXPORT_H
