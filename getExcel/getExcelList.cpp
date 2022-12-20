#ifndef UNICODE
#define UNICODE
#endif

#include "getExcelList.h"
#include <vector>
#include <winnetwk.h>
#include <OpenXLSX.hpp>
#include <codecvt>

using namespace OpenXLSX;
using namespace std;

#define MAX_ELEMENTS_PER_PAGE 500000

struct tm;

string cast(const wstring& ws);

wstring ConvertToUNC(const wstring& sPath);

string fileTimeToString(FILETIME& ft) {
    SYSTEMTIME stUTC, stLocal;
    // преобразовать время создания в локальное время.
    FileTimeToSystemTime(&ft, &stUTC);
    SystemTimeToTzSpecificLocalTime(nullptr, &stUTC, &stLocal);

    // преобразовать полученное время в строку
    return to_string(stLocal.wHour) + ":" + (stLocal.wMinute < 10 ? "0" : "") + to_string(stLocal.wMinute) + " "
           + (stLocal.wDay < 10 ? "0" : "") + to_string(stLocal.wDay) + "."
           + (stLocal.wMonth < 10 ? "0" : "") + to_string(stLocal.wMonth)
           + "." + to_string(stLocal.wYear);
}

void getNameList(wstring path, XLDocument* doc, long long* ind, int* sheets) {
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
                wks.cell(XLCellReference("C" + ss)).value() = (file.nFileSizeHigh * (MAXDWORD + 1)) + file.nFileSizeLow;
                wks.cell(XLCellReference("D" + ss)).value() = fileTimeToString(file.ftCreationTime);
                wks.cell(XLCellReference("E" + ss)).value() = fileTimeToString(file.ftLastWriteTime);
                (*ind)++;
                if ((*ind) > MAX_ELEMENTS_PER_PAGE) {
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

struct tm* gettime() {
    struct tm* u;
    const time_t timer = time(nullptr);
    u = localtime(&timer);
    return u;
}

string timeToString(struct tm* u) {
    char s[40];
    for (char& i: s) i = 0;
    strftime(s, 40, "%H.%M_%d.%m.%Y", u);
    return s;
}

void getExcel(const string& pathForLogs, const wstring& path) {
    if (path.substr(0, 2) == L"\\\\")
        WNetAddConnection(ConvertToUNC(path).c_str(), (LPCWSTR)NULL, (LPCWSTR)NULL);
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

void getExcel(const wstring& pathForLogs, const wstring& path) {
    if (path.substr(0, 2) == L"\\\\")
        WNetAddConnection(ConvertToUNC(path).c_str(), (LPCWSTR)nullptr, (LPCWSTR)nullptr);
    getExcel(cast(pathForLogs) + "\\" + timeToString(gettime()) + ".xlsx", path);
}


int UnicodeToUTF8(char* res, const uint64_t unicode) {
    if (unicode <= 0x7F) {
        res[0] = unicode;
        return 1;
    } else if (unicode <= 0x7FF) {
        res[0] = 0xC0 | (unicode >> 6);
        res[1] = 0x80 | (unicode & 0x3F);
        return 2;
    } else if (unicode >= 0xDC80 && unicode <= 0xDCFF) {
        res[0] = unicode - 0xDC00;
        return 1;
    } else if (unicode <= 0xFFFF) {
        res[0] = 0xE0 | (unicode >> 12);
        res[1] = 0x80 | ((unicode >> 6) & 0x3F);
        res[2] = 0x80 | (unicode & 0x3F);
        return 3;
    } else if (unicode <= 0x10FFFF) {
        res[0] = 0xF0 | (unicode >> 18);
        res[1] = 0x80 | ((unicode >> 12) & 0x3F);
        res[2] = 0x80 | ((unicode >> 6) & 0x3F);
        res[3] = 0x80 | (unicode & 0x3F);
        return 4;
    } else {
        res[0] = 0xE0 | 0xF;
        res[1] = 0x80 | 0x3F;
        res[2] = 0x80 | 0x3D;
        return 3;
    }
}

string cast(const wstring& ws) {
    string s;
    char* res = (char*)malloc(sizeof(char) * 5);
    for (auto wc: ws) {
        int temp = UnicodeToUTF8(res, wc);
        res[temp] = '\0';
        s += res;
    }
    free(res);
    return s;
}

wstring ConvertToUNC(const wstring& sPath) {
    WCHAR temp;
    UNIVERSAL_NAME_INFO* puni = nullptr;
    DWORD bufsize = 0;
    wstring sRet = sPath;
    //Call WNetGetUniversalName using UNIVERSAL_NAME_INFO_LEVEL option
    if (WNetGetUniversalName(sPath.c_str(),
                             UNIVERSAL_NAME_INFO_LEVEL,
                             (LPVOID)&temp,
                             &bufsize) == ERROR_MORE_DATA) {
        // now we have the size required to hold the UNC path
        auto* buf = new WCHAR[bufsize + 1];
        puni = (UNIVERSAL_NAME_INFO*)buf;
        if (WNetGetUniversalName(sPath.c_str(),
                                 UNIVERSAL_NAME_INFO_LEVEL,
                                 (LPVOID)puni,
                                 &bufsize) == NO_ERROR) {
            sRet = wstring(puni->lpUniversalName);
        }
        delete[] buf;
    }

    return sRet;
}
