#include <iostream>

int main() {
    std::cout << "LAB v3!\n";
#ifdef LAB_VERBOSE
    std::cout << "Diagnostics enabled\n";
#endif
}
