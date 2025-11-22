cmake_minimum_required(VERSION 3.19...4.0)

project(UI LANGUAGES CXX)

file(GLOB_RECURSE Header_files "include/*.hpp" "include/*.ipp" "include/*.h")
file(GLOB_RECURSE Source_files "src/*.cpp")

if (NOT TARGET Qt5::Core)
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets OpenGL)
endif ()

if (NOT "${Source_files}")
    add_library(${PROJECT_NAME} SHARED ${Header_files} ${Source_files})
    target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL Common GLObject)
    target_include_directories(${PROJECT_NAME} PUBLIC include)
    export_model(${export_config_file_path} ${PROJECT_NAME})
endif ()

