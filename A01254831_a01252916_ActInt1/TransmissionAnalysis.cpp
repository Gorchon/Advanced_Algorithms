#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Reads the content of a file into a single string
string readFromFile(const string &filePath)
{
    ifstream file(filePath); // Open the file
    string content, line;
    while (getline(file, line))
    {                    // Read each line
        content += line; // Append line to content
    }
    return content;
}

// KMP algorithm to find a pattern within a text
int KMP(const string &text, const string &pattern)
{
    if (pattern.empty())
        return 0; // Immediate match if pattern is empty

    int m = pattern.size();
    int n = text.size();
    vector<int> lps(m, 0); // Longest Prefix Suffix array

    // Build the LPS array
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

    // Search for the pattern in the text using the LPS array
    for (int i = 0, k = 0; i < n;)
    {
        if (text[i] == pattern[k])
        {
            i++, k++;
            if (k == m)
            {
                return i - k; // Match found
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
    return -1; // No match found
}

// Manacher's algorithm to find the longest palindromic substring
pair<int, int> findLongestPalindrome(const string &s)
{
    string T = "^"; // Start with a unique character
    for (char c : s)
    {
        T += "#" + string(1, c); // Insert # between characters to handle even length
    }
    T += "#$"; // End with unique characters

    int n = T.length();
    vector<int> P(n, 0); // Array to store the length of each palindrome
    int C = 0, R = 0;    // Current center and right edge

    // Process each character in the transformed string
    for (int i = 1; i < n - 1; i++)
    {
        int mirror = 2 * C - i; // Find the mirror of the current center
        if (i < R)
        {
            P[i] = min(R - i, P[mirror]);
        }
        // Attempt to expand palindrome centered at i
        while (T[i + 1 + P[i]] == T[i - 1 - P[i]])
        {
            P[i]++;
        }
        // Update center and right edge if expanded past R
        if (i + P[i] > R)
        {
            C = i;
            R = i + P[i];
        }
    }

    // Find the maximum length palindrome
    int maxLen = 0, centerIndex = 0;
    for (int i = 1; i < n - 1; i++)
    {
        if (P[i] > maxLen)
        {
            maxLen = P[i];
            centerIndex = i;
        }
    }

    int start = (centerIndex - maxLen) / 2; // Convert position to original string
    return make_pair(start, start + maxLen - 1);
}

// Find the longest common substring between two strings
pair<int, int> longestCommonSubstring(const string &s1, const string &s2)
{
    int n1 = s1.length(), n2 = s2.length();
    vector<vector<int>> dp(n1 + 1, vector<int>(n2 + 1, 0)); // 2D DP array
    int maxLength = 0, endAt = 0;

    // Build DP table
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

    return make_pair(endAt - maxLength + 1, endAt); // Adjust to 1-based indexing
}

// ANSI color codes
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string MAGENTA = "\033[1;35m"; // Bold magenta
const string RESET = "\033[0m";

int main()
{
    string transmissionFiles[] = {"transmission1.txt", "transmission2.txt"};
    string mcodeFiles[] = {"mcode1.txt", "mcode2.txt", "mcode3.txt"};

    // Read content from all files
    string transmissions[2];
    string mcodes[3];
    for (int i = 0; i < 2; ++i)
    {
        transmissions[i] = readFromFile(transmissionFiles[i]);
    }
    for (int i = 0; i < 3; ++i)
    {
        mcodes[i] = readFromFile(mcodeFiles[i]);
    }

    cout << MAGENTA << "Checking for malicious codes in transmissions:" << RESET << endl;
    // Check for malicious codes in transmissions
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            int foundAt = KMP(transmissions[i], mcodes[j]);
            cout << "Transmission " << (i + 1) << " - mcode " << (j + 1) << ": ";
            cout << (foundAt != -1 ? GREEN + "true " + RESET : RED + "false" + RESET);
            if (foundAt != -1)
            {
                cout << GREEN << " at position " << foundAt + 1 << RESET; // Output is 1-based index
            }
            cout << endl;
        }
    }

    cout << MAGENTA << "Longest palindromic substrings in transmission files:" << RESET << endl;
    // Find the longest palindromic substrings in transmission files
    for (int i = 0; i < 2; ++i)
    {
        pair<int, int> result = findLongestPalindrome(transmissions[i]);
        cout << "Transmission " << (i + 1) << ": Starts at " << result.first + 1 << ", ends at " << result.second + 1 << endl; // Output is 1-based index
    }

    cout << MAGENTA << "Longest common substring between the two transmissions:" << RESET << endl;
    // Find the longest common substring between the two transmissions
    pair<int, int> commonResult = longestCommonSubstring(transmissions[0], transmissions[1]);
    cout << "Starts at " << commonResult.first << ", ends at " << commonResult.second << endl;

    return 0;
}