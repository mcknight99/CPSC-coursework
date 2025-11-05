#include <vector>
#include <iostream>

using namespace std;

vector<vector<double>> allPairsSP(vector<vector<double>> adjacencyMatrix, int i) {
    int n = adjacencyMatrix.size();
    
    // Initialize the distance matrix
    vector<vector<double>> dist = adjacencyMatrix;
    
    // Convert -1 (infinity) to a large number for computation
    double INF = 1e9;
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            if (dist[u][v] == -1 && u != v) {
                dist[u][v] = INF;  // No edge
            }
        }
    }

    // Floyd-Warshall Algorithm (Run until the i-th step)
    for (int k = 0; k < i; k++) {  // k is the intermediate vertex
        for (int u = 0; u < n; u++) {
            for (int v = 0; v < n; v++) {
                if (dist[u][k] != INF && dist[k][v] != INF) {  // Ensure paths exist
                    dist[u][v] = min(dist[u][v], dist[u][k] + dist[k][v]);
                }
            }
        }
    }

    // Convert INF back to -1 for consistency
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            if (dist[u][v] >= INF) {
                dist[u][v] = -1;
            }
        }
    }

    return dist;
}
