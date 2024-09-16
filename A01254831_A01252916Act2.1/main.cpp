// Objective: Check if all strings in a file are distinct using string hashing.
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set> // Using unordered_set to store hashed values

using namespace std;

/*
 * To run this program, you need to first navigate to the directory that is "A01254831_A01252916Act2.1".
 * Once in the correct directory, you can compile the code using the following command:
 *
 *      g++ -o main main.cpp
 *
 * This will compile your program and create an executable named "main".
 *
 * After compiling, you can run the program with four different input test cases, each designed to challenge
 * different aspects of the code, using the following commands:
 *
 *      ./main in1.txt
 *      ./main in2.txt
 *      ./main in3.txt
 *      ./main in4.txt
 *
 * Each of these test cases contains unique challenges to test the robustness of the "Hash String" algorithm.
 */

// Hashing Strategy Explanation:
// We're using a basic polynomial rolling hash function to convert each string into a unique numeric value.
// This strategy is efficient for strings of varying lengths, and it avoids many common hashing issues,
// such as collisions. Each character in the string is treated as a base-256 number (since there are 256
// possible ASCII characters), and the hash is computed by accumulating powers of a small prime number (31).
// Using a large prime modulus (1e9 + 7) ensures that we avoid overflow and minimize the chances of collisions,
// even when dealing with long strings.

// Time and Space Complexity:
// The time complexity of this solution is O(N * L), where N is the number of strings in the input file,
// and L is the average length of each string. This is because we compute the hash of each string by iterating
// through its characters, and we perform a constant-time operation to check for duplicates using the hash set.
//
// The space complexity is O(N) since we store the hash of each string in an unordered_set. This ensures that
// we only keep track of the unique hash values as we process the strings. The space needed for the hash set
// grows proportionally to the number of distinct strings.

// Large prime number to avoid collisions due to overflow
const int primeModulus = 1e9 + 7;

// Base for the polynomial hash function
const int polynomialBase = 31;

// This function computes a hash value for a given string
unsigned long long computeStringHash(const string &inputString)
{
    // Initialize the hash value and the multiplier for powers of the base
    unsigned long long hashValue = 0;
    unsigned long long basePower = 1; // Represents polynomialBase^i

    // Calculate hash by iterating over each character of the string
    for (char currentChar : inputString)
    {
        // Accumulate hash value by adding the character's contribution
        hashValue = (hashValue + (currentChar - 'a' + 1) * basePower) % primeModulus;
        // Update the base power (polynomialBase^i) for the next character
        basePower = (basePower * polynomialBase) % primeModulus;
    }

    return hashValue; // Return the computed hash value
}

int main(int argumentCount, char *argumentValues[])
{
    // Check if file name is provided
    if (argumentCount != 2)
    {
        cerr << "Usage: " << argumentValues[0] << " <input_file>\n";
        return 1;
    }

    // Indicate which file is being processed
    cout << "Processing file: " << argumentValues[1] << endl;

    // Open the file
    ifstream inputFile(argumentValues[1]);

    // Check if file is open
    if (!inputFile.is_open())
    {
        cerr << "Error: Could not open file " << argumentValues[1] << "\n";
        return 1;
    }

    // Declare a set to store unique hashes
    unordered_set<unsigned long long> hashSet;

    // Read each string from the file
    string currentString;
    while (inputFile >> currentString)
    {
        // Compute the hash of the current string
        unsigned long long hashValue = computeStringHash(currentString);

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
