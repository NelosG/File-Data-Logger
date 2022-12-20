#ifndef UNICODE
#define UNICODE
#endif

#include "getExcel/getExcelList.h"
#include <codecvt>
#include <fstream>
#include <sstream>
#include <filesystem>
using namespace std;

string readFile(const char* filename) {
    ifstream wif(filename);
    string s;
    wif >> s;
    return s;
}


int main() {
    setlocale(LC_ALL, "");

    getExcel(filesystem::path(readFile("../PathForLOGS.txt")), filesystem::path(readFile("../Path.txt")));
    return 0;
}
