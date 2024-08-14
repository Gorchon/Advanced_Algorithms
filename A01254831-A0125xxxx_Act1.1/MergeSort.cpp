#include <iostream>
#include <vector>

using namespace std;

// to run this program please first compile it using the following command:
// g++ MergeSort.cpp -o MergeSort

// then run it using the following command:
// ./MergeSort < input.txt

// This function merges two parts of the array.
// We're merging the section from `left` to `mid` with the section from `mid + 1` to `right`.
// The goal here is to combine them in a way that keeps everything in descending order.
void merge(vector<double> &array, int leftIndex, int midIndex, int rightIndex)
{
    int leftSize = midIndex - leftIndex + 1;
    int rightSize = rightIndex - midIndex;

    // Temporary arrays to hold the split parts
    vector<double> leftArray(leftSize);
    vector<double> rightArray(rightSize);

    // Copy the data into the temporary arrays
    for (int i = 0; i < leftSize; i++)
        leftArray[i] = array[leftIndex + i];
    for (int j = 0; j < rightSize; j++)
        rightArray[j] = array[midIndex + 1 + j];

    int leftArrayIndex = 0, rightArrayIndex = 0;
    int mergedArrayIndex = leftIndex;

    // Merge the two arrays back into the main array in the correct order
    while (leftArrayIndex < leftSize && rightArrayIndex < rightSize)
    {
        // to decide the order of the elements we use <= if we want to sort in ascending order and >= if we want to sort in descending order
        if (leftArray[leftArrayIndex] <= rightArray[rightArrayIndex])

        {
            array[mergedArrayIndex] = leftArray[leftArrayIndex];
            leftArrayIndex++;
        }
        else
        {
            array[mergedArrayIndex] = rightArray[rightArrayIndex];
            rightArrayIndex++;
        }
        mergedArrayIndex++;
    }

    // If there are leftover elements in the left array, add them back in
    while (leftArrayIndex < leftSize)
    {
        array[mergedArrayIndex] = leftArray[leftArrayIndex];
        leftArrayIndex++;
        mergedArrayIndex++;
    }

    // If there are leftover elements in the right array, add those too
    while (rightArrayIndex < rightSize)
    {
        array[mergedArrayIndex] = rightArray[rightArrayIndex];
        rightArrayIndex++;
        mergedArrayIndex++;
    }
}

// This function does the actual MergeSort.
// It keeps splitting the array in half until we're working with single elements,
// and then it uses the `merge` function to combine them back together in the right order.
void mergeSort(vector<double> &arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        // Sort the first half
        mergeSort(arr, left, mid);
        // Sort the second half
        mergeSort(arr, mid + 1, right);

        // Now merge the two halves
        merge(arr, left, mid, right);
    }
}

// The main function is where everything starts.
// It reads in the number of elements and then the elements themselves.
// After that, it calls `mergeSort` to sort them in descending order and prints them out.
int main()
{
    int N;
    cin >> N;

    vector<double> arr(N);

    // Reading the input values
    for (int i = 0; i < N; i++)
    {
        cin >> arr[i];
    }

    // Sort the array using MergeSort
    mergeSort(arr, 0, N - 1);

    // Print the sorted array from largest to smallest
    for (const double &value : arr)
    {
        cout << value << endl;
    }

    return 0;
}

/*
 * Time Complexity Explanation:
 *
 * - The MergeSort algorithm works by dividing the array into two halves,
 *   sorting each half recursively, and then merging the sorted halves.
 *
 * - The "divide" step splits the array in half, which takes constant time O(1).
 * - The "conquer" step involves recursively sorting two halves, which takes O(log n) levels of recursion.
 * - The "merge" step, where we combine the two halves, takes linear time O(n) for each level of recursion.
 *
 * - Overall, the time complexity of MergeSort is O(n log n).
 *   This is because we have O(log n) levels of recursion, and at each level, we spend O(n) time to merge.
 *
 * - MergeSort is more efficient than simpler algorithms like Bubble Sort (O(n^2)),
 *   especially for large datasets, because of its logarithmic splitting.
 */
