#ifndef UNICODE
#define UNICODE
#endif
#include "getExcel/getExcelList.hpp"
#include <codecvt>
#include <winnetwk.h>

using namespace std;
using namespace OpenXLSX;

struct tm;

wstring ConvertToUNC(wstring sPath)
{
    WCHAR temp;
    UNIVERSAL_NAME_INFO * puni = NULL;
    DWORD bufsize = 0;
    wstring sRet = sPath;
    //Call WNetGetUniversalName using UNIVERSAL_NAME_INFO_LEVEL option
    if (WNetGetUniversalName(sPath.c_str(),
                             UNIVERSAL_NAME_INFO_LEVEL,
                             (LPVOID) &temp,
                             &bufsize) == ERROR_MORE_DATA)
    {
        // now we have the size required to hold the UNC path
        WCHAR * buf = new WCHAR[bufsize+1];
        puni = (UNIVERSAL_NAME_INFO *)buf;
        if (WNetGetUniversalName(sPath.c_str(),
                                 UNIVERSAL_NAME_INFO_LEVEL,
                                 (LPVOID) puni,
                                 &bufsize) == NO_ERROR)
        {
            sRet = wstring(puni->lpUniversalName);
        }
        delete [] buf;
    }

    return sRet;;
}

wstring readFile(const char *filename) {
    wifstream wif(filename);
    wif.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
    wstringstream wss;
    wss << wif.rdbuf();
    wstring ws = wss.str();
    wif.close();
    return ws;
}

struct tm* gettime() {
    struct tm *u;
    const time_t timer = time(nullptr);
    u = localtime(&timer);
    return u;
}

string timeToString(struct tm *u) {
    char s[40];
    for (char &i : s) i = 0;
    strftime(s, 40, "%H.%M_%d.%m.%Y", u);
    return string(s);
}
int main() {
    setlocale(LC_ALL, "");
    wstring ws = readFile("../Path.txt");
        wstring wss = ConvertToUNC(ws);
    if(ws.substr(0, 2) == L"\\\\") {
        WNetAddConnection(wss.c_str(), (LPCWSTR) NULL, (LPCWSTR) NULL);
    }
    getExcel(cast(readFile("../PathForLOGS.txt")) + "\\" + timeToString(gettime()) + ".xlsx", wss);
    return 0;
}
