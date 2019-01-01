#include "common/test/kevin-test.h"

#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage:" << std::endl;
        std::cerr << "<binary> <TestFixtureName> [<TestCaseName>]" << std::endl;
        return -1;
    }
    kevin::test::runTestByKevin(argv[1], argc == 3 ? argv[2] : "");
    return 0;
}
