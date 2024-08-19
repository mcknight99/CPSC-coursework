#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

#include "../helpers/subset.h"

#include <chrono>

// Code
// explanation from joey:
/*
So this is what it does. First it sums the required ints (rSum) and all the ints (totalSum).
Then it starts counting up from rSum towards totalSum. At each number it first checks with if divided by modulus the answer is modulo (meaning it's a valid answer),
and then checks whether, when you subtract rSum from it, the remaining number is divisible by some combination of the supplementary numbers
(meaning rSum + some combination of supplementary numbers = this valid solution). The first one that meets those requirements will obviously be the lowest because it just counts up afterwards.
*/

// Benefits: doesn't take longer with more supplementaries
// Drawbacks: takes longer with a larger difference from the rSum and totalSum. Would take longer with the rSum step implementation with smaller modulus values.
// Current runtime doesn't provide a breakdown of the used supplementary ints, but that could be added.
// TLDR: It starts at the lowest possible sum (required) and then keeps adding 1 until the maximum sum (required + supplementaries), and returns the first sum that meets the requirements.
// time complexity: O((totalSum - rSum) * n), or O(mn) where m is the difference between totalSum and rSum, n is the number of supplementary ints

bool canSumToTarget(int target, const vector<int> &numbers)
{
    vector<bool> dp(target + 1, false);
    dp[0] = true;
    for (int num : numbers)
    {
        for (int i = target; i >= num; --i)
        {
            if (dp[i - num])
            {
                dp[i] = true;
            }
        }
    }
    return dp[target];
}

std::vector<int> findLowestCombination(const std::vector<int> &requiredInts, const std::vector<int> &supplementaryInts, int desiredModulo, int modulus)
{
    std::vector<int> lowestCombination;
    // Sums
    int minSum = accumulate(requiredInts.begin(), requiredInts.end(), 0);
    int maxSum = minSum;
    for (int i = 0; i < supplementaryInts.size(); i++)
    {
        maxSum += supplementaryInts[i];
    }

    int nearestRSumMultiple = minSum + ((modulus - minSum) % modulus);
    for (int i = nearestRSumMultiple; i < maxSum; i += modulus) //did not make as much as a difference as i was expecting really. probably would at large sizes. but at small sizes, this has no change on runtime
    { // sam speed notes: perhaps start at the nearest multiple of modulus to minSum, and iterate by modulus? // he says he was going to do this but forgot to implement it lol
        if (i % modulus == desiredModulo)
        {
            for (int j = 0; j < supplementaryInts.size(); j++)
            {
                if (canSumToTarget(i - minSum, supplementaryInts))
                {
                    lowestCombination = {requiredInts};
                    std::vector<int> lowestParts = subsetSum(supplementaryInts, i - minSum);
                    // I tried removing the subsetSum to see if it would take off time, but it's negligible (no noticeable difference in time)
                    for (int k = 0; k < lowestParts.size(); k++)
                    {
                        lowestCombination.push_back(lowestParts[k]);
                    }                    

                    return lowestCombination;
                }
            }
        }
    }
    return lowestCombination;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // Given
    vector<int> required = {1880};
    vector<int> supplementary = {2414, 1474, 893, 740, 1261, 1403, 3005};
    int modulus = 25;
    int modulo = 0;

    std::vector<int> lowestCombination = findLowestCombination(required, supplementary, modulo, modulus);
    auto end = chrono::high_resolution_clock::now();
    std::cout << "Time: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << std::endl;
    std::cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    std::cout << "Lowest Combination: ";
    for (int i : lowestCombination)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}