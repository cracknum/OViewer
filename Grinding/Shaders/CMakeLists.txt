cmake_minimum_required(VERSION 3.19...4.0)

project(SDFShaders)

file(GLOB vertex_shader_files "Vertex/*.vert")
file(GLOB geometry_shader_files "Geometry/*.geom")
file(GLOB fragment_shader_files "Fragment/*.frag")
file(GLOB sdf_files "SDF/*.glsl")
file(GLOB include_files "include/*.h")

set(source_files
    ${vertex_shader_files}
    ${geometry_shader_files}
    ${fragment_shader_files}
    ${sdf_files}
    ${include_files}
)

set(library_name ${MODULE_NAME}${PROJECT_NAME})
add_library(${library_name} INTERFACE ${source_files})

target_include_directories(${library_name} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Vertex>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Geometry>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Fragment>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/SDF>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:share/${MODULE_NAME}/include>
)
add_library(${MODULE_NAME}::${PROJECT_NAME} ALIAS ${library_name})


message(STATUS "${library_name}_TESTING checking")
if (${library_name}_TESTING)
    message(STATUS "Enable test: ${library_name}_TESTING")
    if (NOT TARGET gtest_main)
        find_package(Gtest CONFIG REQUIRED)
    endif ()

    add_executable(SDFImport_TESTING tests/SDFImportTest.cpp)
    target_link_libraries(SDFImport_TESTING PRIVATE ${MODULE_NAME}::${PROJECT_NAME} GTest::gtest GTest::gtest_main)
    target_compile_definitions(SDFImport_TESTING PRIVATE TESTS_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif ()