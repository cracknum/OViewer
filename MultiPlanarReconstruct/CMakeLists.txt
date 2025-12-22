cmake_minimum_required(VERSION 3.19...4.0)

project(MultiPlanarReconstruct LANGUAGES CXX)

if (NOT TARGET Qt5::Core)
    find_package(Qt5 REQUIRED COMPONENTS Core)
endif ()

if (NOT TARGET VTK::CommonDataModel)
    find_package(VTK REQUIRED COMPONENTS CommonDataModel CommonExecutionModel)
endif ()

set(MODULE_NAME ${PROJECT_NAME})

add_subdirectory(Base)
add_subdirectory(Rendering)
