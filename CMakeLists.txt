cmake_minimum_required(VERSION 3.10)
project(jetsonGst)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include("cmake/CompilerWarnings.cmake")
include("cmake/git-helper.cmake")

set(default_build_type "Release") 
#include("cmake/StandardProjectSettings.cmake")
set(CMAKE_CXX_VISIBILITY_PRESET default)

find_git_settings()

add_library(project_options INTERFACE)

set( CMAKE_POSITION_INDEPENDENT_CODE ON )


# build sources located in src folder
add_subdirectory(src)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)