# **********************************************************
#      2024 : Arthur JEULIN
#      Copyright (c) Coast Autonomous 
#      Created by Arthur JEULIN on 11/11/2024.
# **********************************************************
#
# Module : Learn RTMaps
#

cmake_minimum_required(VERSION 3.27.5)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 20)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Indiquez à CMake d'utiliser les outils de vcpkg
if(UNIX)
endif()
if(WIN32)
  #This allow us to use GeographicLib otherwise it will required v143
  set(CMAKE_GENERATOR_TOOLSET "v142") # Needed to build RTMaps for now
  set(VCPKG_PLATFORM_TOOLSET v142)
  set(VCPKG_CRT_LINKAGE dynamic)
  set(VCPKG_LIBRARY_LINKAGE dynamic)
endif()
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project(learn-rtmaps
        DESCRIPTION " Coast Autonomous RTMaps Tutorial"
        LANGUAGES CXX C
)

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

add_subdirectory(rtmaps)