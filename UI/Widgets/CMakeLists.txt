
project(Widgets)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)

set(source_files
        src/SceneView.cpp
        src/MenuBar.cpp
        src/Application.cpp
        src/OpenGLWindow.cpp
        src/DataManagerWidget.cpp
        src/ImageNavigatorWidget.cpp
        src/ImagePropertiesWidget.cpp
        src/LogWidget.cpp
        src/PixelValueWidget.cpp
        src/OpenGLViewerWidget.cpp
        src/FileBrowserDialog.cpp
        src/SeriesSelectDialog.cpp
)

set(header_files
        include/SceneView.hpp
        include/MenuBar.h
        include/Application.hpp
        include/OpenGLWindow.hpp
        include/AbstractPropertiesWidget.h
        include/DataManagerWidget.h
        include/ImageNavigatorWidget.h
        include/ImagePropertiesWidget.h
        include/LogWidget.h
        include/PixelValueWidget.h
        include/OpenGLViewerWidget.h
        include/FileBrowserDialog.h
        include/SeriesSelectDialog.h
)

add_library(${PROJECT_NAME} SHARED ${source_files} ${header_files})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(
        ${PROJECT_NAME} PUBLIC
        glm::glm
        imgui::imgui
        ImGuiHelper
        Core
        GL
        spdlog::spdlog
        OpenGLCore::Object
        Dicom
)
target_compile_definitions(${PROJECT_NAME} PRIVATE WIDGETS_EXPORTS)
add_library(${frame_work_name}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})