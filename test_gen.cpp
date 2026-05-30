#include <cstdio>
#include <iostream>
#include <string>

#include "include/intsort.h"
#include "include/strlenlexi.h"
#include "include/strlexi.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Error: invalid command\n";
        cout << "Command: " << argv[0] << " <int/strlexi/strlenlexi> <1/2/3/4/5>\n";
        return 1;
    }

    string algorithm = argv[1];
    string test_id = argv[2];

    string filename = "test/" + algorithm + "/test00" + test_id + ".in";

    if (freopen(filename.c_str(), "r", stdin) == nullptr) {
        cout << "Error: cannot read test file";
        return 1;
    }

    if (algorithm == "int") {
        runIntSort();
    } else if (algorithm == "strlexi") {
        runStrLexiSort();
    } else if (algorithm == "strlenlexi") {
        runStrLenLexiSort();
    } else {
        std::cerr << "Error: invalid algorithm option.\n";
        return 1;
    }
    return 0;
}
