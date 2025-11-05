#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <climits>

using namespace std;

void minPopularVoteWin(const vector<tuple<char, int, int>>& states);

int main() {
    // Example states with (state ID, popular votes, electoral votes)
    // vector<tuple<char, int, int>> states = {
    //     {'D', 17, 2},
    //     {'K', 12, 3},
    //     {'M', 10, 9},
    //     {'W', 14, 3},
    //     {'Z', 13, 4}
    // };

    vector<tuple<char, int, int>> states = {
        {'F', 15, 9},
        {'O', 11, 5},
        {'P', 18, 6},
        {'T', 12, 4},
        {'Z', 19, 7}
    };

    minPopularVoteWin(states);

    return 0;

}

void minPopularVoteWin(const vector<tuple<char, int, int>>& states) {
    int n = states.size();
    int totalEV = 0;
    vector<int> minVotesToWin(n);  // votes needed to win each state
    vector<int> electoralVotes(n);
    vector<char> stateIDs(n);

    // Prepare data
    for (int i = 0; i < n; ++i) {
        char id;
        int popVotes, ev;
        tie(id, popVotes, ev) = states[i];
        stateIDs[i] = id;
        electoralVotes[i] = ev;
        minVotesToWin[i] = (popVotes / 2) + 1;
        totalEV += ev;
    }

    int targetEV = totalEV / 2 + 1;  // majority needed

    // dp[ev] = min popular votes needed to get exactly 'ev' electoral votes
    vector<int> dp(totalEV + 1, INT_MAX);
    vector<vector<bool>> taken(n + 1, vector<bool>(totalEV + 1, false)); // for traceback
    dp[0] = 0;

    // 0/1 knapsack DP: minimize votes to reach at least targetEV electoral votes
    for (int i = 0; i < n; ++i) {
        for (int ev = totalEV; ev >= electoralVotes[i]; --ev) {
            if (dp[ev - electoralVotes[i]] != INT_MAX) {
                int newVotes = dp[ev - electoralVotes[i]] + minVotesToWin[i];
                if (newVotes < dp[ev]) {
                    dp[ev] = newVotes;
                    taken[i][ev] = true;
                }
            }
        }
    }

    // Find the minimum popular vote total that gets >= targetEV
    int bestEV = -1;
    int minVotes = INT_MAX;
    for (int ev = targetEV; ev <= totalEV; ++ev) {
        if (dp[ev] < minVotes) {
            minVotes = dp[ev];
            bestEV = ev;
        }
    }

    // Trace back to find which states were selected
    vector<char> statesWon;
    int ev = bestEV;
    for (int i = n - 1; i >= 0; --i) {
        if (ev >= electoralVotes[i] && taken[i][ev]) {
            statesWon.push_back(stateIDs[i]);
            ev -= electoralVotes[i];
        }
    }

    // Output
    cout << "Electoral votes won: " << bestEV << "\n";
    cout << "States won: ";
    for (char id : statesWon)
        cout << id << " ";
    cout << "\n";
}
