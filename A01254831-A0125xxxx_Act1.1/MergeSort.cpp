#include <iostream>
#include <vector>

using namespace std;

// This function merges two parts of the array.
// We're merging the section from `left` to `mid` with the section from `mid + 1` to `right`.
// The goal here is to combine them in a way that keeps everything in descending order.
void merge(vector<double> &arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Temporary arrays to hold the split parts
    vector<double> L(n1);
    vector<double> R(n2);

    // Copy the data into the temp arrays
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    // Merge the two arrays back into the main array in the correct order
    while (i < n1 && j < n2)
    {
        if (L[i] >= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // If there are leftover elements in the left array, add them back in
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    // If there are leftover elements in the right array, add those too
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
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
