#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include <chrono>

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

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // Given
    vector<int> required = {1880};
    vector<int> supplementary = {2414, 1474, 893, 740, 1261, 1403, 3005};
    int modulus = 25;
    int modulo = 0;

    // Sums
    int rSum = 0;
    for (int i = 0; i < required.size(); i++)
    {
        rSum += required[i];
    }
    int totalSum = rSum;
    for (int i = 0; i < supplementary.size(); i++)
    {
        totalSum += supplementary[i];
    }

    // Code
    // explanation from joey:
    /*
    So this is what it does. First it sums the required ints (rSum) and all the ints (totalSum).
    Then it starts counting up from rSum towards totalSum. At each number it first checks with if divided by modulus the answer is modulo (meaning it's a valid answer),
    and then checks whether, when you subtract rSum from it, the remaining number is divisible by some combination of the supplementary numbers
    (meaning rSum + some combination of supplementary numbers = this valid solution). The first one that meets those requirements will obviously be the lowest because it just counts up afterwards.
    */

    // Benefits: doesn't take longer with more supplementaries
    // Drawbacks: takes longer with a larger difference from the rSum and totalSum. Would take longer with the rSum step implementation with smaller modulus values. n runtime?.
    // TLDR: It starts at the lowest possible sum (required) and then keeps adding 1 until the maximum sum (required + supplementaries), and returns the first sum that meets the requirements.
    // time complexity: O((totalSum - rSum) * n), or O(mn) where m is the difference between totalSum and rSum, n is the number of supplementary ints
    int sol = -1;
    for (int i = rSum; i < totalSum; i += 1)
    { // sam speed notes: perhaps start at the nearest multiple of modulus to rSum, and iterate by modulus? // he says he was going to do this but forgot to implement it lol
        if (i % modulus == modulo)
        {
            for (int j = 0; j < supplementary.size(); j++)
            {
                if (canSumToTarget(i - rSum, supplementary))
                {
                    sol = i;
                    j = supplementary.size();
                    i = totalSum;
                }
            }
        }
    }
    if (sol != -1)
        cout << "Lowest sum: " << sol << endl;
    else
        cout << "No bueno" << endl;

    auto end = chrono::high_resolution_clock::now();
    cout << "Time: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << "ns" << endl;
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
}