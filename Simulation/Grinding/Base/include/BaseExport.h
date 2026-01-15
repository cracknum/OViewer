/*--------------------------------------------------------------
 * do not modify this file, which is generate by cmake
 *-------------------------------------------------------------*/
#ifndef BASE_EXPORT_OPENGL_H
#define BASE_EXPORT_OPENGL_H

#ifdef BASE_EXPORTS
    #define BASE_API __declspec(dllexport)
#else
    #define BASE_API __declspec(dllimport)
#endif

#endif //BASE_EXPORT_OPENGL_H
