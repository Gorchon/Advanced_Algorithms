#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Solution
{
public:
    string longestPalindrome(string s)
    {
        // Step 1: Preprocess the string by inserting '#' between every character
        if (s.empty())
            return ""; // Handle empty string case

        string t = "#";
        for (char c : s)
        {
            t += c;
            t += "#";
        }

        int n = t.size();
        vector<int> P(n, 0);       // Here we store the fucking radius of al characters
        int center = 0, right = 0; // To track the center and right boundary of the current palindrome
        int maxLen = 0;            // To track the length of the longest palindrome
        int maxCenter = 0;         // To track the center of the longest palindrome

        // Step 2: Manacher's Algorithm to calculate the palindrome radii
        for (int i = 0; i < n; i++)
        {
            // Step 3: Calculate the mirrored position of i with respect to the center
            int mirror = 2 * center - i;

            // If i is within the right boundary, use the mirrored value
            // example if i = 5, center = 3, mirror = 1, right = 7 as we can see i is within the right boundary so we can use the mirrored value
            if (i < right)
            {
                P[i] = min(right - i, P[mirror]);
                // This line sets the palindrome radius at i to the minimum of the distance between i and right and the palindrome radius at the mirrored position
                // it has to be the minimum because we can't expand beyond the right boundary for sureeee
            }

            // Step 4: Expand around center i
            int left = i - P[i] - 1;
            int right = i + P[i] + 1;

            while (right < n && left >= 0 && t[right] == t[left])
            {
                P[i]++;
                left--;
                right++;
            }

            // Update the center and right boundary if the palindrome expands beyond the current boundary
            if (i + P[i] > right) // an example would be if i = 5, P[i] = 2, right = 7, so we have to update the center and right boundary cause 5 + 2 > 7
            {
                center = i;
                right = i + P[i];
            }

            // Step 5: Update the longest palindrome found so far
            if (P[i] > maxLen)
            {
                maxLen = P[i];
                maxCenter = i;
            }
        }

        // Step 6: Recover the longest palindrome from the transformed string
        int start = (maxCenter - maxLen) / 2; // Find the start index in the original string
        return s.substr(start, maxLen);       // Return the longest palindromic substring
    }
};

// Main function to test the longestPalindrome function
int main()
{
    Solution solution;

    // Predefined test strings
    vector<string> testStrings = {
        "babad",  // Expected output: "bab" or "aba"
        "cbbd",   // Expected output: "bb"
        "a",      // Expected output: "a"
        "ac",     // Expected output: "a" or "c"
        "racecar" // Expected output: "racecar"
    };

    // Test each string
    for (const string &s : testStrings)
    {
        string result = solution.longestPalindrome(s);
        cout << "The longest palindromic substring in \"" << s << "\" is: " << result << endl;
    }

    return 0;
}
