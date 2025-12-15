cmake_minimum_required(VERSION 3.19...4.0)

project(Primitive LANGUAGES CXX)

file(GLOB_RECURSE Header_files "include/*.hpp" "include/*.ipp")
file(GLOB_RECURSE Source_files "src/*.cpp")

if (NOT TARGET glm::glm)
    find_package(glm CONFIG REQUIRED)
endif ()

if (NOT "${Source_files}")
    add_library(${PROJECT_NAME} SHARED ${Header_files} ${Source_files} resource.qrc)
    target_link_libraries(${PROJECT_NAME} PUBLIC
            Common GLObject
            spdlog::spdlog
            glm::glm
            OpenGLCore::Object
    )
    target_include_directories(${PROJECT_NAME} PUBLIC include)
    export_model(${export_config_file_path} ${PROJECT_NAME})

    if (ENABLE_DEBUG)
        target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
    endif ()
endif ()
