cmake_minimum_required(VERSION 3.19...4.0)

project(Base LANGUAGES CXX)

file(GLOB_RECURSE Header_files "include/*.hpp" "include/*.ipp")
file(GLOB_RECURSE Source_files "src/*.cpp")

if (NOT TARGET VTK::RenderingCore)
    find_package(VTK REQUIRED COMPONENTS RenderingCore)
endif ()

set(library_name ${MODULE_NAME}${PROJECT_NAME})
add_library(${library_name} SHARED ${Header_files} ${Source_files})
target_link_libraries(${library_name} PUBLIC
        Qt5::Core
        Common
        VTK::RenderingCore
)

add_library(${MODULE_NAME}::${PROJECT_NAME} ALIAS ${library_name})

target_include_directories(${library_name} PUBLIC include)

export_model(${export_config_file_path} ${PROJECT_NAME} ${library_name})

# --------- enable testing ----------
message(STATUS "${MODULE_NAME}_${PROJECT_NAME}_TESTING checking")
if (${library_name}_TESTING)
    message(STATUS "Enable test: ${MODULE_NAME}_${PROJECT_NAME}_TESTING")
    if (NOT TARGET gtest_main)
        find_package(Gtest CONFIG REQUIRED)
    endif ()

    add_executable(${library_name}_TESTING tests/SlicedGeometry_unittest.cpp)
    target_link_libraries(${library_name}_TESTING PRIVATE ${MODULE_NAME}::${PROJECT_NAME} GTest::gtest GTest::gtest_main)
endif ()
