/*
 * C++ Program to Implement Dijkstra's and Floyd-Warshall Algorithms
 *
 * Computational Complexity:
 *
 * 1. Dijkstra's Algorithm:
 *    - Complexity per source node: O(n^2)
 *      - The graph is represented using an adjacency matrix. For each node, the algorithm must:
 *        1. Find the unvisited node with the minimum distance, which takes O(n) time.
 *        2. For each of the n nodes, check if a shorter path exists using the adjacent nodes, which also takes O(n).
 *      - Therefore, the overall complexity for finding the shortest path from one source node is O(n^2).
 *
 *    - Total complexity for all nodes: O(n^3)
 *      - Since Dijkstra's algorithm is run once for each of the n nodes (to find shortest paths from every node),
 *        the total complexity becomes O(n^2) * n = O(n^3).
 *
 * 2. Floyd-Warshall Algorithm:
 *    - Complexity: O(n^3)
 *      - The algorithm uses three nested loops:
 *        1. The outer loop iterates over each possible intermediate node (O(n)).
 *        2. The middle loop iterates over every possible start node (O(n)).
 *        3. The inner loop iterates over every possible end node (O(n)).
 *      - In each iteration, the algorithm checks if the path through the intermediate node offers a shorter distance.
 *      - Therefore, the overall complexity is O(n) * O(n) * O(n) = O(n^3).
 *
 * Steps to compile and run this program on a Linux system:
 *
 * 1. Compile the program:
 *    g++ -std=c++11 -o shortest_path main.cpp
 *
 * 2. Run the program with input redirection:
 *    ./shortest_path < in.txt
 *
 * 3. Format of input in `in.txt`:
 *    - First line: an integer n (number of nodes)
 *    - Next n x n values: adjacency matrix representing the directed graph.
 *      - -1 represents no direct edge between nodes i and j, except for diagonal elements which are 0.
 *
 * Author: José María Soto Valenzuela and Cesas Alan Silva Ramos
 */

/*
* Explanations of each test case

*Test Case 1:  Basic Connectivity with No Edges
*Explanation:
*This test case represents a graph with 4 nodes but no edges between them
* (other than self-loops). The Dijkstra’s algorithm should show that no paths
* exist between nodes except from a node to itself (0 distance), and the Floyd-Warshall
* output should be a matrix of 0 for diagonals and -1 everywhere else. */

/*
*Test Case 2: All Pairs Connected with Same Weights

*Explanation:
*This test case represents a fully connected graph of 3 nodes where all edge weights are equal
* (1). The Dijkstra’s algorithm should return a distance of 1 between any two different nodes,
 *and Floyd-Warshall should show the same distances between all pairs.
 */
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

/*
 * Helper function to find the minimum distance node for Dijkstra's Algorithm.
 * This function looks for the node with the smallest distance value from
 * the set of nodes that have not yet been visited.
 */
int minDistance(vector<int> &dist, vector<bool> &visited, int n)
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < n; v++)
        if (!visited[v] && dist[v] <= min)
        {
            min = dist[v], min_index = v;
        }

    return min_index;
}

/*
 * Dijkstra's Algorithm:
 *
 * Finds the shortest path from the source node to all other nodes in the graph.
 * The graph is represented by an adjacency matrix where graph[i][j] is the weight
 * of the edge from node i to node j. If no edge exists, the value is -1.
 */
void dijkstra(const vector<vector<int>> &graph, int src, int n)
{
    vector<int> dist(n, INT_MAX);   // Distance from source to each node
    vector<bool> visited(n, false); // True if node is processed

    dist[src] = 0; // The distance from the source to itself is 0

    // Loop to find the shortest path to all nodes
    for (int count = 0; count < n - 1; count++)
    {
        // Find the node with the smallest distance that hasn't been visited
        int u = minDistance(dist, visited, n);
        visited[u] = true; // Mark the node as visited

        // Update the distance of the adjacent nodes of the selected node
        for (int v = 0; v < n; v++)
        {
            if (!visited[v] && graph[u][v] != -1 && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
        }
    }

    // Print the results: shortest path from src to each node
    for (int i = 0; i < n; i++)
    {
        if (i != src && dist[i] != INT_MAX)
            cout << "node " << src + 1 << " to node " << i + 1 << " : " << dist[i] << endl;
    }
}

/*
 * Floyd-Warshall Algorithm:
 *
 * This algorithm computes the shortest paths between all pairs of nodes.
 * It uses a dynamic programming approach to update the shortest path between each pair (i, j)
 * by checking whether a shorter path can be found via an intermediate node k.
 */
void floydWarshall(vector<vector<int>> &graph, int n)
{
    vector<vector<int>> dist = graph; // Initialize distance matrix with the input graph

    // The core of the algorithm: iterating over each possible intermediate node k
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                // Only update if both dist[i][k] and dist[k][j] are not infinite (INT_MAX)
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX)
                {
                    if (dist[i][j] == INT_MAX || dist[i][j] > dist[i][k] + dist[k][j])
                    {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }

    // Print the shortest distance matrix
    cout << "Floyd :" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (dist[i][j] == INT_MAX)
                cout << "-1 "; // No path between i and j
            else
                cout << dist[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    int n; // Number of nodes in the graph
    cin >> n;

    // Input: adjacency matrix representing the graph
    vector<vector<int>> graph(n, vector<int>(n));

    // Reading the adjacency matrix
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> graph[i][j];
            if (graph[i][j] == -1 && i != j)
            {
                graph[i][j] = INT_MAX; // Set the value to INT_MAX for no edges
            }
        }
    }

    // Run Dijkstra's algorithm for each node as source
    cout << "Dijkstra :" << endl;
    for (int i = 0; i < n; i++)
    {
        dijkstra(graph, i, n);
    }

    // Run Floyd-Warshall algorithm to compute shortest paths between all pairs of nodes
    floydWarshall(graph, n);

    return 0;
}