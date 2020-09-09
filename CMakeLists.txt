cmake_minimum_required(VERSION 3.16)
project(Logger)


#=======================================================================================================================
# Output Directories.
#=======================================================================================================================
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/source)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/source)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/source)




set(CMAKE_CXX_STANDARD 17)
add_subdirectory(getExcel)
add_executable(Logger main.cpp)
target_link_libraries(Logger PRIVATE getExcel)
set_target_properties(
        Logger
        PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
        LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)

file(WRITE ${CMAKE_BINARY_DIR}/output/Path.txt "C:\\")
file(WRITE ${CMAKE_BINARY_DIR}/output/PathForLOGS.txt "../LOGS")
