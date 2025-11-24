cmake_minimum_required(VERSION 3.19...4.0)

project(Testing)

enable_testing()

if (NOT TARGET gtest_main)
    find_package(Gtest CONFIG REQUIRED)
endif ()

set(Dicom_model_test_files
        DicomTest/dicomTest.cpp
)

set(Testing_files
        ${Dicom_model_test_files}
)

add_executable(${PROJECT_NAME} ${Testing_files})
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main Dicom)

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME})

