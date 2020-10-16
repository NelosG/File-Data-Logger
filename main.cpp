#ifndef UNICODE
#define UNICODE
#endif

#include "getExcel/getExcelList.h"
#include <codecvt>
#include <fstream>
#include <sstream>

using namespace std;


wstring readFile(const char *filename) {
    wifstream wif(filename);
    wif.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
    wstringstream wss;
    wss << wif.rdbuf();
    wstring ws = wss.str();
    wif.close();
    return ws;
}


int main() {
    setlocale(LC_ALL, "");

    getExcel(readFile("../PathForLOGS.txt"), readFile("../Path.txt"));
    return 0;
}
