#ifndef UNICODE
#define UNICODE
#endif

#include "getExcel/getExcelList.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <random>

using namespace std;

string readFile(const char* filename) {
    ifstream wif(filename);
    string s;
    wif >> s;
    return s;
}

filesystem::path createTempDir(
      unsigned long long max_tries = 100) {
    auto tmp_dir = filesystem::temp_directory_path();
    unsigned long long i = 0;
    random_device dev;
    mt19937 prng(dev());
    uniform_int_distribution<uint64_t> rand(0);
    filesystem::path path;
    while (true) {
        stringstream ss;
        ss << hex << rand(prng);
        path = tmp_dir / ss.str();
        // true if the directory was created.
        if (filesystem::create_directory(path)) {
            break;
        }
        if (i == max_tries) {
            throw runtime_error("could not find non-existing directory");
        }
        i++;
    }
    return path;
}


int main(int argc, char *argv[]) {
    auto path = argc > 1 ? argv[1] : readFile("../Path.txt");
    auto pathForLogs = argc > 2 ? argv[2] : readFile("../PathForLogs.txt");

    auto tempDir = createTempDir();

    getExcel(filesystem::path(path), tempDir);

    filesystem::copy(tempDir, filesystem::path(pathForLogs));
    filesystem::remove_all(tempDir);
    return 0;
}
