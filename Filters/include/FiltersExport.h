/*--------------------------------------------------------------
 * do not modify this file, which is generate by cmake
 *-------------------------------------------------------------*/
#ifndef FILTERS_EXPORT_OPENGL_H
#define FILTERS_EXPORT_OPENGL_H

#ifdef FILTERS_EXPORTS
    #define FILTERS_API __declspec(dllexport)
#else
    #define FILTERS_API __declspec(dllimport)
#endif

#endif //FILTERS_EXPORT_OPENGL_H
