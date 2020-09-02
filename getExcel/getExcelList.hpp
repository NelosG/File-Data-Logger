#ifndef UNICODE
#define UNICODE
#endif
#include <Windows.h>
#include <vector>
#include <OpenXLSX.hpp>
#include "Cast.h"

std::string fileTimeToString(FILETIME &ft) ;

void getNameList(std::wstring path, OpenXLSX::XLDocument *doc, long long * ind, int *sheets) ;

void getExcel(const std::string& pathForLogs, const std::wstring& path) ;

[[maybe_unused]] void getExcel(const std::wstring& pathForLogs, const std::wstring& path) ;