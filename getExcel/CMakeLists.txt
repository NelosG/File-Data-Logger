cmake_minimum_required(VERSION 3.16)
project(untitled)


#=======================================================================================================================
# Output Directories.
#=======================================================================================================================
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/source)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/source)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/source)




set(CMAKE_CXX_STANDARD 17)
add_subdirectory(OpenXLSX-master)
add_library(getExcel getExcelList.h getExcelList.cpp)
target_link_libraries(getExcel PRIVATE OpenXLSX-static mpr.dll)
target_compile_definitions(getExcel PRIVATE OPENXLSX_STATIC_DEFINE)
add_custom_command(
        TARGET getExcel
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/mpr.dll ${CMAKE_BINARY_DIR}/output/source
)