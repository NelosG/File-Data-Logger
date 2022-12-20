#ifndef UNICODE
#define UNICODE
#endif

#include "getExcelList.h"
#include <vector>
#include <logger.h>

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

void getNameList(wstring path, logger& lg, long long& ind, int& sheets) {
    WIN32_FIND_DATA file;
    if (path.empty()) return;
    path += (path[path.length() - 1] == L'\\' ? L"" : L"\\");

    HANDLE hFind = FindFirstFile((path + L"*").c_str(), &file);
    if (hFind == INVALID_HANDLE_VALUE) return;
    do {
        if (wstring(file.cFileName) != L"." && wstring(file.cFileName) != L"..") {

            if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                getNameList(path + file.cFileName, lg, ind, sheets);

            } else {
                lg << cast(file.cFileName);
                lg << cast(path + file.cFileName);
                lg << (file.nFileSizeHigh * (MAXDWORD + 1)) + file.nFileSizeLow;
                lg << fileTimeToString(file.ftCreationTime);
                lg << fileTimeToString(file.ftLastWriteTime);
                lg.next_line();
                ++ind;
                if (ind > MAX_ELEMENTS_PER_PAGE) {
                    ++sheets;
                    lg.set_page("Sheet" + to_string(sheets),
                                {"Имя Файла", "Путь", "Размер", "Время Создания", "Время Изменения"});
                    ind = 2;
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

void getExcel(const std::string& pathForLogs, const wstring& path) {
    if (path.substr(0, 2) == L"\\\\")
        WNetAddConnection(ConvertToUNC(path).c_str(), (LPCWSTR)nullptr, (LPCWSTR)nullptr);
    logger lg(pathForLogs);
    lg.set_page("Sheet1", {"Имя Файла", "Путь", "Размер", "Время Создания", "Время Изменения"});
    long long ind = 2;
    int sh = 1;
    getNameList(path, lg, ind, sh);
    lg.close();
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
    UNIVERSAL_NAME_INFO* puni;
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
