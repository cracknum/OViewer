cmake_minimum_required(VERSION 3.19...4.0)

project(com_oviewer_interaface_view)

if(NOT TARGET Qt5::Core)
	find_package(Qt5 COMPONENTS Core)
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE Qt5::Core)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(Plugins::ViewInterface ALIAS ${PROJECT_NAME})

set(library_name ${MODULE_NAME}${PROJECT_NAME})
export_interface(${export_config_file_path} ${PROJECT_NAME} ${library_name})

