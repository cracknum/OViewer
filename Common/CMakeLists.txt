cmake_minimum_required(VERSION 3.10...4.0)

project(Common)

file(GLOB_RECURSE Header_files "include/*.hpp" "include/*.h")
file(GLOB_RECURSE Source_files "src/*.cpp" "src/*.cxx")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)