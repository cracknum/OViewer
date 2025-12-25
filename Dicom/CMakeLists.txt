cmake_minimum_required(VERSION 3.19...4.0)

project(Dicom LANGUAGES CXX)

file(GLOB_RECURSE Header_files "include/*.hpp" "include/*.ipp")
file(GLOB_RECURSE Source_files "src/*.cpp")

if(NOT TARGET Qt5::Core)
    find_package(Qt5 REQUIRED COMPONENTS Core)
endif()

if(NOT TARGET ITKIOImageBase)
    find_package(ITK CONFIG REQUIRED)
endif()

add_library(${PROJECT_NAME} SHARED ${Header_files} ${Source_files})
target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt5::Core
    Common
    ITKCommon ITKIOImageBase ITKIOGDCM ITKVTK
    VTK::CommonDataModel VTK::CommonExecutionModel VTK::IOImage
)
target_include_directories(${PROJECT_NAME} PUBLIC include)
export_model(${export_config_file_path} ${PROJECT_NAME} ${PROJECT_NAME})
