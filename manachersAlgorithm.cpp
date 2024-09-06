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
        vector<int> P(n, 0);       // Array to store the radius of palindromes centered at each character
        int center = 0, right = 0; // To track the center and right boundary of the current palindrome
        int maxLen = 0;            // To track the length of the longest palindrome
        int maxCenter = 0;         // To track the center of the longest palindrome

        // Step 2: Manacher's Algorithm to calculate the palindrome radii
        for (int i = 0; i < n; i++)
        {
            // Step 3: Calculate the mirrored position of i with respect to the center
            int mirror = 2 * center - i;

            // If i is within the right boundary, use the mirrored value
            if (i < right)
            {
                P[i] = min(right - i, P[mirror]);
            }

            // Step 4: Expand around center i
            while (i + P[i] + 1 < n && i - P[i] - 1 >= 0 && t[i + P[i] + 1] == t[i - P[i] - 1])
            {
                P[i]++;
            }

            // Update the center and right boundary if the palindrome expands beyond the current boundary
            if (i + P[i] > right)
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
    string s;

    // Take input from the user
    cout << "Enter a string: ";
    cin >> s;

    // Call the longestPalindrome function
    string result = solution.longestPalindrome(s);

    // Output the result
    cout << "The longest palindromic substring is: " << result << endl;

    return 0;
}
