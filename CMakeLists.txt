# CMake version

cmake_minimum_required(VERSION 3.21)

enable_testing()

#
# Declare project
#

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")

project(
  afro
  VERSION 0.1.0
  LANGUAGES CXX
  DESCRIPTION "A procedural material creating software package")


#
# Configure vcpkg
#

set(CMAKE_CXX_STANDARD 20)

#
# CMake modules
#

# Standard modules

# Custom modules

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(cmake/HealthCheck.cmake)
include(cmake/CompileOptions.cmake)
include(cmake/BuildInfo.cmake)

#
# Project configuration options
#

option(AFRO_USE_DOXYGEN "Add a doxygen target to generate the documentation" ON)
option(AFRO_WITH_TESTS "Add tests target" ON)
option(AFRO_WITH_DOCS "Add docs target" OFF)
option(AFRO_WITH_CYCLES "Build with cycles render engine" OFF)
option(AFRO_WITH_PYTHON "Build with python scripting" OFF)

#
# Health Checks

# Configure health check tools
enable_cppcheck(ON)
enable_clang_tidy(ON)

#
# Sub directories
#

add_subdirectory(deploy)
if(AFRO_WITH_DOCS)
  add_subdirectory(docs)
endif()
add_subdirectory(external)
add_subdirectory(source)
