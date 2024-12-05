#include <iostream>
#include <vector>
#include <numeric>   //accumulate
#include <cmath>     //pow

#include <chrono> 

bool verbose = true;

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
// Further thought: sorting it doesn't really give an advantage when I don't know the real minimum sum, so that sorting doesn't make it faster when it is going to iterate everything anyways

// A sort of random ints effectively will not work as a sort
// But if we sort by their modulos and create a map of modulos to their ints, we can find modulos that we need to get closer to the desired modulo -> perhaps future iteration -> flpmc-redux?
// ^funny bc this is how i solved it first by head/on paper; this should significantly reduce the time complexity of the program (? at least if done well. I'm curious to see Caroline's implementation)

// final notes:
// how to guarantee most efficient modulo combo to value combo?
// my current fastest runtime is with the best answers @ the start of the list & worst runtime is with best answers @ of the list
// what are the best answers and how can i get them to the start/get the worst to the end?
// any modulo added to the reqSum that takes it over the modulo gets moved to end?

// sort by modulo, find difference to desired modulo,
//re-call findLowestPossibleSum on the modulo sorted ints and the difference to the desired modulo
// still wouldn't guarantee lowest possible, just the simplest solution (least nums required to get to the desired modulo) 

// breaking down the problem:
// %2 only has to find the lowest value that changes the mod to the desired mod
// %3 only has to find the lowest value that changes the mod to the desired mod. if no single works out that way, then the lowest combination of 2 that works
// %4 only has to find the lowest value that changes the mod to the desired mod. if no single works out that way, then the lowest combination of 2 that works, then 3
// etc...
// rerun fLC with the ints with the modulos that are closest to the desired modulo 
// if unsatisfied, rerun with the next closest modulos
// alternatively, look for a perfect mod to sum to. if no perfect mod, then sort by nums ascnd; find the first two that sum to the desired mod, then the first three, etc.

void printIntToBinary(int x, int maxX)
{
    // only print required amount of bits to store x
    int numBits = 0;
    for (int i = 31; i >= 0; --i)
    {
        if ((maxX >> i) % 2 == 1)
        {
            numBits = i + 1;
            break;
        }
    }
    numBits--;
    
    for (int i = numBits - 1; i >= 0; --i)
    {
        std::cout << (x >> i) % 2;
    }
    std::cout << std::endl;
}

//can only take max 30 supplementary ints because of the 2^31-1 int limit
std::vector<int> findLowestCombination(const std::vector<int> &requiredInts, const std::vector<int> &supplementaryInts, int desiredModulo, int modulus)
{
    std::vector<int> lowestCombination;
    int lowestCombinationSum = INT_MAX;
    for (int i = 0; i < pow(2, supplementaryInts.size()); ++i)
    {
        printIntToBinary(i, pow(2, supplementaryInts.size()));
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

        if (sum % modulus == desiredModulo && sum != 0) //sum != 0 to avoid the case where required ints is empty 
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
    std::vector<int> requiredInts = {408};
    std::vector<int> supplementaryInts = {1527, 1938, 587, 595, 2379, 639, 5895};
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