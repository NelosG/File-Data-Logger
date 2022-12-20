cmake_minimum_required(VERSION 3.16)
project(Logger)


#=======================================================================================================================
# Output Directories.
#=======================================================================================================================
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/source)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/source)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/source)




set(CMAKE_CXX_STANDARD 20)
add_subdirectory(getExcel)
add_executable(Logger main.cpp)
target_link_libraries(Logger PRIVATE getExcel)

#if (CMAKE_BUILD_TYPE MATCHES Release)
#    set_target_properties(
#            Logger
#            PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
#            LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
#    )
#endif ()
file(WRITE ${CMAKE_SOURCE_DIR}/output/Path.txt "C:\\Users\\glebp\\Desktop\\DB")
file(WRITE ${CMAKE_SOURCE_DIR}/output/PathForLOGS.txt "..")
