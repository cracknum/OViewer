/*--------------------------------------------------------------
 * do not modify this file, which is generate by cmake
 *-------------------------------------------------------------*/
#ifndef PASSES_EXPORT_OPENGL_H
#define PASSES_EXPORT_OPENGL_H

#ifdef PASSES_EXPORTS
    #define PASSES_API __declspec(dllexport)
#else
    #define PASSES_API __declspec(dllimport)
#endif

#endif //PASSES_EXPORT_OPENGL_H
