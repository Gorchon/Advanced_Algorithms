// On this file we will compare 2 approachs to solve a very common problem in computer science, "Coin Change Problem".
// The problem is as follows: given a set of coins, and a target value, find the minimum number of coins needed to reach the target value.

// To run this code you can use the following command:
// g++ -o main main.cpp
// And then to read the input from our test file you can use the following command:
// ./main < in.txt

#include <iostream>
#include <vector>

using namespace std;

//  The first approach will be using a Greedy algoritm, which will always choose the coin with the highest value that is less than or equal to the remaining target value.
//  We'll see that using this approach we can't always get the optimal solution. This is because we depend a lot on inputs and the order of the coins. This approach does not
// understand delayed gratification, it only cares about the immediate reward.

vector<int> GreedyApproach(int change, vector<int> &CurrencyUnits)
{

    // Time Complexity: O(n log n) for sorting, where n is the size of CurrencyUnits.
    //                  O(n * (change / min(CurrencyUnits))) for the loop, in the worst case.
    // Space Complexity: O(n) for the coinsUsed vector, where n is the size of CurrencyUnits.

    // sort it to ensure this will always be a greedy approach
    sort(CurrencyUnits.rbegin(), CurrencyUnits.rend());
    // Initialize a vector to store the number of coins used for each kind of coin.
    vector<int> coinsUsed(CurrencyUnits.size(), 0);

    // Iterate through each denomination in the CurrencyUnits vector.
    // Greedy approach: Start from the largest denomination (assuming CurrencyUnits is sorted in descending order).
    for (int i = 0; i < CurrencyUnits.size(); i++)
    {
        // While the current denomination can still be subtracted from the remaining change,
        // keep subtracting it and increment the corresponding counter in coinsUsed.
        // This step represents the greedy choice: always use the largest denomination possible.
        while (change >= CurrencyUnits[i])
        {
            change -= CurrencyUnits[i]; // Subtract the denomination from the remaining change.
            coinsUsed[i]++;             // Increment the count for this denomination.
        }
    }

    // Return the list of counts of coins/bills used for each denomination.
    // This reflects the result of applying the greedy strategy.
    return coinsUsed;
}

// The second approach will be using a Dynamic Programming algorithm. This approach will always give us the optimal solution. And unlike the Greedy approach,
// it does not depend on the order of the coins. And unlike brute forcer it does not need to recalculate the same subproblems over and over again. making it much more
// efficient for solving this problem for a general solution.

// Here are some point that we will consider when we create our dynamic programming solution:
//  1. Do not rely on inputs
//  2. Do not rely on the order of the coins, we do not need any kind of sorting
//  3. We will use a bottom-up approach to solve the problem

vector<int> DymamicProgrammingApproach(int change, vector<int> &CurrencyUnits)
{
    // Time Complexity: O(n * change), where n is the number of denominations.
    // Space Complexity: O(change) for the dp and coinsUsed arrays.

    // This is the array that will store the minimum number of coins needed to make change for each amount from 0 to 'change'.
    // this technique is called bottom-up dynamic programming. We start from the smallest subproblem and build our way up to the final problem.
    //  we use INT_MAX to represent that it is not possible to make change for that amount
    // it is like saying “I don’t know how to make change for this amount yet”.
    vector<int> dp(change + 1, INT_MAX);

    // The coinsUsed array is used to track which coin was used last to achieve the minimum coin count for each amount.
    // This allows you to reconstruct the exact set of coins that make up the optimal
    // solution after you have determined the minimum number of coins required.
    vector<int> coinsUsed(change + 1, -1);

    // Just setting the base case for 0 amount
    dp[0] = 0;

    // go through all amounts from 1 to 'change'
    for (int i = 1; i <= change; i++)
    {
        // Check every coin denomination to find the minimum number of coins needed
        for (int j = 0; j < CurrencyUnits.size(); j++)
        {
            if (i >= CurrencyUnits[j] && dp[i - CurrencyUnits[j]] != INT_MAX)
            {
                if (dp[i] > dp[i - CurrencyUnits[j]] + 1) // check if the current solution is better than the previous one
                {
                    dp[i] = dp[i - CurrencyUnits[j]] + 1; // Update the minimum number of coins needed
                    coinsUsed[i] = j;                     // Store the index of the coin used
                }
            }
        }
    }

    // If change can't be made by any combination of coins, return an empty vector
    if (dp[change] == INT_MAX)
    {
        cout << "It is not possible to make change for this amount\n";
        return vector<int>();
    }

    // Reconstruct the solution to find which coins were used
    vector<int> result(CurrencyUnits.size(), 0);
    int remainingChange = change;

    while (remainingChange > 0)
    {
        int coinIndex = coinsUsed[remainingChange];
        result[coinIndex]++;
        remainingChange -= CurrencyUnits[coinIndex];
    }

    return result;
}

void printCoins(vector<int> &coins, vector<int> &CurrencyUnits, int change)
{

    cout << "Total change to provide:\033[1;31m " << change << " \033[0munits\n";
    for (int i = 0; i < coins.size(); ++i)
    {
        if (coins[i] > 0)
        {
            cout << "CurrencyUnits: \033[1;32m" << CurrencyUnits[i] << "\033[0m - Number of coins: \033[1;33m" << coins[i] << "\033[0m" << endl;
        }
    }
}

int main()
{
    int N;
    cin >> N;
    vector<int> CurrencyUnits(N);
    for (int i = 0; i < N; ++i)
    {
        cin >> CurrencyUnits[i];
    }

    int P, Q;
    cin >> P >> Q;

    int change = Q - P;

    sort(CurrencyUnits.begin(), CurrencyUnits.end(), greater<int>());

    cout << "\033[1;35mThis is the Greedy result: \033[0m" << endl;

    // Use Greedy Algorithm
    vector<int> greedyResult = GreedyApproach(change, CurrencyUnits);
    printCoins(greedyResult, CurrencyUnits, change);

    cout << "\033[1;35mThis is the DP result: \033[0m" << endl;
    vector<int> dpResult = DymamicProgrammingApproach(change, CurrencyUnits);
    printCoins(dpResult, CurrencyUnits, change);

    return 0;
}
