cmake_minimum_required(VERSION 3.19...4.0)

# OPENGL Viewer
project(OViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CUDAToolkit REQUIRED) # image process
find_package(ITK CONFIG REQUIRED) # read dicom files
find_package(spdlog CONFIG REQUIRED) # for log info
find_package(VTK REQUIRED)
find_package(OpenGL REQUIRED)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(export_config_file_path ${CMAKE_CURRENT_SOURCE_DIR}/Cmake/Export.h.in)
include(cmake/ExportFunction.cmake)
include(cmake/files_group.cmake)
include(cmake/resource_copy.cmake)


# for spdlog print debug info
set(ENABLE_DEBUG ON)

if (ENABLE_DEBUG)
    add_definitions(-DSPDLOG_ACTIVE_LEVEL=1)
endif ()

add_subdirectory(Dicom)
add_subdirectory(App)
add_subdirectory(Common)
add_subdirectory(GLObject)
add_subdirectory(Primitive)
add_subdirectory(Data)
add_subdirectory(Filters)
add_subdirectory(third_parties/RenderFramework)
add_subdirectory(RenderingGL)

option(enable_testing OFF)

if (enable_testing)
    add_subdirectory(Testing)
endif ()






