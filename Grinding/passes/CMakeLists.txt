cmake_minimum_required(VERSION 3.19...4.0)

project(Passes LANGUAGES CXX)

file(GLOB_RECURSE Header_files "include/*.hpp" "include/*.ipp")
file(GLOB_RECURSE Source_files "src/*.cpp")

if(NOT TARGET VTK::RenderingCore)
    find_package(VTK 9.5 REQUIRED COMPONENTS RenderingCore RenderingOpenGL2)
endif()

message(STATUS "find vtk ${VTK_VERSION}")

set(library_name ${MODULE_NAME}${PROJECT_NAME})
add_library(${library_name} SHARED ${Header_files} ${Source_files})
target_link_libraries(${library_name} PUBLIC
    Common
    Grinding::Base
    Grinding::Shaders
    VTK::RenderingCore
    VTK::RenderingOpenGL2
)

add_library(${MODULE_NAME}::${PROJECT_NAME} ALIAS ${library_name})

target_include_directories(${library_name} PUBLIC include PRIVATE shader)
option(GRINDING_OPAQUE_DEPTH_RENDER_PASS_DEBUG "enable debug OpaqueDepthRenderPass class" OFF)

if(GRINDING_OPAQUE_DEPTH_RENDER_PASS_DEBUG)
    target_compile_definitions(${library_name} PRIVATE "GRINDING_OPAQUE_DEPTH_RENDER_PASS_DEBUG")
endif()

option(GRINDING_UPDATE_TOOL_RENDER_PASS_DEBUG "enable debug update tool render pass class" OFF)
if(GRINDING_UPDATE_TOOL_RENDER_PASS_DEBUG)
    target_compile_definitions(${library_name} PRIVATE GRINDING_UPDATE_TOOL_RENDER_PASS_DEBUG)
endif()

option(GRINDING_ABUFFER_RENDER_PASS_DEBUG "enable debug a buffer render pass class" OFF)
if(GRINDING_ABUFFER_RENDER_PASS_DEBUG)
    target_compile_definitions(${library_name} PRIVATE GRINDING_ABUFFER_RENDER_PASS_DEBUG)
endif()

option(GRINDING_COMPOSIT_RENDER_PASS_DEBUG "enable debug composit render pass class" OFF)
if(GRINDING_COMPOSIT_RENDER_PASS_DEBUG)
    target_compile_definitions(${library_name} PRIVATE GRINDING_COMPOSIT_RENDER_PASS_DEBUG)
endif()


set(Shader_PATH ${CMAKE_CURRENT_SOURCE_DIR}/shader/)
target_compile_definitions(${library_name} PRIVATE ASSERT_PATH="${Shader_PATH}")

export_model(${export_config_file_path} ${PROJECT_NAME} ${library_name})