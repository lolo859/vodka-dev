#include <boost/crypto/hash.hpp>
#include <iostream>
#include <string>
#include <iomanip>

int main() {
    std::string input = "Hello, SHA3-512!";
    
    // Effectuer le hash SHA3-512
    auto digest = boost::crypto::sha3_512(input);

    // Afficher le résultat en hexadécimal
    std::cout << "SHA3-512: ";
    for (unsigned char byte : digest) {
        std::cout << std::hex << std::setfill('0') << std::setw(2)
                  << static_cast<int>(byte);
    }
    std::cout << std::endl;

    return 0;
}