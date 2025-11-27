cmake_minimum_required(VERSION 3.19...4.0)

project(UI LANGUAGES CXX)

file(GLOB_RECURSE Header_files "include/*.hpp" "include/*.ipp" "include/*.h")
file(GLOB_RECURSE Source_files "src/*.cpp")

if (NOT TARGET Qt5::Core)
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets OpenGL)
endif ()

if (NOT TARGET SARibbonBar::SARibbonBar)
    set(CMAKE_PREFIX_PATH $ENV{SARibbonBar_DIR};${CMAKE_PREFIX_PATH})
    find_package(SARibbonBar REQUIRED)
endif ()

if (NOT VTK_FOUND)
    find_package(VTK REQUIRED)
endif ()

if (NOT TARGET spdlog::spdlog)
    find_package(spdlog CONFIG REQUIRED)
endif ()

if (NOT "${Source_files}")
    set(vtk_libs VTK::FiltersProgrammable VTK::GUISupportQt)
    add_library(${PROJECT_NAME} SHARED ${Header_files} ${Source_files} ${CMAKE_CURRENT_SOURCE_DIR}/resource.qrc)
    target_link_libraries(${PROJECT_NAME} PUBLIC
            Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL
            Common Dicom
            SARibbonBar::SARibbonBar
            spdlog::spdlog
            ${vtk_libs}
    )
    target_include_directories(${PROJECT_NAME}
            PUBLIC include
            PRIVATE ${SARibbonBar_INCLUDE_DIR})
            
    export_model(${export_config_file_path} ${PROJECT_NAME})
    group_sources_by_directory(${PROJECT_NAME})

    vtk_module_autoinit(TARGETS ${PROJECT_NAME} MODULES  ${vtk_libs})
endif ()

