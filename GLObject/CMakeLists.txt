cmake_minimum_required(VERSION 3.19...4.0)

project(GLObject LANGUAGES CXX)

file(GLOB_RECURSE Header_files "include/*.hpp" "include/*.ipp")
file(GLOB_RECURSE Source_files "src/*.cpp")

if(NOT TARGET Qt5::Core)
    find_package(Qt5 REQUIRED COMPONENTS Core Gui OpenGL)
endif()

if (NOT TARGET glm::glm)
    find_package(glm CONFIG REQUIRED)
endif ()

if(NOT "${Source_files}")
    add_library(${PROJECT_NAME} SHARED ${Header_files} ${Source_files} resource.qrc)
    target_link_libraries(${PROJECT_NAME} PUBLIC
            Qt5::Core Qt5::Gui Qt5::OpenGL
            Common
            spdlog::spdlog
            glm::glm
    )
    target_include_directories(${PROJECT_NAME} PUBLIC include)
    export_model(${export_config_file_path} ${PROJECT_NAME})
endif()
