// Names: José María Soto Valenzuela, Student2 ID: Cesar Silva
// Program Description:
// This program implements the Simulated Annealing algorithm to solve the Traveling Salesman Problem (TSP).
// It reads input data from a file provided as a command-line argument, processes it, and outputs an optimal or near-optimal tour.
// The input must follow the specified format.
// The program can be compiled using g++ and run in a Linux environment.

/*
Test Cases:

Test Case 1: Small Dataset (5 cities)
Purpose: Ensure the algorithm works on a simple, manageable problem.
Input File: in1.txt

Test Case 2: Edge Case with Identical Coordinates
Purpose: Check for division by zero or unexpected behavior with overlapping cities.
Input File: in2.txt

Test Case 3: Medium Dataset (10 cities)
Purpose: Test the algorithm on a slightly larger dataset.
Input File: in3.txt

Test Case 4: Edge Case with Minimal Cities (2 cities)
Purpose: Ensure the algorithm handles the minimum number of cities.
Input File: in4.txt

Test Case 5: Large Dataset (52 cities)
Purpose: Evaluate the algorithm's ability to handle larger inputs.
Input File: in5.txt

How to Run:
Compile:
g++ -std=c++11 -o tsp main.cpp
Run:
./tsp in1.txt
./tsp in2.txt
./tsp in3.txt
./tsp in4.txt
./tsp in5.txt
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <random>
#include <limits>
#include <string>

using namespace std;

// Structure to represent a city with an ID and x, y coordinates
struct City
{
    int id;   // City identifier
    double x; // X-coordinate
    double y; // Y-coordinate
};

// Function to calculate the Euclidean distance between two cities
// Complexity: O(1)
double calculateDistance(const City &cityA, const City &cityB)
{
    double deltaX = cityA.x - cityB.x;
    double deltaY = cityA.y - cityB.y;
    return sqrt(deltaX * deltaX + deltaY * deltaY);
}

// Function to read the input data from a file
// Complexity: O(N^2), due to distance matrix calculation
void readInputData(istream &in, vector<City> &cities, vector<vector<double>> &distanceMatrix)
{
    int numberOfCities;
    in >> numberOfCities;

    cities.resize(numberOfCities);
    for (int index = 0; index < numberOfCities; ++index)
    {
        in >> cities[index].id >> cities[index].x >> cities[index].y;
    }

    // Precompute the distance matrix
    distanceMatrix.resize(numberOfCities, vector<double>(numberOfCities, 0.0));
    for (int i = 0; i < numberOfCities; ++i)
    {
        for (int j = i + 1; j < numberOfCities; ++j)
        {
            double distance = calculateDistance(cities[i], cities[j]);
            distanceMatrix[i][j] = distance;
            distanceMatrix[j][i] = distance; // Symmetric matrix
        }
    }
}

// Function to calculate the total length of a tour
// Complexity: O(N), where N is the number of cities
double calculateTotalTourDistance(const vector<int> &tour, const vector<vector<double>> &distanceMatrix)
{
    double totalDistance = 0.0;
    int numberOfCities = tour.size();
    for (int index = 0; index < numberOfCities - 1; ++index)
    {
        totalDistance += distanceMatrix[tour[index]][tour[index + 1]];
    }
    // Return to the starting city
    totalDistance += distanceMatrix[tour[numberOfCities - 1]][tour[0]];
    return totalDistance;
}

// Function to generate an initial random tour
// Complexity: O(N)
void generateInitialTour(vector<int> &tour, int numberOfCities, mt19937 &rng)
{
    tour.resize(numberOfCities);
    for (int index = 0; index < numberOfCities; ++index)
    {
        tour[index] = index;
    }
    shuffle(tour.begin(), tour.end(), rng);
}

// Function to generate a neighboring tour by reversing a segment
// Complexity: O(N)
void generateNeighborTour(const vector<int> &currentTour, vector<int> &neighborTour, mt19937 &rng)
{
    neighborTour = currentTour;
    int numberOfCities = currentTour.size();

    uniform_int_distribution<int> dist(0, numberOfCities - 1);
    int positionA = dist(rng);
    int positionB = dist(rng);
    while (positionB == positionA)
    {
        positionB = dist(rng);
    }

    // Ensure positionA < positionB
    if (positionA > positionB)
    {
        swap(positionA, positionB);
    }

    // Reverse the segment between positionA and positionB
    reverse(neighborTour.begin() + positionA, neighborTour.begin() + positionB + 1);
}

// Function to calculate the acceptance probability
// Complexity: O(1)
double calculateAcceptanceProbability(double currentDistance, double newDistance, double temperature)
{
    if (newDistance < currentDistance)
    {
        return 1.0;
    }
    return exp((currentDistance - newDistance) / temperature);
}

// Simulated Annealing algorithm to solve the TSP
// Complexity: Depends on the number of iterations
void performSimulatedAnnealing(
    const vector<City> &cities,
    const vector<vector<double>> &distanceMatrix,
    vector<int> &bestTour,
    double &bestDistance)
{
    int numberOfCities = cities.size();
    vector<int> currentTour;

    // Initialize random number generator
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 rng(seed);
    uniform_real_distribution<double> realDist(0.0, 1.0);

    generateInitialTour(currentTour, numberOfCities, rng);
    double currentDistance = calculateTotalTourDistance(currentTour, distanceMatrix);

    bestTour = currentTour;
    bestDistance = currentDistance;

    // Parameters for the algorithm
    double temperature = 10000.0;         // Initial temperature
    double coolingRate = 0.9995;          // Cooling rate (must be between 0 and 1)
    double absoluteTemperature = 0.00001; // Minimum temperature to stop the algorithm
    int iteration = 0;

    while (temperature > absoluteTemperature)
    {
        vector<int> neighborTour;
        generateNeighborTour(currentTour, neighborTour, rng);
        double neighborDistance = calculateTotalTourDistance(neighborTour, distanceMatrix);

        double acceptanceProb = calculateAcceptanceProbability(currentDistance, neighborDistance, temperature);

        // Decide whether to accept the neighbor tour
        if (acceptanceProb > realDist(rng))
        {
            currentTour = neighborTour;
            currentDistance = neighborDistance;
        }

        // Update the best tour found so far
        if (currentDistance < bestDistance)
        {
            bestTour = currentTour;
            bestDistance = currentDistance;
        }

        // Cool down the system
        temperature *= coolingRate;
        iteration++;

        // Optionally print progress every 10000 iterations
        if (iteration % 10000 == 0)
        {
            cout << "Iteration: " << iteration << ", Best Distance: " << bestDistance << endl;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: ./tsp input_file" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile)
    {
        cerr << "Error opening input file: " << argv[1] << endl;
        return 1;
    }

    vector<City> cities;
    vector<vector<double>> distanceMatrix;
    readInputData(inputFile, cities, distanceMatrix);

    vector<int> bestTour;
    double bestDistance;

    performSimulatedAnnealing(cities, distanceMatrix, bestTour, bestDistance);

    // Output the best tour found
    cout << "Best tour length: " << bestDistance << endl;
    cout << "Tour:" << endl;
    for (int index = 0; index < bestTour.size(); ++index)
    {
        cout << cities[bestTour[index]].id << " ";
    }
    cout << endl;

    return 0;
}
