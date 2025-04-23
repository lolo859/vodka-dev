#include <boost/hash2/sha3.hpp>
#include <iostream>
#include <iomanip>
#include <string>

int main() {
    std::string input = "Hello, world!";
    
    // Create SHA3-512 hasher
    boost::hash2::sha3_512 hasher;
    hasher.update(input.data(),input.size());
    auto digest=hasher.result();
    for (auto byte : digest) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(byte);
    }
    std::cout << std::endl;
    return 0;
}