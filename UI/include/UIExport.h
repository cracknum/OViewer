/*--------------------------------------------------------------
 * do not modify this file, which is generate by cmake
 *-------------------------------------------------------------*/
#ifndef UI_EXPORT_OPENGL_H
#define UI_EXPORT_OPENGL_H

#ifdef UI_EXPORTS
    #define UI_API __declspec(dllexport)
#else
    #define UI_API __declspec(dllimport)
#endif

#endif //UI_EXPORT_OPENGL_H
