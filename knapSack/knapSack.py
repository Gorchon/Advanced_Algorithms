# Naive recursive Solution Code 
# Naive recursive Solution Code

def knapSack(n, c, w, v):
    # Base Case
    if n == 0 or c == 0:
        return 0
    # If weight of the nth item is more than the capacity, skip this item
    elif w[n-1] > c:
        return knapSack(n-1, c, w, v)
    else:
        # Compare: skipping the item vs taking the item
        tmp1 = knapSack(n-1, c, w, v)
        tmp2 = v[n-1] + knapSack(n-1, c-w[n-1], w, v)
        return max(tmp1, tmp2)
