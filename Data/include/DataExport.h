/*--------------------------------------------------------------
 * do not modify this file, which is generate by cmake
 *-------------------------------------------------------------*/
#ifndef DATA_EXPORT_OPENGL_H
#define DATA_EXPORT_OPENGL_H

#ifdef DATA_EXPORTS
    #define DATA_API __declspec(dllexport)
#else
    #define DATA_API __declspec(dllimport)
#endif

#endif //DATA_EXPORT_OPENGL_H
