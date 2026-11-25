#include <iostream>
#include <lab/diagnostics.h>

void print_diagnostics() {
#ifdef LAB_VERBOSE
    std::cout << "Diagnostics enabled\n";
#endif
};