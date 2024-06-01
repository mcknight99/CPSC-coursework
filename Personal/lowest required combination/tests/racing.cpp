#include <iostream>
#include <chrono>
#include <random>
#include "flpmc.h"
#include "joey-redux.h"

using namespace std;

int overallMax = 500000;
int requiredIntsMaxSize = 3;
int requiredIntsMinSize = 1;
int supplementaryIntsMaxSize = 25;
int supplementaryIntsMinSize = 1;
int maxInt = 50000;
int minInt = 1;
int maxModulus = 1000;
int minModulus = 2;
// Max modulus must be greater than max desired modulo

// Notes: when it has an actual answer, flpmc is faster than joey-redux. With no answer, joey-redux is faster.

// Pros and cons of each:
// flpmc: quick solve time with any sum difference, but slow with large supplementary ints and slow to figure out if there is no answer
// joey-redux: constant* with large supplementary ints and quick to figure out if there is no answer, but slow solve time and even slower with large differences between the required and total sum
// *by constant, if these supplementary ints are significant, they can also add solve time to joey-redux because they increase the total sum

int gen_rand_int(int min, int max)
{
    int seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

int main()
{
    vector<int> answer;
    vector<int> requiredInts;
    vector<int> supplementaryInts;
    int desiredModulo;
    int modulus;

    do
    {
        // Generate the inputs
        int requiredIntsSize = gen_rand_int(requiredIntsMinSize, requiredIntsMaxSize);
        int supplementaryIntsSize = gen_rand_int(supplementaryIntsMinSize, supplementaryIntsMaxSize);
        requiredInts.resize(requiredIntsSize);
        supplementaryInts.resize(supplementaryIntsSize);
        for (int i = 0; i < requiredIntsSize; i++)
        {
            requiredInts[i] = gen_rand_int(minInt, maxInt);
        }
        for (int i = 0; i < supplementaryIntsSize; i++)
        {
            supplementaryInts[i] = gen_rand_int(minInt, maxInt);
        }
        modulus = gen_rand_int(minModulus, maxModulus);
        desiredModulo = gen_rand_int(0, modulus - 1);

        // Start the races

        auto start = chrono::high_resolution_clock::now();
        auto end = chrono::high_resolution_clock::now();

        // Measure the runtime of joey_redux
        start = chrono::high_resolution_clock::now();
        vector<int> test = findLowestCombination_JR(requiredInts, supplementaryInts, desiredModulo, modulus);
        end = chrono::high_resolution_clock::now();
        auto joey_redux_runtime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        auto joey_rr_seconds = joey_redux_runtime / 1e9;
        if(test.size() == 0)
        {
            // Joey-redux is faster when there is no answer, so skip the flpmc test to save time
            //std::cout << "joey-redux did not find an answer, skipping this random gen" << std::endl;
            continue;
        }

        std::cout<<"confirmed answer, racing now..."<<std::endl;

        // Measure the runtime of flpmc
        start = chrono::high_resolution_clock::now();
        findLowestCombination_FLPMC(requiredInts, supplementaryInts, desiredModulo, modulus);
        end = chrono::high_resolution_clock::now();
        auto flpmc_runtime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        auto flpmc_seconds = flpmc_runtime / 1e9;

        answer = (flpmc_runtime > joey_redux_runtime) ? findLowestCombination_JR(requiredInts, supplementaryInts, desiredModulo, modulus) : findLowestCombination_FLPMC(requiredInts, supplementaryInts, desiredModulo, modulus);

        // Print the runtimes
        std::cout << "flpmc runtime: \t\t" << flpmc_runtime << " nanoseconds \t(" << flpmc_seconds << "s)" << endl;
        std::cout << "joey-redux runtime: \t" << joey_redux_runtime << " nanoseconds \t(" << joey_rr_seconds << "s)" << endl;

    } while (answer.size() == 0);

    std::cout << "Solved problem details" << std::endl;
    std::cout << "Required Ints: ";
    for (int i : requiredInts)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "Supplementary Ints: ";
    for (int i : supplementaryInts)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "Desired Modulo: " << desiredModulo << std::endl;
    std::cout << "Modulus: " << modulus << std::endl;
    std::cout << "Lowest Sum: ";
    for (int i : answer)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "Sum: " << accumulate(answer.begin(), answer.end(), 0) << std::endl;
    std::cout << "Modulus check: " << accumulate(answer.begin(), answer.end(), 0) << "%" << modulus << " = " << accumulate(answer.begin(), answer.end(), 0) % modulus << " (desired modulo: " << desiredModulo << ")" << std::endl;

    return 0;
}