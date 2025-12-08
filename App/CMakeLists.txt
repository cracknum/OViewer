cmake_minimum_required(VERSION 3.10...4.0)

project(App)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
        spdlog::spdlog
        RenderingGL
        Dicom RenderFramework::Widgets)
resource_copy()