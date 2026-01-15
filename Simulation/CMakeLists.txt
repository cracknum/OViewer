cmake_minimum_required(VERSION 3.19...4.0)
project(Simulation)

set(MODULE_NAME ${PROJECT_NAME})

option(enable_springMass "enable spring mass module" OFF)

if(enable_springMass)
    add_subdirectory(SpringMass)
endif()

option(enable_grinding "enable grinding module" OFF)

if (enable_grinding)
    add_subdirectory(Grinding)
endif()