#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Function to solve the knapsack problem
int knapsack(int W, const vector<int> &values, const vector<int> &weights, int N)
{
    vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= N; i++)
    {
        for (int w = 0; w <= W; w++)
        {
            if (weights[i - 1] <= w)
            {
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + values[i - 1]);
            }
            else
            {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    return dp[N][W];
}

int main()
{
    int N;
    cin >> N; // Number of elements

    vector<int> values(N);
    vector<int> weights(N);

    // Reading values
    for (int i = 0; i < N; i++)
    {
        cin >> values[i];
    }

    // Reading weights
    for (int i = 0; i < N; i++)
    {
        cin >> weights[i];
    }

    int W;
    cin >> W; // Maximum weight capacity of the knapsack

    // Solve the knapsack problem and output the result
    cout << knapsack(W, values, weights, N) << endl;

    return 0;
}
