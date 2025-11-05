#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <set>
#include <algorithm>
#include <climits>

using namespace std;

// ---------- Helper structures ----------

struct Edge {
    char u, v;
    int weight;
    bool directed;

    Edge(char a, char b, int w, bool d) : u(a), v(b), weight(w), directed(d) {}
};

// Union-Find for Kruskal's MST
struct UnionFind {
    map<char, char> parent;

    char find(char x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(char x, char y) {
        char rootX = find(x);
        char rootY = find(y);
        if (rootX != rootY)
            parent[rootY] = rootX;
    }

    void add(char x) {
        if (!parent.count(x)) parent[x] = x;
    }
};

// ---------- MST Function (Kruskal's) ----------
vector<string> getMST(vector<char> nodes, const vector<Edge>& edges) {
    UnionFind uf;
    for (char node : nodes) uf.add(node);

    vector<Edge> sortedEdges = edges;
    sort(sortedEdges.begin(), sortedEdges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    vector<string> mst;
    for (const auto& e : sortedEdges) {
        if (uf.find(e.u) != uf.find(e.v)) {
            uf.unite(e.u, e.v);
            string edgeID = string() + e.u + e.v;
            mst.push_back(edgeID);
        }
        if (mst.size() == nodes.size() - 1) break;
    }
    return mst;
}

// ---------- Floyd-Warshall and Intermediate Check ----------
void checkIntermediateShortening(vector<char> nodes, const vector<Edge>& edges) {
    int n = nodes.size();
    map<char, int> nodeIndex;
    for (int i = 0; i < n; ++i) nodeIndex[nodes[i]] = i;

    vector<vector<int>> dist(n, vector<int>(n, INT_MAX));
    vector<vector<int>> original = dist;

    // Initialize distances
    for (int i = 0; i < n; ++i) dist[i][i] = 0;

    for (const auto& e : edges) {
        int u = nodeIndex[e.u];
        int v = nodeIndex[e.v];
        dist[u][v] = min(dist[u][v], e.weight);
        if (!e.directed)
            dist[v][u] = min(dist[v][u], e.weight);
    }

    original = dist; // store original for comparison

    for (int k = 0; k < n; ++k) {
        char intermediate = nodes[k];
        vector<vector<int>> newDist = dist;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
                    dist[i][k] + dist[k][j] < newDist[i][j]) {
                    newDist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }

        // Compare and print improvements
        cout << "Using " << intermediate << " as intermediate:\n";
        bool anyShorter = false;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (newDist[i][j] < original[i][j]) {
                    cout << "  " << nodes[i] << " -> " << nodes[j]
                         << " improved from " << original[i][j]
                         << " to " << newDist[i][j] << "\n";
                    anyShorter = true;
                }
            }
        }
        if (!anyShorter)
            cout << "  No path shortened.\n";

        cout << "-------------------------\n";
    }
}

int main() {

    // Example nodes and edges

    vector<char> nodes = {'A', 'B', 'C', 'D', 'E'}; // add F for 4

    vector<Edge> edges = {
        //567
        Edge('A', 'B', 4, true),
        Edge('B', 'C', 5, true),
        Edge('C', 'A', 2, true),
        Edge('C', 'D', 2, true),
        Edge('D', 'B', 2, true),
        Edge('D', 'E', 1, true),
        Edge('E', 'B', 5, true),
        Edge('E', 'A', 1, true),
        Edge('E', 'C', 4, true),
        // 4
        // Edge('A', 'C', 5, false),
        // Edge('A', 'D', 3, false),
        // Edge('A', 'E', 2, false),
        // Edge('A', 'F', 2, false),
        // Edge('B', 'C', 1, false),
        // Edge('B', 'D', 5, false),
        // Edge('B', 'E', 4, false),
        // Edge('C', 'D', 3, false),
        // Edge('C', 'E', 5, false),
        // Edge('D', 'E', 5, false),
        // Edge('E', 'F', 5, false),
    };

    // Part 1: MST
    auto mst = getMST(nodes, edges);
    cout << "Edges in MST:\n";
    for (const auto& e : mst)
        cout << e << "\n";

    cout << "\n";

    // Part 2: Intermediate Node Path Shortening
    checkIntermediateShortening(nodes, edges);

    return 0;
}
