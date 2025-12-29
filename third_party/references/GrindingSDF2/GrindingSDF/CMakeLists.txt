cmake_minimum_required(VERSION 3.8)
project(GrindingSDF)
find_package(VTK 8.1 REQUIRED)

file(GLOB source_files "src/*.cpp")
add_executable(GrindingSDF ${source_files})
target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES} GrindingLib)
