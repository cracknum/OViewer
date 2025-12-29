# CMakeList.txt: CMakeProject1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("GrindingSDF")

add_library(GrindingLib SHARED IMPORTED)
set_target_properties(GrindingLib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/lib/GrindSimulater/include)
set_target_properties(GrindingLib PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/GrindSimulater/bin/GrindSimulater.dll)
set_target_properties(GrindingLib PROPERTIES IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/GrindSimulater/lib/GrindSimulater.lib)

add_subdirectory(GrindingSDF)
