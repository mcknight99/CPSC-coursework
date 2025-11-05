#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm> // for std::max

using namespace std;

vector<int> longestIncreasingSubsequence(vector<int> numbers);

int main() {
    vector<int> numbers = {0, 7, -2, 6, -4, -6, -8, -1, 2, -7, -5, 4, 9, -3, 3, 5};
    vector<int> lis = longestIncreasingSubsequence(numbers);

    cout << "Longest Increasing Subsequence: ";
    for (int num : lis) {
        cout << num << " ";
    }
    cout << endl;

    return 0;

}

vector<int> longestIncreasingSubsequence(vector<int> numbers) {
    int n = numbers.size();
    if (n == 0) return {};

    // dp[i] stores the length of the LIS ending at index i
    vector<int> dp(n, 1);
    // parent[i] stores the index of the previous element in the LIS ending at i
    vector<int> parent(n, -1);

    int maxLength = 1;
    int maxEndIndex = 0;

    // Fill the dp and parent arrays
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (numbers[j] < numbers[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                parent[i] = j;
            }
        }
        // Keep track of the overall maximum length and ending index
        if (dp[i] > maxLength) {
            maxLength = dp[i];
            maxEndIndex = i;
        }
    }

    // Reconstruct the longest increasing subsequence
    vector<int> lis;
    for (int i = maxEndIndex; i >= 0; i = parent[i]) {
        lis.push_back(numbers[i]);
        if (parent[i] == -1) break;
    }

    // Reverse the sequence since we built it backwards
    reverse(lis.begin(), lis.end());
    return lis;
}
