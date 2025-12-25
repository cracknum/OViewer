cmake_minimum_required(VERSION 3.19...4.0)

project(SDFShaders)

file(GLOB vertex_shader_files "Vertex/*.vert")
file(GLOB geometry_shader_files "Geometry/*.geom")
file(GLOB fragment_shader_files "Fragment/*.frag")
file(GLOB sdf_files "SDF/*.glsl")

set(source_files 
    ${vertex_shader_files} 
    ${geometry_shader_files} 
    ${fragment_shader_files} 
    ${sdf_files}
)

add_library(${MODULE_NAME}_${PROJECT_NAME} INTERFACE ${source_files})

target_include_directories(${MODULE_NAME}_${PROJECT_NAME} INTERFACE Vertex Geometry Fragment)
add_library(${MODULE_NAME}::${PROJECT_NAME} ALIAS ${MODULE_NAME}_${PROJECT_NAME})
