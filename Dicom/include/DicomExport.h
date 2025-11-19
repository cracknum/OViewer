/*--------------------------------------------------------------
 * do not modify this file, which is generate by cmake
 *-------------------------------------------------------------*/
#ifndef DICOM_EXPORT_OPENGL_H
#define DICOM_EXPORT_OPENGL_H

#ifdef DICOM_EXPORTS
    #define DICOM_API __declspec(dllexport)
#else
    #define DICOM_API __declspec(dllimport)
#endif

#endif //DICOM_EXPORT_OPENGL_H
