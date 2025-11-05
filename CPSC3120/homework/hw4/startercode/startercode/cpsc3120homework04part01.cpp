#include "cpsc3120homework04part01.h"
#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm> // for std::max

using namespace std;

int longestIncreasingSubsequence(vector<int> numbers) {
    int n = numbers.size();
    if (n == 0) return 0;

    // dp[i] will store the length of the longest increasing subsequence ending at index i
    vector<int> dp(n, 1); // Initialize all to 1 since the shortest LIS at each point is 1 (itself)

    // Iterate through each number in the list
    for (int i = 1; i < n; ++i) {
        // Compare with all previous elements
        for (int j = 0; j < i; ++j) {
            // If a valid increasing pair is found
            if (numbers[i] > numbers[j]) {
                // Update dp[i] to reflect the new longer subsequence
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
    }

    // The longest subsequence will be the maximum value in the dp array
    return *max_element(dp.begin(), dp.end());
}