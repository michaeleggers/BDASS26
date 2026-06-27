#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <thread>
#include <vector>
#include <algorithm>

#define TEN (10)
#define HUNDRED (TEN*TEN)
#define THOUSAND (10*HUNDRED)
#define MILLION (THOUSAND*THOUSAND)
#define BILLION (THOUSAND*MILLION)

int64_t RandBetween(int64_t min, int64_t max)
{
    double r = (float)rand()/(float)RAND_MAX;
    int64_t range = max - min;
    return int64_t((double)range * r);
}

std::vector<int64_t> GenerateNumbers(uint64_t num)
{
    printf("Generating numbers...\n");
    clock_t startNumberGen = clock();
    std::vector<int64_t> randomNumbers{};
    for (int i = 0; i < num; i++) {
    //    int64_t randomNumber = rand();
    int64_t randomNumber = RandBetween(0, 10);
       randomNumbers.push_back(randomNumber);
    }
    clock_t endNumberGen = clock();
    double secsGenerated = (double)(endNumberGen - startNumberGen) / CLOCKS_PER_SEC;
    printf("Time needed for number generation (sec): %f\n", secsGenerated);
    
    return randomNumbers;
}

void BenchSingleThreaded(uint64_t numNumbers)
{
    std::vector<int64_t> randomNumbers = GenerateNumbers(numNumbers);
    clock_t startSort = clock();
    std::sort(randomNumbers.begin(), randomNumbers.end());
    clock_t endSort = clock();
    double secsSorted = (double)(endSort - startSort) / CLOCKS_PER_SEC;
    printf("Time needed for sorting (sec): %f\n", secsSorted);

    for (int i = 0; i < numNumbers; i++){
        printf("%d\n", randomNumbers[i]);
    }

}

void Sort(std::vector<int64_t>& randomNumbers, int start, int end)
{
    std::sort(randomNumbers.begin() + start, randomNumbers.begin() + end);
}

void BenchMultiThreaded(int numThreads, uint64_t numNumbers)
{
    std::vector<int64_t> randomNumbers = GenerateNumbers(numNumbers);
    // for (int i = 0; i< numNumbers; i++)
    // {
    //     printf("%d\n", randomNumbers[i]);
    // }

    int length = randomNumbers.size() / numThreads;
   
    std::vector<std::thread> threads{};
    for (int i = 0; i < numThreads; i++)
    {
        int start = i*length;
        int end = start + length;
        threads.push_back(std::thread(Sort, std::ref(randomNumbers), start, end));
    }
    for (int i = 0; i < numThreads; i++)
    {
        std::thread& t = threads[i];
        t.join();
    }

    /*
    for (int i = 0; i< numNumbers; i++)
    {
        printf("%d\n", randomNumbers[i]);
    }
    */
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    BenchMultiThreaded(16, BILLION);

    // BenchSingleThreaded(10);
    
    return 0;
}