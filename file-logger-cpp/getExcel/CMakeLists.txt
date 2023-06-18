cmake_minimum_required(VERSION 3.16)
project(untitled)
set(CMAKE_CXX_STANDARD 17)


#=======================================================================================================================
# Output Directories.
#=======================================================================================================================
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/source)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/source)



configure_file(CMakeLists.txt.in Excel-Logger-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Excel-Logger-download)

if (result)
    message(FATAL_ERROR "CMake step for Excel-Logger failed: ${result}")
endif ()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Excel-Logger-download)

if (result)
    message(FATAL_ERROR "Build step for Excel-Logger failed: ${result}")
endif ()

add_subdirectory(
        ${CMAKE_CURRENT_BINARY_DIR}/Excel-Logger-src
        ${CMAKE_CURRENT_BINARY_DIR}/Excel-Logger-build
)

add_library(getExcel getExcelList.h getExcelList.cpp)
target_link_libraries(getExcel PRIVATE logger mpr.dll)
