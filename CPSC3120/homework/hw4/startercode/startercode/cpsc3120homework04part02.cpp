#include "cpsc3120homework04part02.h"
#include <vector>
#include <climits>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>

using namespace std;

unsigned long long int minimumVotes(vector<unsigned long long> votes, vector<int> ecVotes, vector<string> abbr) {
    int n = votes.size();
    int totalEC = 0;

    // Calculate total EC votes available
    for (int v : ecVotes) totalEC += v;

    int target = totalEC / 2 + 1; // Minimum EC votes needed to win (i.e., 270 out of 538)

    // Create a list of state vote requirements
    vector<unsigned long long> minVotes(n);
    for (int i = 0; i < n; ++i) {
        // Minimum popular votes needed to win this state
        minVotes[i] = votes[i] / 2 + 1;
    }

    // DP array: dp[i] = minimum number of votes needed to gain exactly i EC votes
    vector<unsigned long long> dp(totalEC + 1, ULLONG_MAX);
    dp[0] = 0; // 0 votes to gain 0 EC votes

    // Standard 0/1 Knapsack: for each state, decide to include it or not
    for (int i = 0; i < n; ++i) {
        for (int j = totalEC; j >= ecVotes[i]; --j) {
            if (dp[j - ecVotes[i]] != ULLONG_MAX) {
                dp[j] = min(dp[j], dp[j - ecVotes[i]] + minVotes[i]);
            }
        }
    }

    // Find the minimum popular votes needed to win at least `target` EC votes
    unsigned long long result = ULLONG_MAX;
    for (int i = target; i <= totalEC; ++i) {
        if (dp[i] < result) {
            result = dp[i];
        }
    }

    return result;
}
