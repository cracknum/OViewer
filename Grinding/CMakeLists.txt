cmake_minimum_required(VERSION 3.19...4.0)

project(Grinding LANGUAGES CXX)

set(MODULE_NAME ${PROJECT_NAME})

add_subdirectory(Base)
add_subdirectory(Shaders)
add_subdirectory(passes)

macro(create_test test_name test_file_list)
    message(STATUS "Enable test: ${test_name}")

    add_executable(${test_name}
        ${test_file_list}
    )
    
    target_link_libraries(${test_name} PRIVATE
        ${ARGN}
    )

    target_compile_definitions(${test_name} PRIVATE TESTS_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests")
    vtk_module_autoinit(TARGETS ${test_name} MODULES ${VTK_DEPENDS})
endmacro()


# ------------- Assembly Test-------------------
message(STATUS "${MODULE_NAME}_TESTING checking")

if(${MODULE_NAME}_TESTING)
    if(NOT TARGET VTK::IOImage)
        find_package(
            VTK REQUIRED 
            COMPONENTS 
            IOImage IOGeometry RenderingOpenGL2 
            CommonCore InteractionStyle RenderingFreeType InteractionWidgets
            ImagingSources
            )
    endif()
    set(VTK_DEPENDS 
        VTK::IOImage VTK::IOGeometry VTK::RenderingOpenGL2 VTK::CommonCore 
        VTK::InteractionStyle VTK::RenderingFreeType VTK::InteractionWidgets
        VTK::ImagingSources
        )
    message(STATUS "Enable test: ${MODULE_NAME}_TESTING")

    add_executable(${MODULE_NAME}_TESTING
        tests/GrindingTest.cpp
    )
    
    target_link_libraries(${MODULE_NAME}_TESTING PRIVATE
        ${MODULE_NAME}::Passes
        ${VTK_DEPENDS}
    )
    target_compile_definitions(${MODULE_NAME}_TESTING PRIVATE TESTS_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests")
    vtk_module_autoinit(TARGETS ${MODULE_NAME}_TESTING MODULES ${VTK_DEPENDS})

    create_test(
        ${MODULE_NAME}_UpdateTool_TESTING 
        tests/UpdateToolRenderPassTest.cpp 
        ${MODULE_NAME}::Passes ${VTK_DEPENDS}
    )
endif()