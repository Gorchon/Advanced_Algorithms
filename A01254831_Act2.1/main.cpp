// Objective: Check if all strings in a file are distinct using string hashing.

// Include necessary headers
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set> // Using unordered_set to store hashed values

using namespace std;

// Hashing Strategy Explanation:
// We're using a basic polynomial rolling hash function. This strategy is efficient for strings.
// We treat each string as a base-256 number (since there are 256 possible ASCII characters) and compute
// its hash value by accumulating powers of a small prime number (31). The hash function converts each string
// into a large number that will be unique (with very high probability) for distinct strings.
// We use modulo to avoid overflow, ensuring numbers don't grow too large.
//
// The hash function ensures that even strings with similar characters will have distinct hash values.

const int MOD = 1e9 + 7; // Large prime number to avoid collisions due to overflow
const int BASE = 31;     // Base for the polynomial hash function

// Function to compute the hash of a string using polynomial rolling hash
unsigned long long computeHash(const string &str)
{
    unsigned long long hash_value = 0;
    unsigned long long p = 1; // BASE^i

    // Calculate hash by iterating over each character of the string
    for (char c : str)
    {
        hash_value = (hash_value + (c - 'a' + 1) * p) % MOD;
        p = (p * BASE) % MOD; // Update power of BASE
    }

    return hash_value; // Return the computed hash
}

int main(int argc, char *argv[])
{
    // Check if file name is provided
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    // Indicate which file is being processed
    cout << "Processing file: " << argv[1] << endl;

    // Open the file
    ifstream inputFile(argv[1]);

    // Check if file is open
    if (!inputFile.is_open())
    {
        cerr << "Error: Could not open file " << argv[1] << "\n";
        return 1;
    }

    // Declare a set to store unique hashes
    unordered_set<unsigned long long> hashSet;

    // Read each string from the file
    string currentString;
    while (inputFile >> currentString)
    {
        // Compute the hash of the current string
        unsigned long long hashValue = computeHash(currentString);

        // Check if the hash already exists in the set (duplicate string detected)
        if (hashSet.find(hashValue) != hashSet.end())
        {
            cout << "Strings are not distinct\n";
            return 0;
        }

        // Insert the hash into the set
        hashSet.insert(hashValue);
    }

    // If we reach here, all strings are distinct
    cout << "All strings are distinct\n";

    // Close the file
    inputFile.close();

    return 0;
}
