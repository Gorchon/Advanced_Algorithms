#include <vector>
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
