# 💻 CHEM 284 - Assembly and SIMD

## 🧪 Goal

The goal of this lab is to:

1. Learn about **C++ compilation, Assembly and leveraging the CPU directly**.
2. Learn how to use **SIMD intrinsics to speed up code that the compiler can't automatically**. 
3. Practice using **godbolt.com to visualize assembly code and its instructions**.
4. Profile 2 versions of cumulative summation code with and without SIMD intrinsics.

---
## 🗂️ Provided

- A `docker` file to set up the dev environment.
- A `CMakeLists.txt` file to help build the executables.
- A `src` directory with the `sum.cpp` and `simd_sum.cpp` files.

---
## 💻 Setup
```bash
./docker_build.sh # You may need to chmod +x
./docker_run.sh # You may need to chmod +x
```
To build:
```
# From the main directory
mkdir build
cd build
cmake ..
make
```

To run the executables
```
# Make sure you are in the build directory
./sum
./simd_sum
```

## ✅ Tasks
### Investigation 1:
```cpp
int add(int a, int b) {
    return a + b;
}

int main() {
    add(4, 5);
    return 0;
}
```
Paste the code into the [godbolt.com](https://godbolt.org/) C++ source text editor and see what the assembly looks like. Do the instructions and data types make sense? Hover over each instruction to get an idea of what is going on and hovering over the C++ source code will highlight the corresponding regions in the assembly compilers.

1) Now use the optimization flag for C++ `-O1` in the compiler options. What does the assembly look like now? Whats going on here?
2) Now try `-O2` as the compiler optimization option. What changes?

### Investigation 2:
```cpp
void scale(float* a, float s, int n) {
    for (int i = 0; i < n; i++) {
        a[i] *= s;
    }
}

int main() {
    int nums = 50;
    float a[nums];
    scale(a, 2.0, nums);
    return 0;
}
```
Paste the code into the godbolt C++ source text editor and see what the assembly looks like. This should be much larger in assembly. What registers are being used for the multiplication? Is this vectorized? If so what instructions show that it is?

1) Now use the different optimization flags -O1 to -O3 to see when vectorization occurs and which registers are being used along with which SIMD instruction. Also try `-O3 -march=native`, does anything change?

| Optimization Flag          | Vectorized? (Yes/No) | SIMD Registers Used (XMM/YMM/ZMM) | Key SIMD Instructions Observed | Notes / Observations |
|----------------------------|----------------------|----------------------------------|--------------------------------|----------------------|
| -O0 or None                |                      |                                  |                                |                      |
| -O1                        |                      |                                  |                                |                      |
| -O2                        |                      |                                  |                                |                      |
| -O3                        |                      |                                  |                                |                      |
| -O3 -march=native          |                      |                                  |                                |                      |

### Investigation 3:
```cpp
#include <iostream>

long long sum(int num) {
    long long sum = 0;
    for (int i = 1; i <= num; i++) {
        sum += i;
    }
    return sum;
}

int main() {
    int nums = 1e3;
    auto total_sum = sum(nums);
    std::cout << total_sum << std::endl;
    return 0;
}
```

The code above is not vectorizable, you can try to put it into godbolt and try any compilation flags you like but you won't be able to get it to vectorize. It is not vectorizable because there is data dependency on `sum`, each iteration depends on what the previous value of `sum`.

But all hope is not lost! We can still vectorize this but it will require more work from us using SIMD intrinsics: [intel intrinsics for AVX2](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#avxnewtechs=AVX2) (on the right hand side you see what Assembly instruction the instrinic maps to).

To vectorize we will be using 256-bit registers on 32-bit integers. This means each register will have 8 integers! To look for which intrinsics you need you can use the search bar on the website above, and make sure you are using the correct data type!
1) Set up a SIMD accumulator which is just an empty 256-bit vector with zeros. This is where we will add the running sum for each set of 8 integers.
```
// This is pseudocode, there is an SIMD intrinsic for this!
__m256i vsum = [0, 0, 0, 0, 0, 0, 0, 0]
```
2) Then set up a loop, and process the elements in chunks of 8 integers. You will need to create a new `__m256i` vector within the loop and use a SIMD intrinsic to `set` the values.
3) Add the new vector to the accumulator vector you created outside the loop. Use a SIMD addition intrinsic for this. Each lane will act as a partial sum.
```
// Iteration 1
vsum = [0,0,0,0,0,0,0,0]
v    = [8,7,6,5,4,3,2,1]
--------------------------------
vsum = [8,7,6,5,4,3,2,1]
```
4) Once the loop ends, you need to move the data back to memory in an aligned array. This is required for the `__mm256_store_si256` intrinsic, there is another store intrinsic we could use that doesn't require alignment but its usually slower.
```
alignas(32) int tmp[8];
_mm256_store_si256((__m256i*)tmp, vsum);
```
5) After you have all the data in memory you should have an array of 8 integers which are all partial sums. Now you can simply loop through the `tmp` array and add all the values sequentially into 1 scalar sum!

Work on this on godbolt until you see it compiling with the flag `-mavx2`. To confirm that it is vectorized, you should look for `vpaddd` which is the vectorized addition of DOUBLE WORD which is what our 32-bit integers are.

Once you have a vectorized solution, copy and paste it into `simd_sum.cpp` in src and build the program(s) using the instructions above. Then you can run the executables `sum` and `simd_sum` and see how long they take!

```
// ./sum
Total sum took 2376 ns

// ./simd_sum
Total sum took 350 ns
```

### Extra time
Can you try to modify the `simd_sum.cpp` code to use `AVX-512`? How much faster is it?
