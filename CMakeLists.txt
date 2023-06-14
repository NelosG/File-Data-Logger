cmake_minimum_required(VERSION 3.16)
project(Logger)


#=======================================================================================================================
# Output Directories.
#=======================================================================================================================
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/output-${CMAKE_BUILD_TYPE})
set(SOURCE_DIR ${OUTPUT_DIR}/source)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOURCE_DIR})




set(CMAKE_CXX_STANDARD 20)
add_subdirectory(getExcel)
add_executable(file-logger main.cpp)
target_link_libraries(file-logger PRIVATE getExcel)

if (CMAKE_BUILD_TYPE MATCHES Release)
    set_target_properties(
            file-logger
            PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
            LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )
endif ()

set(PATH_CONFIG_FILE ${OUTPUT_DIR}/Path.txt)
set(OUTPUT_CONFIG_FILE ${OUTPUT_DIR}/PathForLogs.txt)

if(NOT EXISTS ${PATH_CONFIG_FILE})
file(WRITE ${PATH_CONFIG_FILE} "..\\DB")
endif ()

if(NOT EXISTS ${OUTPUT_CONFIG_FILE})
file(WRITE ${OUTPUT_CONFIG_FILE} "..\\logs")
endif ()
