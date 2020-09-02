set(CMAKE_CXX_STANDARD 20)
add_subdirectory(OpenXLSX-master)

add_library(getExcel STATIC Cast.h Cast.cpp getExcelList.hpp getExcelList.cpp)

target_link_libraries(getExcel OpenXLSX-static mpr.dll)

target_compile_definitions(getExcel PRIVATE OPENXLSX_STATIC_DEFINE)