cmake_minimum_required(VERSION 3.19...4.0)

project(MultiPlanarReconstruct LANGUAGES CXX)

file(GLOB_RECURSE Header_files "include/*.hpp" "include/*.ipp")
file(GLOB_RECURSE Source_files "src/*.cpp")

if(NOT TARGET Qt5::Core)
    find_package(Qt5 REQUIRED COMPONENTS Core)
endif()

if(NOT TARGET VTK::CommonDataModel)
    find_package(VTK REQUIRED COMPONENTS CommonDataModel CommonExecutionModel)
endif()

add_library(${PROJECT_NAME} SHARED ${Header_files} ${Source_files})
target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt5::Core
    Common
    VTK::CommonDataModel VTK::CommonExecutionModel
)
target_include_directories(${PROJECT_NAME} PUBLIC include)
export_model(${export_config_file_path} ${PROJECT_NAME})
