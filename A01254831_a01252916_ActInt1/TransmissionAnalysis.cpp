#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Function to read the entire content of a file
string readFromFile(const string &filePath)
{
    ifstream file(filePath);
    string content, line;
    while (getline(file, line))
    {
        content += line;
    }
    return content;
}

// KMP algorithm to search for a substring
int KMP(const string &text, const string &pattern)
{
    if (pattern.empty())
    {
        return 0;
    }

    int m = pattern.size();
    int n = text.size();
    vector<int> lps(m, 0);

    // Preprocess the pattern to create the LPS array
    for (int i = 1, length = 0; i < m;)
    {
        if (pattern[i] == pattern[length])
        {
            lps[i++] = ++length;
        }
        else if (length)
        {
            length = lps[length - 1];
        }
        else
        {
            lps[i++] = 0;
        }
    }

    // Search the pattern in the text
    for (int i = 0, k = 0; i < n;)
    {
        if (text[i] == pattern[k])
        {
            i++, k++;
            if (k == m)
            {
                return i - k;
            }
        }
        else if (k)
        {
            k = lps[k - 1];
        }
        else
        {
            i++;
        }
    }
    return -1;
}

// Manacher's algorithm to find the longest palindromic substring
pair<int, int> findLongestPalindrome(const string &s)
{
    string T = "^";
    for (char c : s)
    {
        T += "#" + string(1, c);
    }
    T += "#$";

    int n = T.length();
    vector<int> P(n, 0);
    int C = 0, R = 0;

    for (int i = 1; i < n - 1; i++)
    {
        int mirror = 2 * C - i;
        if (i < R)
        {
            P[i] = min(R - i, P[mirror]);
        }
        while (T[i + 1 + P[i]] == T[i - 1 - P[i]])
        {
            P[i]++;
        }
        if (i + P[i] > R)
        {
            C = i;
            R = i + P[i];
        }
    }

    int maxLen = 0, centerIndex = 0;
    for (int i = 1; i < n - 1; i++)
    {
        if (P[i] > maxLen)
        {
            maxLen = P[i];
            centerIndex = i;
        }
    }

    int start = (centerIndex - maxLen) / 2;
    return make_pair(start, start + maxLen - 1);
}

// Function to find the longest common substring using dynamic programming
pair<int, int> longestCommonSubstring(const string &s1, const string &s2)
{
    int n1 = s1.length(), n2 = s2.length();
    vector<vector<int>> dp(n1 + 1, vector<int>(n2 + 1, 0));
    int maxLength = 0, endAt = 0;

    for (int i = 1; i <= n1; ++i)
    {
        for (int j = 1; j <= n2; ++j)
        {
            if (s1[i - 1] == s2[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > maxLength)
                {
                    maxLength = dp[i][j];
                    endAt = i;
                }
            }
        }
    }

    return make_pair(endAt - maxLength + 1, endAt);
}

int main()
{
    string transmissionFiles[] = {"transmission1.txt", "transmission2.txt"};
    string mcodeFiles[] = {"mcode1.txt", "mcode2.txt", "mcode3.txt"};

    string transmissions[2] = {readFromFile(transmissionFiles[0]), readFromFile(transmissionFiles[1])};
    string mcodes[3] = {readFromFile(mcodeFiles[0]), readFromFile(mcodeFiles[1]), readFromFile(mcodeFiles[2])};

    // Part 1: Check for malicious codes in transmissions
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            int foundAt = KMP(transmissions[i], mcodes[j]);
            cout << (foundAt != -1 ? "true " : "false");
            if (foundAt != -1)
            {
                cout << foundAt + 1; // Output is 1-based index
            }
            cout << endl;
        }
    }

    // Part 2: Find the longest palindromic substrings in transmission files
    for (const string &transmission : transmissions)
    {
        pair<int, int> result = findLongestPalindrome(transmission);
        cout << result.first + 1 << " " << result.second + 1 << endl; // Output is 1-based index
    }

    // Part 3: Find the longest common substring between the two transmissions
    pair<int, int> commonResult = longestCommonSubstring(transmissions[0], transmissions[1]);
    cout << commonResult.first << " " << commonResult.second << endl;

    return 0;
}
