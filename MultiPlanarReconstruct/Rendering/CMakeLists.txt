cmake_minimum_required(VERSION 3.19...4.0)

project(Rendering LANGUAGES CXX)

file(GLOB_RECURSE Header_files "include/*.hpp" "include/*.ipp")
file(GLOB_RECURSE Source_files "src/*.cpp")

if (NOT TARGET VTK::RenderingCore)
    find_package(VTK REQUIRED COMPONENTS RenderingCore InteractionWidgets)
endif ()

add_library(${PROJECT_NAME} SHARED ${Header_files} ${Source_files})
target_link_libraries(${PROJECT_NAME} PUBLIC
        Common Base
        VTK::RenderingCore VTK::InteractionWidgets
)

add_library(${MODULE_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC include)

export_model(${export_config_file_path} ${PROJECT_NAME})

# --------- enable testing ----------
message(STATUS "${MODULE_NAME}_${PROJECT_NAME}_TESTING checking")
if (${MODULE_NAME}_${PROJECT_NAME}_TESTING)
    message(STATUS "Enable test: ${MODULE_NAME}_${PROJECT_NAME}_TESTING")
    if (NOT TARGET gtest_main)
        find_package(Gtest CONFIG REQUIRED)
    endif ()

    # add_executable(${PROJECT_NAME}_TEST tests/SlicedGeometry_unittest.cpp)
    # target_link_libraries(${PROJECT_NAME}_TEST PRIVATE ${MODULE_NAME}::${PROJECT_NAME} GTest::gtest GTest::gtest_main)
endif ()
