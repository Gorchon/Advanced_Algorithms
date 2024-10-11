#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
This program solves the 0/1 Knapsack problem. The problem is about selecting items, each with a value and weight,
to maximize the total value in the knapsack without exceeding its weight capacity.
We use dynamic programming (DP) to solve this problem.

Time complexity:
The time complexity of this solution is O(N * W), where N is the number of items and W is the capacity of the knapsack.
This is because we have two nested loops: the outer one iterates over each item, and the inner one iterates over each possible weight from 0 to W.

Space complexity:
The space complexity is also O(N * W) since we use a 2D DP array of size (N + 1) x (W + 1) to store intermediate results.

Why are the 4 test cases useful?
- Test 1 is a basic small input case that checks if the program works with typical inputs.
- Test 2 ensures the program handles situations where the knapsack's capacity is smaller than many item weights, forcing it to make difficult choices.
- Test 3 is an edge case with only one item, testing how the program performs with minimal input.
- Test 4 is a larger input case that tests the efficiency and scalability of the program with more items and a higher weight capacity.
*/
/*
 * Steps to compile and run this program on a Linux system:
 *
 * 1. Compile the program:
g++ -std=c++11 -o knapsack main.cpp
 *
 * 2. Run the program with input redirection:
./knapsack < input1.txt
./knapsack < input2.txt
./knapsack < input3.txt
./knapsack < input4.txt
*/
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
