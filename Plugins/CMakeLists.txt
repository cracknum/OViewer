cmake_minimum_required(VERSION ${MINI_VERSION})

project(Plugins)

file(GLOB SUBDIRS LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/*)

set(PLUGIN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/plugins")

foreach(dir IN LISTS SUBDIRS)
	if(IS_DIRECTORY ${dir})
		add_subdirectory(${dir})
		get_filename_component(plugin_name ${dir} NAME)
		message(STATUS "enable plugin: ${plugin_name}")
	endif()
endforeach()
