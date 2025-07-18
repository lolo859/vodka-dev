#include <iostream>
#include <sstream>
#include "dependencies/termcolor.hpp"

int main() {
    std::stringstream ss;
    ss << termcolor::red << "This is red text" << termcolor::reset;
    
    std::cout << "Raw stringstream content:\n" << ss.str() << "\n";

    // When you output the content to the terminal, colors show up:
    std::cout << ss.str() << "\n";

    return 0;
}
