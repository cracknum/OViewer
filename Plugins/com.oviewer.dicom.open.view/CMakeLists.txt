cmake_minimum_required(VERSION 3.19...4.0)

project(com_oviewer_dicom_open_view)

if(NOT TARGET Qt5::Core)
	find_package(Qt5 COMPONENTS Core Gui Widgets)
endif()
set(CMAKE_AUTORCC ON)

file(GLOB source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB header_files ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(${PROJECT_NAME} MODULE ${source_files} ${header_files} resources/Resource.qrc)
target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt5::Core Qt5::Gui Qt5::Widgets
	Plugins::ViewInterface
)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}"
    PREFIX "" 
)
message(STATUS "set plugin output: ${PLUGIN_OUTPUT_DIR}")
file(GLOB MOC_FILES "${CMAKE_CURRENT_BINARY_DIR}/mocs_compilation_*.cpp")
if(MOC_FILES)
    set_source_files_properties(${MOC_FILES} PROPERTIES VS_FILTER "Generated Files")
endif()
