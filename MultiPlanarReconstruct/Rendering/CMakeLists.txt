cmake_minimum_required(VERSION 3.19...4.0)

project(Rendering LANGUAGES CXX)

file(GLOB_RECURSE Header_files "include/*.hpp" "include/*.ipp")
file(GLOB_RECURSE Source_files "src/*.cpp")

if (NOT TARGET VTK::RenderingCore)
    find_package(VTK REQUIRED COMPONENTS CommonCore RenderingCore InteractionWidgets ImagingCore IOImage InteractionStyle)
endif ()

if(TARGET MultiPlanarReconstruct::Base)
    message(STATUS "find target MultiPlanarReconstruct::Base")
endif()

add_library(${PROJECT_NAME} SHARED ${Header_files} ${Source_files})
target_link_libraries(${PROJECT_NAME} PUBLIC
        Common MultiPlanarReconstruct::Base
        VTK::RenderingCore VTK::InteractionWidgets VTK::ImagingCore VTK::IOImage VTK::InteractionStyle
)

add_library(${MODULE_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC include)

export_model(${export_config_file_path} ${PROJECT_NAME} ${PROJECT_NAME})
vtk_module_autoinit(TARGETS ${PROJECT_NAME} MODULES VTK::RenderingCore VTK::InteractionWidgets VTK::ImagingCore VTK::IOImage VTK::InteractionStyle)

# --------- enable testing ----------
message(STATUS "${MODULE_NAME}_${PROJECT_NAME}_TESTING checking")
if (${MODULE_NAME}_${PROJECT_NAME}_TESTING)
    message(STATUS "Enable test: ${MODULE_NAME}_${PROJECT_NAME}_TESTING")
    if (NOT TARGET gtest_main)
        find_package(Gtest CONFIG REQUIRED)
    endif ()
    add_definitions(-DSPDLOG_ACTIVE_LEVEL=1)

    add_executable(${MODULE_NAME}_${PROJECT_NAME}_TESTING tests/MPRSliceWidgetTest.cpp)
    target_link_libraries(${MODULE_NAME}_${PROJECT_NAME}_TESTING PRIVATE ${MODULE_NAME}::${PROJECT_NAME} ${VTK_LIBRARIES} GTest::gtest GTest::gtest_main)

endif ()
