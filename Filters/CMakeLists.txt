cmake_minimum_required(VERSION 3.19...4.0)

project(Filters LANGUAGES CXX)

file(GLOB_RECURSE Header_files "include/*.hpp" "include/*.ipp" "include/*.h" "include/*.cuh")
file(GLOB_RECURSE Source_files "src/*.cpp" "src/*.cu")

if (NOT VTK_FOUND)
    find_package(VTK REQUIRED)
endif ()

if (NOT TARGET spdlog::spdlog)
    find_package(spdlog CONFIG REQUIRED)
endif ()

if(NOT TARGET glm::glm)
    find_package(glm CONFIG REQUIRED)
endif()

if(NOT TARGET CUDA::cudart)
    find_package(CUDAToolkit REQUIRED)
endif()

if (NOT "${Source_files}")
    set(vtk_libs VTK::CommonDataModel)
    add_library(${PROJECT_NAME} SHARED ${Header_files} ${Source_files})
    target_link_libraries(${PROJECT_NAME} PUBLIC
            Common Dicom Data
            spdlog::spdlog
            glm::glm
            ${vtk_libs}
            CUDA::cudart
    )
    target_include_directories(${PROJECT_NAME}
            PUBLIC include
            PRIVATE ${SARibbonBar_INCLUDE_DIR})
            
    export_model(${export_config_file_path} ${PROJECT_NAME})
    group_sources_by_directory(${PROJECT_NAME})

    vtk_module_autoinit(TARGETS ${PROJECT_NAME} MODULES  ${vtk_libs})
endif ()

