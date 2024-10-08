# Initialize a memoization array with 'None' values
def initialize_memo(n, C):
    return [[None for _ in range(C + 1)] for _ in range(n + 1)]

def knapSack(n, C, w, v, arr): 
    
    ## notation is 
    ## n = number of items
    ## C = capacity of the knapsack
    ## w = weights of the items
    ## v = values of the items
    ## arr = memoization table
    
    # Check if the solution has already been computed
    if arr[n][C] is not None:
        return arr[n][C]
    
    # Base case: no items left or no remaining capacity
    if n == 0 or C == 0:
        result = 0
    # If the current item's weight exceeds the remaining capacity, skip the item
    elif w[n-1] > C:
        result = knapSack(n-1, C, w, v, arr)
    else:
        # Compute the two possibilities:
        # 1. Exclude the current item
        tmp1 = knapSack(n-1, C, w, v, arr)
        # 2. Include the current item
        tmp2 = v[n-1] + knapSack(n-1, C-w[n-1], w, v, arr) # here we check the option where we include the value
        result = max(tmp1, tmp2)
    
    # Store the computed result in the memoization table
    arr[n][C] = result
    return result

# Example usage:
n = 4  # Number of items
C = 10  # Capacity of the knapsack
w = [5, 4, 6, 3]  # Weights of the items
v = [10, 40, 30, 50]  # Values of the items

# Initialize the memoization table
arr = initialize_memo(n, C)

# Compute the maximum value that can be obtained
max_value = knapSack(n, C, w, v, arr)
print(f"Maximum value: {max_value}")
