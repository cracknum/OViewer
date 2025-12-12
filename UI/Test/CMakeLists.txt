project(TestMain)

message(STATUS "--------------------TEST")

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${frame_work_name}::Widgets)