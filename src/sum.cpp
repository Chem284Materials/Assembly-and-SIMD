#include <iostream>
#include <chrono>

long long sum(int num) {
    long long sum = 0;
    for (int i = 1; i <= num; i++) {
        sum += i;
    }
    return sum;
}

int main() {
    int nums = 1e3;
    // --- Timing Start ---
    auto start = std::chrono::high_resolution_clock::now();

    auto total_sum = sum(nums);

    // --- Timing End ---
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << total_sum << std::endl;

    // Compute elapsed time in nanoseconds
    auto s = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Total sum took " << s.count() << " ns\n";

    return 0;
}
