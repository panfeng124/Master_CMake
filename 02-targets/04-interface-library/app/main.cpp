#include <iostream>
#include <lab/diagnostics.h>
#include <lab/version.h>

int main() {
    std::cout << "LAB " << lab::version() << "!\n";
    print_diagnostics();
}
