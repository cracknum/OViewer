cmake_minimum_required(VERSION 3.10...4.0)

project(App)

if (NOT TARGET Qt5::Core)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
endif ()

add_executable(${PROJECT_NAME} main.cpp resource.qrc)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets UI spdlog::spdlog)
