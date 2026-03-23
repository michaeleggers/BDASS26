#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <vector>
#include <algorithm>

#define TEN (10)
#define HUNDRED (TEN*TEN)
#define THOUSAND (10*HUNDRED)
#define MILLION (THOUSAND*THOUSAND)
#define BILLION (THOUSAND*MILLION)


void BenchSingleThreaded()
{
    printf("Generating numbers...\n");
    clock_t startNumberGen = clock();
    std::vector<int64_t> randomNumbers{};
    for (int i = 0; i < BILLION; i++) {
       int64_t randomNumber = rand();
       randomNumbers.push_back(randomNumber);
    }
    clock_t endNumberGen = clock();
    double secsGenerated = (double)(endNumberGen - startNumberGen) / CLOCKS_PER_SEC;
    printf("Time needed for number generation (sec): %f\n", secsGenerated);
    
    clock_t startSort = clock();
    std::sort(randomNumbers.begin(), randomNumbers.end());
    clock_t endSort = clock();
    double secsSorted = (double)(endSort - startSort) / CLOCKS_PER_SEC;
    printf("Time needed for sorting (sec): %f\n", secsSorted);

}

int main(int argc, char** argv)
{
    BenchSingleThreaded();
    
    return 0;
}