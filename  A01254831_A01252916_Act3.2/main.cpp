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
   g++ -std=c++11 -o shortest_path main.cpp
 *
 * 2. Run the program with input redirection:
./shortest_path < input1.txt > output1.txt
./shortest_path < input2.txt > output2.txt
 ./shortest_path < input3.txt > output3.txt
./shortest_path < input4.txt > output4.txt
./shortest_path < input5.txt > output5.txt
 *
 * 3. Format of input in `in.txt`:
 *    - First line: an integer n (number of nodes)
 *    - Next n x n values: adjacency matrix representing the directed graph.
 *      - -1 represents no direct edge between nodes i and j, except for diagonal elements which are 0.
 *
 * Authors: José María Soto Valenzuela and Cesas Alan Silva Ramos
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

/*
* Test Case 3: Mixed Graph with Different Weights and No Direct Connections

* This graph has a mix of different weights, some disconnected nodes, and various
 *+possible paths between pairs of nodes. Dijkstra's algorithm will show different distances between the
 * nodes, while the Floyd-Warshall matrix will reveal the shortest paths, even filling in where direct connections
  * are missing. */

/*
* Test Case 4: Large Dense Graph

* This is a larger, dense graph with 5 nodes and various weights. Both Dijkstra’s and Floyd-Warshall algorithms should compute
*  multiple paths and ensure they all match the shortest paths. Like this teacher you can see that the algorithm handles larger
 * graphs correctly and efficiently. */

/* *
*  Test Case 5: negative cycles on a graph
* This test case is useful because it challenges the program with a graph that contains negative edge weights and a negative cycle, allowing us to:

* Verify that Dijkstra's algorithm is not applied when inappropriate.
* Confirm that the Floyd-Warshall algorithm correctly detects negative cycles.
* Ensure that the program handles complex scenarios gracefully.
* Validate the correctness and robustness of the implemented algorithms. */

#include <iostream>
#include <vector>
#include <climits>

using namespace std;

/*
 * Helper function to find the minimum distance node for Dijkstra's Algorithm.
 * This function looks for the node with the smallest distance value from
 * the set of nodes that have not yet been visited.
 */
int minDistance(vector<int> &dist, vector<bool> &visited, int n) // *  This is kinda of our min heap to be adding each value to our "minheap" and then be able to extract the min value
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

// Dijkstra's Algorithm:
void dijkstra(const vector<vector<int>> &graph, int src, int n)
{
    vector<int> dist(n, INT_MAX);   // The distance from source to each node
    vector<bool> visited(n, false); // Mark nodes as visited

    dist[src] = 0; // The distance from the source to itself is 0

    // Loop to find the shortest path to all nodes
    for (int count = 0; count < n; count++)
    { // Run the loop n times
        // Find the node with the smallest distance that hasn't been visited
        int u = minDistance(dist, visited, n);

        // If no more reachable nodes, break early
        if (dist[u] == INT_MAX)
            break;

        visited[u] = true; // Mark the node as visited

        // Update the distance of the adjacent nodes of the selected node
        for (int v = 0; v < n; v++)
        {
            // Only update if the node is not visited and there is a direct edge
            if (!visited[v] && graph[u][v] != INT_MAX && dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v]; // Relax the edge
            }
        }
    }

    // Print the results: shortest path from src to each node
    for (int i = 0; i < n; i++)
    {
        if (i != src)
        {
            if (dist[i] == INT_MAX)
                cout << "node " << src + 1 << " to node " << i + 1 << " : there is no path" << endl;
            else
                cout << "node " << src + 1 << " to node " << i + 1 << " : " << dist[i] << endl;
        }
    }
}

/*
 * Floyd-Warshall Algorithm:
 *
 * This algorithm computes the shortest paths between all pairs of nodes.
 * It uses a dynamic programming approach to update the shortest path between each pair (i, j)
 * by checking whether a shorter path can be found via an intermediate node k.
 */
// Floyd-Warshall Algorithm
// Floyd-Warshall algorithm implementation
void floydWarshall(const vector<vector<int>> &graph)
{
    int n = graph.size();
    vector<vector<int>> dist = graph;
    int INF = INT_MAX / 2; // Use INT_MAX / 2 to prevent overflows

    // Initialize distance matrix
    for (int i = 0; i < n; i++)
    {
        dist[i][i] = 0; // Distance to self is zero
        for (int j = 0; j < n; j++)
        {
            if (dist[i][j] == -1)
                dist[i][j] = INF;
        }
    }

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            if (dist[i][k] >= INF / 2)
                continue;
            for (int j = 0; j < n; j++)
            {
                if (dist[k][j] >= INF / 2)
                    continue;
                if (dist[i][j] > dist[i][k] + dist[k][j] && dist[i][k] + dist[k][j] < INF)
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // Check for negative cycles
    for (int i = 0; i < n; i++)
    {
        if (dist[i][i] < 0)
        {
            cout << endl
                 << "Negative cycle detected." << endl;
            return;
        }
    }

    // Output the result
    cout << endl
         << "Floyd :" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (dist[i][j] >= INF / 2)
                cout << "-1 ";
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
    floydWarshall(graph);

    return 0;
}