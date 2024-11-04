#include <vector>
#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;

class Solution
{
public:
    int findTheCity(int n, vector<vector<int>> &edges, int distanceThreshold)
    {
        // Step 1: Initialize the distance matrix
        vector<vector<int>> dist(n, vector<int>(n, INT_MAX));

        // Set distance from each city to itself as 0
        for (int i = 0; i < n; ++i)
        {
            dist[i][i] = 0;
        }

        // Initialize distances based on the edges
        for (const auto &edge : edges)
        {
            int u = edge[0], v = edge[1], weight = edge[2];
            dist[u][v] = weight;
            dist[v][u] = weight;
        }

        // Step 2: Apply Floyd-Warshall algorithm
        for (int k = 0; k < n; ++k)
        {
            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
                        dist[i][j] > dist[i][k] + dist[k][j])
                    {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }

        // Step 3: Count reachable cities within the threshold for each city
        int minReachableCities = n; // maximum possible initial value
        int resultCity = -1;

        for (int i = 0; i < n; ++i)
        {
            int reachableCities = 0;
            for (int j = 0; j < n; ++j)
            {
                if (i != j && dist[i][j] <= distanceThreshold)
                {
                    reachableCities++;
                }
            }

            // Step 4: Update result based on the smallest number of reachable cities
            if (reachableCities < minReachableCities)
            {
                minReachableCities = reachableCities;
                resultCity = i;
            }
            else if (reachableCities == minReachableCities)
            {
                // If there's a tie, choose the city with the greatest number
                resultCity = max(resultCity, i);
            }
        }

        return resultCity;
    }
};

int main()
{
    Solution solution;

    // Test Case 1
    int n1 = 4;
    vector<vector<int>> edges1 = {
        {0, 1, 3},
        {1, 2, 1},
        {2, 3, 4},
        {0, 3, 7}};
    int distanceThreshold1 = 4;
    cout << "Test Case 1: Expected output = 3, Actual output = "
         << solution.findTheCity(n1, edges1, distanceThreshold1) << endl;

    // Test Case 2
    int n2 = 5;
    vector<vector<int>> edges2 = {
        {0, 1, 2},
        {1, 2, 3},
        {2, 3, 1},
        {3, 4, 5},
        {0, 4, 8}};
    int distanceThreshold2 = 6;
    cout << "Test Case 2: Expected output = 0, Actual output = "
         << solution.findTheCity(n2, edges2, distanceThreshold2) << endl;

    // Test Case 3
    int n3 = 6;
    vector<vector<int>> edges3 = {
        {0, 1, 10},
        {0, 2, 5},
        {1, 2, 6},
        {1, 3, 15},
        {2, 4, 9},
        {4, 5, 3}};
    int distanceThreshold3 = 10;
    cout << "Test Case 3: Expected output = 5, Actual output = "
         << solution.findTheCity(n3, edges3, distanceThreshold3) << endl;

    // Test Case 4
    int n4 = 4;
    vector<vector<int>> edges4 = {
        {0, 1, 1},
        {1, 2, 1},
        {2, 3, 1},
        {0, 3, 4}};
    int distanceThreshold4 = 2;
    cout << "Test Case 4: Expected output = 0, Actual output = "
         << solution.findTheCity(n4, edges4, distanceThreshold4) << endl;

    return 0;
}