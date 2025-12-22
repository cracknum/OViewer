/*--------------------------------------------------------------
 * do not modify this file, which is generate by cmake
 *-------------------------------------------------------------*/
#ifndef RENDERING_EXPORT_OPENGL_H
#define RENDERING_EXPORT_OPENGL_H

#ifdef RENDERING_EXPORTS
    #define RENDERING_API __declspec(dllexport)
#else
    #define RENDERING_API __declspec(dllimport)
#endif

#endif //RENDERING_EXPORT_OPENGL_H
