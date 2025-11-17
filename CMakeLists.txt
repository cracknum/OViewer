cmake_minimum_required(VERSION 3.19...4.0)

# OPENGL Viewer
project(OViewer)

find_package(Qt5 COMPONENTS Widgets Core Gui OpenGL)
find_package(CUDAToolkit REQUIRED) # image process
find_package(VTK REQUIRED) # read dicom files


add_subdirectory(Dicom)






