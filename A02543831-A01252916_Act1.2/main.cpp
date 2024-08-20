// On this file we will compare 2 approachs to solve a very common problem in computer science, "Coin Change Problem".
// The problem is as follows: given a set of coins, and a target value, find the minimum number of coins needed to reach the target value.

//  The first approach will be using a Greedy algoritms which will always choose the coin with the highest value that is less than or equal to the remaining target value.
//  We'll see that using this approach we can't always get the optimal solution. This is because we depend a lot on inputs and the order of the coins. This approach does not
// understand delayed gratification, it only cares about the immediate reward.

// The second approach will be using a Dynamic Programming algorithm. This approach will always give us the optimal solution. And unlike the Greedy approach,
// it does not depend on the order of the coins. And unlike brute forcer it does not need to recalculate the same subproblems over and over again. making it much more
// efficient for solving this problem for a general solution.

// Here are some point that we will consider when we create our dynamic programming solution:
//  1. Do not rely on inputs
//  2. Do not rely on the order of the coins, we do not need any kind of sorting
//  3. We will use a bottom-up approach to solve the problem

#include <iostream>
#include <vector>

using namespace std;

void printCoins(vector<int> &coins)
{
    for (int coin : coins)
    {
        cout << coin << endl;
    }
}

vector<int> changeUsingGreedy(int change, vector<int> &denominations)
{
    vector<int> coinsUsed(denominations.size(), 0);
    for (int i = 0; i < denominations.size(); i++)
    {
        while (change >= denominations[i])
        {
            change -= denominations[i];
            coinsUsed[i]++;
        }
    }
    return coinsUsed;
}

int main()
{
    int N;
    cin >> N;
    vector<int> denominations(N);
    for (int i = 0; i < N; ++i)
    {
        cin >> denominations[i];
    }

    int P, Q;
    cin >> P >> Q;

    int change = Q - P;

    sort(denominations.begin(), denominations.end(), greater<int>());

    cout << "this is the greedy result" << endl;

    // Use Greedy Algorithm
    vector<int> greedyResult = changeUsingGreedy(change, denominations);
    printCoins(greedyResult);

    cout << "this is the dp result" << endl;

    return 0;
}
