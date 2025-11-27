/*--------------------------------------------------------------
 * do not modify this file, which is generate by cmake
 *-------------------------------------------------------------*/
#ifndef PRIMITIVE_EXPORT_OPENGL_H
#define PRIMITIVE_EXPORT_OPENGL_H

#ifdef PRIMITIVE_EXPORTS
    #define PRIMITIVE_API __declspec(dllexport)
#else
    #define PRIMITIVE_API __declspec(dllimport)
#endif

#endif //PRIMITIVE_EXPORT_OPENGL_H
