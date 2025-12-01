cmake_minimum_required(VERSION 3.19...4.0)

project(Testing)

enable_testing()

if (NOT TARGET gtest_main)
    find_package(Gtest CONFIG REQUIRED)
endif ()

set(Dicom_model_test_files
        DicomTest/dicomTest.cpp
)

set(Testing_files)
if (ENABLE_DICOM_TEST)
set(Testing_files
        ${Dicom_model_test_files}
)
set(TEST_LIB Dicom) 
endif()

if(ENABLE_IMAGE_FILTER_TEST)
  set(TEST_LIB Filters Dicom Data) 
  set(Testing_files FiltersTest/ImageResliceTest.cpp)
  message(STATUS "add filters test")
endif()


if (NOT "${Testing_files}")
add_executable(${PROJECT_NAME} ${Testing_files})
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main ${TEST_LIB})

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME})
endif()

