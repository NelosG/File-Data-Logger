cmake_minimum_required(VERSION 3.16)
project(untitled)


#=======================================================================================================================
# Output Directories.
#=======================================================================================================================
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

#=======================================================================================================================
# Add build options
#=======================================================================================================================
option(ENABLE_UNICODE_FILENAMES "Enable handling of files with non-ASCII filenames (only relevant on Windows)" ON)



set(CMAKE_CXX_STANDARD 17)
add_subdirectory(library)
add_executable(Logger main.cpp getExcelList.h Cast.h)
target_link_libraries(Logger PRIVATE OpenXLSX-static mpr.dll)
target_compile_definitions(Logger PRIVATE OPENXLSX_STATIC_DEFINE)
set_target_properties(
        Logger
        PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
        LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)
