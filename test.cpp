#include <iostream>
#include <chrono>
#include <random>
#include <stdlib.h>
#include "vodka-lib/vodka-lib.h" // Assure-toi que ta fonction est bien déclarée ici
random_device rd;
mt19937_64 gen(rd());
int main() {
    const size_t iterations = 10000000;
    using namespace std::chrono;

    auto start = high_resolution_clock::now();

    for (size_t i = 0; i < iterations; ++i) {
        vodka::utilities::genvyid();
        // volatile pour éviter une éventuelle optimisation par le compilateur
    }

    auto end = high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    std::cout << "Total time: " << duration.count() << " seconds\n";
    std::cout << "Average time per call: " << (duration.count() / iterations) * 1000000 << " µs\n";

    return 0;
}
