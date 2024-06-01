#include <iostream>
#include <vector>
#include <numeric>   //accumulate
#include <cmath>     //pow

#include <chrono> 

bool verbose = false;

// Goal: find the lowest combination of supplementary ints that when added to the required ints,
// results in a sum that is a multiple of a given modulus
// "Find Lowest Possible Modulus Combination"

// Some inspiration taken from my CPSC2120 Lab Work for the wolf goat cabbage problem:
// Iteration reflects what each node represents
// int with each bit == 0 left of river, bit == 1 right of river
// each bit represents a different item (wolf, goat, cabbage, person)
// but here it's representing which supplementary int is being used
// and 'i' is the state of the supplementary ints that is finding the bits in,
// so iterating from i to 2^supplementaryInts.size() gives all possible combinations of supplementary ints

// Benefits: efficiently iterates through all possible sums of supplementary ints, skipping impossible sums
//

// Drawbacks: takes longer with more supplementary ints (2^n time for n supplementary ints)
// Doesn't efficiently locate the lowest sum, but rather finds all possible sums and then finds the lowest one

// TLDR: Iterates through all possible combinations of supplementary ints and finds the lowest sum that is a multiple of the modulus

// Bit position representation for the state of the supplementary ints
bool bit(int x, int i) { return (x >> i) & 1; }

// Found that sorting the supplementary ints in descending order before iterating through them made the program run slower.
// Could run faster if sorted in ascending order at vastly larger sizes?
// At what point does the sort time outweigh the time saved by iterating through the supplementary ints in a more efficient order?

// A sort of random ints effectively will not work as a sort
// But if we sort by their modulos and create a map of modulos to their ints, we can find modulos that we need to get closer to the desired modulo -> perhaps future iteration -> flpmc-redux?
// ^funny bc this is how i solved it first by head/on paper; this should significantly reduce the time complexity of the program (? at least if done well. I'm curious to see Caroline's implementation)
std::vector<int> findLowestCombination(const std::vector<int> &requiredInts, const std::vector<int> &supplementaryInts, int desiredModulo, int modulus)
{
    std::vector<int> lowestCombination;
    int lowestCombinationSum = INT_MAX;
    for (int i = 0; i < pow(2, supplementaryInts.size()); ++i)
    {
        std::vector<int> utilizedInts = {requiredInts};
        int sum = accumulate(requiredInts.begin(), requiredInts.end(), 0);
        for (int j = 0; j < supplementaryInts.size(); ++j)
        {
            if (bit(i, j) == 1)
            {
                utilizedInts.push_back(supplementaryInts[j]);
                sum += supplementaryInts[j];
            }
        }

        if (sum % modulus == desiredModulo)
        {
            if (sum < lowestCombinationSum)
            {
                lowestCombinationSum = std::accumulate(utilizedInts.begin(), utilizedInts.end(), 0);
                lowestCombination = utilizedInts;

                if (verbose)
                {
                    std::cout << "\tNew Lowest Combination: ";
                    for (int num : lowestCombination)
                    {
                        std::cout << num << " ";
                    }
                    std::cout << "sum: " << lowestCombinationSum << std::endl;
                }
            }
        }
    }
    return lowestCombination;
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    // Example usage
    std::vector<int> requiredInts = {25603};
    std::vector<int> supplementaryInts = {31892, 41486, 21731, 32129, 47905, 14806, 30720, 29367, 209, 31623, 26695, 4294, 45121, 35513, 17829, 15267, 412, 38022, 34499, 37021, 22538, 13989, 44480, 48542};
    int desiredModulo = 0;
    int modulus = 25;

    if (verbose)
    {
        std::cout << "Required Ints: ";
        for (int i : requiredInts)
        {
            std::cout << i << " ";
        }
        std::cout << "sum: " << accumulate(requiredInts.begin(), requiredInts.end(), 0) << std::endl;

        std::cout << "Supplementary Ints: ";
        for (int i : supplementaryInts)
        {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

    std::vector<int> utilizedInts = findLowestCombination(requiredInts, supplementaryInts, desiredModulo, modulus);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns" << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    // Print the utilized ints
    std::cout << "Lowest Sum: ";
    for (int i : utilizedInts)
    {
        std::cout << i << " ";
    }
    // std::cout << "sum: " << std::accumulate(utilizedInts.begin(), utilizedInts.end(), 0);
    std::cout << std::endl;

    return 0;
}