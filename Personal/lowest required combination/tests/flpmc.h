#include <iostream>
#include <vector>
#include <numeric>   //accumulate
#include <cmath>     //pow

#include <algorithm>

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
std::vector<int> findLowestCombination_FLPMC(const std::vector<int> &requiredInts, const std::vector<int> &supplementaryInts, int desiredModulo, int modulus)
{
    std::vector<int> sortedSupplementaryInts = supplementaryInts;
    std::sort(sortedSupplementaryInts.begin(), sortedSupplementaryInts.end());
    std::vector<int> lowestCombination;
    int lowestCombinationSum = INT_MAX;
    for (int i = 0; i < pow(2, sortedSupplementaryInts.size()); ++i)
    {
        std::vector<int> utilizedInts = {requiredInts};
        int sum = accumulate(requiredInts.begin(), requiredInts.end(), 0);
        for (int j = 0; j < sortedSupplementaryInts.size(); ++j)
        {
            if (bit(i, j) == 1)
            {
                utilizedInts.push_back(sortedSupplementaryInts[j]);
                sum += sortedSupplementaryInts[j];
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