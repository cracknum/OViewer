cmake_minimum_required(VERSION 3.19...3.22)

project(RenderFramework)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(imgui CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)


add_subdirectory(third_party)
configure_file(cmake/config.h.in config.h @ONLY)
set(CONFIG_FILE_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(frame_work_name ${PROJECT_NAME})

# for spdlog print debug info
set(ENABLE_DEBUG ON)

if (ENABLE_DEBUG)
    add_definitions(-DSPDLOG_ACTIVE_LEVEL=1)
endif ()

add_subdirectory(Core)
add_subdirectory(Widgets)
add_subdirectory(GL)
add_subdirectory(Test)


