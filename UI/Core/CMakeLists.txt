
project(Core)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)

set(source_files
        src/UIObject.cpp
)

set(header_files
        include/CoreExport.h
        include/IEventObserver.h
        include/EventObject.h
        include/UIObject.h
)

add_library(${PROJECT_NAME} SHARED ${source_files} ${header_files})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)
target_compile_definitions(${PROJECT_NAME} PRIVATE CORE_EXPORTS)

add_library(${frame_work_name}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})