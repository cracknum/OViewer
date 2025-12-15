
project(GL)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)

set(source_files
        src/UIContext.cpp
)

add_library(${PROJECT_NAME} SHARED ${source_files})
target_include_directories(${PROJECT_NAME} PUBLIC include  ${CONFIG_FILE_PATH})
target_link_libraries(
        ${PROJECT_NAME} PUBLIC
        imgui::imgui
        Core
        OpenGLCore::Context
)

target_compile_definitions(${PROJECT_NAME} PRIVATE GL_EXPORTS)
add_library(${frame_work_name}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})