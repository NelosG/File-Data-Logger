#include <Windows.h>
#include <vector>
#include "Cast.h"
#include <OpenXLSX.hpp>
using namespace OpenXLSX;

std::string fileTimeToString(FILETIME &ft) {
    SYSTEMTIME stUTC, stLocal;
    using namespace std;
    // преобразовать время создания в локальное время.
    FileTimeToSystemTime(&ft, &stUTC);
    SystemTimeToTzSpecificLocalTime(nullptr, &stUTC, &stLocal);

    // преобразовать полученное время в строку
    return to_string(stLocal.wHour) + ":" + (stLocal.wMinute < 10 ? "0" : "") + to_string(stLocal.wMinute) + " "
           + (stLocal.wDay < 10 ? "0" : "") + to_string(stLocal.wDay) + "."
           + (stLocal.wMonth < 10 ? "0" : "") + to_string(stLocal.wMonth)
           + "." + to_string(stLocal.wYear);
}

void getNameList(std::wstring path, XLDocument *doc, long long * ind, int *sheets) {
    using namespace std;
    WIN32_FIND_DATA file;
    path += (path[path.length() - 1] == L'\\' ? L"" : L"\\");
    auto wks = (*doc).workbook().worksheet("Sheet" + to_string(*sheets));

    HANDLE hFind = FindFirstFile((path + L"*").c_str(), &file);
    if (hFind == INVALID_HANDLE_VALUE) return;
    do {
        if (wstring(file.cFileName) != L"." && wstring(file.cFileName) != L"..") {
            if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                getNameList(path + file.cFileName, doc, ind, sheets);
            } else {
                std::string ss(std::to_string(*ind));
                wks.cell(XLCellReference("A" + ss)).value() = cast(file.cFileName);
                wks.cell(XLCellReference("B" + ss)).value() = cast(path + file.cFileName);
                wks.cell(XLCellReference("C" + ss)).value() = (file.nFileSizeHigh * (MAXDWORD+1)) + file.nFileSizeLow;
                wks.cell(XLCellReference("D" + ss)).value() = fileTimeToString(file.ftCreationTime);
                wks.cell(XLCellReference("E" + ss)).value() = fileTimeToString(file.ftLastWriteTime);
                (*ind)++;
                if((*ind) > 500000) {
                    (*sheets)++;
                    (*doc).workbook().addWorksheet("Sheet" + to_string(*sheets));
                    wks = (*doc).workbook().worksheet("Sheet" + to_string(*sheets));
                    wks.cell(XLCellReference("A1")).value() = "Имя Файла";
                    wks.cell(XLCellReference("B1")).value() = "Путь";
                    wks.cell(XLCellReference("C1")).value() = "Размер";
                    wks.cell(XLCellReference("D1")).value() = "Время Создания";
                    wks.cell(XLCellReference("E1")).value() = "Время Изменения";
                    (*ind) = 2;
                }
            }
        }
    } while (FindNextFile(hFind, &file) != 0);
    FindClose(hFind);
}

void getExcel(const std::string& pathForLogs, const std::wstring& path) {
    XLDocument doc;
    doc.create(pathForLogs);
    auto wks = doc.workbook().worksheet("Sheet1");
    wks.cell(XLCellReference("A1")).value() = "Имя Файла";
    wks.cell(XLCellReference("B1")).value() = "Путь";
    wks.cell(XLCellReference("C1")).value() = "Размер";
    wks.cell(XLCellReference("D1")).value() = "Время Создания";
    wks.cell(XLCellReference("E1")).value() = "Время Изменения";
    long long ind = 2;
    int sh = 1;
    getNameList(path, &doc, &ind, &sh);
    doc.save();
}

[[maybe_unused]] void getExcel(const std::wstring& pathForLogs, const std::wstring& path) {
    getExcel(cast(pathForLogs), path);
}