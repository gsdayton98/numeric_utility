#include <iostream>
#include "miller_rabin.hpp"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
auto main(int, char*[]) -> int {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    auto lang = "C++";
    std::cout << "Hello and welcome to " << lang << "!\n";

    std::cout << 101 << " " << (millerRabin(101) ? "prime" : "not prime") << "\n";

    return 0;
}