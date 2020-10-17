cmake_minimum_required(VERSION 3.16)
project(untitled)
set(CMAKE_CXX_STANDARD 17)


#=======================================================================================================================
# Output Directories.
#=======================================================================================================================
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)


configure_file(CMakeLists.txt.in OpenXLSX-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/OpenXLSX-download )
if(result)
    message(FATAL_ERROR "CMake step for OpenXLSX failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/OpenXLSX-download )
if(result)
    message(FATAL_ERROR "Build step for OpenXLSX failed: ${result}")
endif()

add_subdirectory(
        ${CMAKE_CURRENT_BINARY_DIR}/OpenXLSX-src
        ${CMAKE_CURRENT_BINARY_DIR}/OpenXLSX-build
        EXCLUDE_FROM_ALL
)

add_library(getExcel getExcelList.h getExcelList.cpp)
target_link_libraries(getExcel PRIVATE OpenXLSX-static mpr.dll)
target_compile_definitions(getExcel PRIVATE OPENXLSX_STATIC_DEFINE)

add_custom_command(
        TARGET getExcel
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/mpr.dll ${CMAKE_BINARY_DIR}/output
)