#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <cmath>

/*
 * Steps to compile and run this program on a Linux system:
 *
 * 1. Compile the program:
 *    g++ -std=c++11 -o integrated2 main.cpp
 *
 * 2. Run the program with an input file:
 *    ./integrated2 < input.txt
 */

using namespace std;

// Structure to represent an edge in the graph
struct Edge
{
    int from;   // Starting node of the edge
    int to;     // Ending node of the edge
    int weight; // Weight of the edge (e.g., distance)
};

// Union-Find Disjoint Set class for Kruskal's Algorithm
class UnionFind
{
private:
    vector<int> parent; // Parent of each node in the disjoint set
    vector<int> rank;   // Rank (approximate depth) of each tree in the forest

public:
    // Constructor to initialize the Union-Find structure
    UnionFind(int n)
    {
        parent.resize(n);
        rank.resize(n, 0);
        // Initially, each node is its own parent (makeset operation)
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    // Find operation with path compression
    int find(int x)
    {
        // If x is not its own parent, recursively find the root and compress the path
        if (parent[x] != x)
            parent[x] = find(parent[x]); // Path compression
        return parent[x];
    }

    // Union operation by rank
    void unite(int x, int y)
    {
        int rootX = find(x); // Find root of x
        int rootY = find(y); // Find root of y

        if (rootX == rootY)
            return; // x and y are already in the same set

        // Attach the smaller rank tree under the root of the higher rank tree
        if (rank[rootX] < rank[rootY])
            parent[rootX] = rootY; // RootY becomes parent of RootX
        else if (rank[rootX] > rank[rootY])
            parent[rootY] = rootX; // RootX becomes parent of RootY
        else
        {
            parent[rootY] = rootX; // RootX becomes parent of RootY
            rank[rootX]++;         // Increase rank of RootX
        }
    }
};

// Part 1: Kruskal's Algorithm for Minimum Spanning Tree
vector<Edge> kruskalMST(int numNodes, vector<vector<int>> &graph)
{
    vector<Edge> edges;

    // Build the list of edges from the adjacency matrix
    for (int i = 0; i < numNodes; i++)
    {
        for (int j = i + 1; j < numNodes; j++)
        {
            if (graph[i][j] > 0)
            {
                // Add edge to the list if there is a connection
                edges.push_back({i, j, graph[i][j]});
            }
        }
    }

    // Sort edges by weight in ascending order
    sort(edges.begin(), edges.end(), [](Edge a, Edge b)
         { return a.weight < b.weight; });

    UnionFind uf(numNodes); // Initialize Union-Find structure
    vector<Edge> mst;       // Vector to store edges of the MST

    // Iterate over sorted edges and build MST
    for (auto &edge : edges)
    {
        // If adding this edge doesn't form a cycle
        if (uf.find(edge.from) != uf.find(edge.to))
        {
            uf.unite(edge.from, edge.to); // Union the sets
            mst.push_back(edge);          // Add edge to MST
        }
    }
    return mst;
}

// Global variables for TSP
int minTSPCost = INT_MAX; // Minimum total cost found so far
vector<int> optimalRoute; // Stores the best route found

// Part 2: Traveling Salesman Problem using brute-force (for small N)
void tsp(int startNode, vector<vector<int>> &graph, vector<bool> &visited, vector<int> &currentPath, int currentCost, int numNodes)
{
    // Base case: if all nodes have been visited
    if (currentPath.size() == numNodes)
    {
        // Add cost to return to the starting node
        currentCost += graph[currentPath.back()][startNode];
        // Update minimum cost and best route if current cost is lower
        if (currentCost < minTSPCost)
        {
            minTSPCost = currentCost;
            optimalRoute = currentPath;
        }
        return;
    }

    // Try all possible next nodes
    for (int i = 0; i < numNodes; i++)
    {
        if (!visited[i])
        {
            // Choose the next node
            visited[i] = true;
            currentPath.push_back(i);
            currentCost += graph[currentPath[currentPath.size() - 2]][i];

            // Recursive call to explore further
            tsp(startNode, graph, visited, currentPath, currentCost, numNodes);

            // Backtrack to previous state
            currentCost -= graph[currentPath[currentPath.size() - 2]][i];
            currentPath.pop_back();
            visited[i] = false;
        }
    }
}

// Part 3: Ford-Fulkerson Algorithm for Maximum Flow using DFS
bool dfs(vector<vector<int>> &residualGraph, int current, int sink, vector<int> &parent)
{
    if (current == sink)
        return true; // Reached sink node

    for (int next = 0; next < residualGraph.size(); ++next)
    {
        // If there is available capacity and next node is unvisited
        if (residualGraph[current][next] > 0 && parent[next] == -1)
        {
            parent[next] = current; // Set parent for path reconstruction
            if (dfs(residualGraph, next, sink, parent))
                return true; // Path to sink found
        }
    }
    return false; // No path found from current node
}

int maxFlow(vector<vector<int>> &graph, int source, int sink)
{
    int totalFlow = 0;                         // Initialize total flow to zero
    vector<vector<int>> residualGraph = graph; // Create residual graph
    vector<int> parent(graph.size());          // To store augmenting path

    while (true)
    {
        fill(parent.begin(), parent.end(), -1); // Reset parent array
        parent[source] = -2;                    // Mark source node as visited

        // Use DFS to find an augmenting path
        if (!dfs(residualGraph, source, sink, parent))
            break; // No more augmenting paths, algorithm terminates

        // Find the bottleneck (minimum residual capacity) along the path found
        int flow = INT_MAX;
        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            flow = min(flow, residualGraph[u][v]);
        }

        // Update residual capacities along the path
        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            residualGraph[u][v] -= flow; // Subtract flow from forward edge
            residualGraph[v][u] += flow; // Add flow to reverse edge
        }

        totalFlow += flow; // Add flow to total flow
    }
    return totalFlow;
}

// Helper function to convert index to character (e.g., 0 -> 'A')
char indexToChar(int index)
{
    return 'A' + index;
}

int main()
{
    int numNodes;
    cin >> numNodes;

    vector<vector<int>> distanceMatrix(numNodes, vector<int>(numNodes)); // Distance between neighborhoods
    vector<vector<int>> capacityMatrix(numNodes, vector<int>(numNodes)); // Capacity between neighborhoods
    vector<pair<int, int>> coordinates(numNodes);                        // Coordinates of the exchanges

    // Read distance matrix
    for (int i = 0; i < numNodes; i++)
    {
        for (int j = 0; j < numNodes; j++)
        {
            cin >> distanceMatrix[i][j];
        }
    }

    // Read capacity matrix
    for (int i = 0; i < numNodes; i++)
    {
        for (int j = 0; j < numNodes; j++)
        {
            cin >> capacityMatrix[i][j];
        }
    }

    // Read coordinates of exchanges
    for (int i = 0; i < numNodes; i++)
    {
        char ch;
        cin >> ch; // Read '('
        int x;
        cin >> x;
        cin >> ch; // Read ','
        int y;
        cin >> y;
        cin >> ch; // Read ')'
        coordinates[i] = {x, y};
    }

    // Part 1: Optimal wiring (Minimum Spanning Tree)
    vector<Edge> mst = kruskalMST(numNodes, distanceMatrix);
    cout << "1. Way of wiring the neighborhoods with fiber (list of arcs):" << endl;
    for (auto &edge : mst)
    {
        cout << "(" << indexToChar(edge.from) << "," << indexToChar(edge.to) << ")" << endl;
    }

    // Part 2: Mail delivery route (Traveling Salesman Problem)
    minTSPCost = INT_MAX;
    optimalRoute.clear();
    vector<bool> visited(numNodes, false);
    vector<int> currentPath;
    int startNode = 0; // Start from node 0 ('A')
    visited[startNode] = true;
    currentPath.push_back(startNode);
    tsp(startNode, distanceMatrix, visited, currentPath, 0, numNodes);

    cout << "2. Route to be followed by the mail delivery personnel:" << endl;
    for (int i = 0; i < optimalRoute.size(); i++)
    {
        cout << indexToChar(optimalRoute[i]) << " -> ";
    }
    cout << indexToChar(startNode) << endl; // Return to starting node

    // Part 3: Maximum information flow value from the initial node to the final node
    int sourceNode = 0;          // Initial node ('A')
    int sinkNode = numNodes - 1; // Final node (last node)
    int max_flow = maxFlow(capacityMatrix, sourceNode, sinkNode);
    cout << "3. Maximum information flow value from the initial node to the final node: " << max_flow << endl;

    // Part 4: List of polygons (coordinates of exchanges)
    // Output the coordinates of the exchanges
    cout << "4. List of polygons (coordinates of exchanges):" << endl;
    for (int i = 0; i < numNodes; i++)
    {
        cout << "(" << coordinates[i].first << "," << coordinates[i].second << ")" << endl;
    }

    return 0;
}
