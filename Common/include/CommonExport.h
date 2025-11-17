/*--------------------------------------------------------------
 * do not modify this file, which is generate by cmake
 *-------------------------------------------------------------*/
#ifndef Common_EXPORT_OPENGL_H
#define Common_EXPORT_OPENGL_H

#ifdef Common_EXPORTS
    #define Common_API __declspec(dllexport)
#else
    #define Common_API __declspec(dllimport)
#endif

#endif

#endif //Common_EXPORT_OPENGL_H
