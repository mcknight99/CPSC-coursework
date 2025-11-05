#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>

using namespace std;

// Disjoint Set (Union-Find) with Path Compression
class DisjointSet {
public:
    vector<int> parent, rank;

    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int v) {
        if (parent[v] != v)
            parent[v] = find(parent[v]); // Path Compression
        return parent[v];
    }

    bool unite(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU == rootV) return false;

        // Union by rank
        if (rank[rootU] > rank[rootV]) parent[rootV] = rootU;
        else if (rank[rootU] < rank[rootV]) parent[rootU] = rootV;
        else {
            parent[rootV] = rootU;
            rank[rootU]++;
        }
        return true;
    }
};

// Function to compute Maximum Spanning Tree
double maximumST(vector<vector<double>> adjacencyMatrix) {
    int n = adjacencyMatrix.size();
    vector<tuple<double, int, int>> edges;  // (weight, node1, node2)

    // Convert adjacency matrix to edge list
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; j++) {  // Avoid duplicate edges
            if (adjacencyMatrix[i][j] != -1) {
                edges.emplace_back(adjacencyMatrix[i][j], i, j);
            }
        }
    }

    // Sort edges in descending order by weight (Max-Heap behavior)
    sort(edges.rbegin(), edges.rend()); 

    DisjointSet ds(n);
    double maxSTWeight = 0;
    int edgeCount = 0;

    // Kruskal's Algorithm to find Maximum Spanning Tree
    for (const auto& [weight, u, v] : edges) {
        if (ds.unite(u, v)) {  // If no cycle is formed, add edge
            maxSTWeight += weight;
            edgeCount++;
            if (edgeCount == n - 1) break;  // MST complete when (n-1) edges are added
        }
    }

    return maxSTWeight;
}
