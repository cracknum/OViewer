cmake_minimum_required(VERSION 3.19...4.0)

project(Grinding LANGUAGES CXX)

set(MODULE_NAME ${PROJECT_NAME})

add_subdirectory(Base)
add_subdirectory(Shaders)

# ------------- Assembly Test-------------------
message(STATUS "${MODULE_NAME}_TESTING checking")

if(${MODULE_NAME}_TESTING)
    if(NOT TARGET gtest_main)
        find_package(Gtest CONFIG REQUIRED)
    endif()

    message(STATUS "Enable test: ${MODULE_NAME}_TESTING")

    add_executable(${MODULE_NAME}_TESTING
        tests/GrindingTest.cpp
    )
    target_link_libraries(${MODULE_NAME}_TESTING PRIVATE
        # ${MODULE_NAME}::Base ${MODULE_NAME}::Shaders
        GTest::gtest GTest::gtest_main
    )
    target_compile_definitions(${MODULE_NAME}_TESTING PRIVATE TESTS_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()