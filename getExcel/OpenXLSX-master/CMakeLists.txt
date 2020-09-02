#=======================================================================================================================
# Preamble
#=======================================================================================================================
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(OpenXLSX VERSION 0.2.0 LANGUAGES CXX)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

#=======================================================================================================================
# Set C/C++ compiler version
#=======================================================================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(IGNORE_ME ${CMAKE_C_COMPILER})


#=======================================================================================================================
# Add build options
#=======================================================================================================================
option(BUILD_OBJECT_LIB "Build object library (not available in Xcode)" OFF)
option(ENABLE_UNICODE_FILENAMES "Enable handling of files with non-ASCII filenames (only relevant on Windows)" ON)

#=======================================================================================================================
# Add project subdirectories
#=======================================================================================================================
add_subdirectory(library)

