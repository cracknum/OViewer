cmake_minimum_required(VERSION 3.19...4.0)

project(Testing LANGUAGES CXX CUDA)

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
    find_package(Stb REQUIRED)
endif ()

if (ENABLE_IMAGE_FILTER_TEST)
    set(TEST_LIB Filters Dicom Data VTK::IOImage)
    set(Testing_files FiltersTest/ImageResliceTest.cpp)
    message(STATUS "add filters test")
endif ()


if (NOT "${Testing_files}")
    add_executable(${ModuleName}${PROJECT_NAME} ${Testing_files})
    target_link_libraries(${ModuleName}${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main ${TEST_LIB})
    target_include_directories(${ModuleName}${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
    include(GoogleTest)

    gtest_discover_tests(${ModuleName}${PROJECT_NAME})
endif ()

