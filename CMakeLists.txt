cmake_minimum_required(VERSION 3.19...4.0)

# OPENGL Viewer
project(OViewer)

find_package(Qt5 COMPONENTS Widgets Core Gui OpenGL)
find_package(CUDAToolkit REQUIRED) # image process
find_package(ITK CONFIG REQUIRED) # read dicom files

set(export_config_file_path ${CMAKE_CURRENT_SOURCE_DIR}/Cmake/Export.h.in)

add_subdirectory(Dicom)
add_subdirectory(App)
add_subdirectory(Common)






