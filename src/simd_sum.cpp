#include <chrono>
#include <iostream>
#include <immintrin.h>

long long simd_sum(int N) {
    long long result = 0;

    // YOUR CODE HERE!

    return result;
}

int main() {
    int nums = 1e3;
    // --- Timing Start ---
    auto start = std::chrono::high_resolution_clock::now();
    
    auto total_sum = simd_sum(nums);
    
    // --- Timing End ---
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << total_sum << std::endl;

    // Compute elapsed time in nanoseconds
    auto s = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << "Total sum took " << s.count() << " ns\n";
    return 0;
}
