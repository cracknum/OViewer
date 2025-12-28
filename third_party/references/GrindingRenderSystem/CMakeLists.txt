cmake_minimum_required(VERSION 3.12)
project(GrindingRenderSystem)

find_package(VTK REQUIRED COMPONENTS
  RenderingOpenGL2
  FiltersSources
  InteractionStyle
)
vtk_module_autoinit(
  TARGETS ${PROJECT_NAME}
  MODULES ${VTK_LIBRARIES}
)

add_executable(${PROJECT_NAME}
  main.cxx  # 可选：你自己的主程序
  vtkGrindingRenderPass.cxx
  GrindingComputeUpdater.cxx
)

target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})