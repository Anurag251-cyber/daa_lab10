#include <stdio.h>
#include <stdlib.h>

/*
   Lexicographically smallest permutation using allowed swaps.
   Positions that are connected by swaps form a component, and inside a
   component the values can be arranged in ANY order. So the smallest
   values of a component must go to its smallest positions.
*/

int n, m;
int *parent, *size_;

int find(int x)
{
    while (parent[x] != x)
    {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

void unite(int a, int b)
{
    int ra = find(a), rb = find(b);
    if (ra == rb)
        return;
    if (size_[ra] < size_[rb])
    {
        int t = ra; ra = rb; rb = t;
    }
    parent[rb] = ra;
    size_[ra] += size_[rb];
}

int main()
{
    int i, u, v;
    int *A, *posOf, *start, *ptr, *grp, *res;

    fprintf(stderr, "Enter n: ");
    if (scanf("%d", &n) != 1 || n <= 0)
    {
        printf("n must be positive.\n");
        return 0;
    }

    A      = (int *)malloc((n + 2) * sizeof(int));
    posOf  = (int *)calloc(n + 2, sizeof(int));
    parent = (int *)malloc((n + 2) * sizeof(int));
    size_  = (int *)malloc((n + 2) * sizeof(int));
    start  = (int *)calloc(n + 2, sizeof(int));
    ptr    = (int *)calloc(n + 2, sizeof(int));
    grp    = (int *)malloc((n + 2) * sizeof(int));
    res    = (int *)malloc((n + 2) * sizeof(int));
    if (!A || !posOf || !parent || !size_ || !start || !ptr || !grp || !res)
    {
        printf("Memory allocation failed.\n");
        return 0;
    }

    fprintf(stderr, "Enter the permutation A[1..%d]: ", n);
    for (i = 1; i <= n; i++)
    {
        if (scanf("%d", &A[i]) != 1 || A[i] < 1 || A[i] > n || posOf[A[i]])
        {
            printf("A must be a permutation of 1..%d.\n", n);
            return 0;
        }
        posOf[A[i]] = i;
        parent[i] = i;
        size_[i] = 1;
    }

    fprintf(stderr, "Enter number of allowed swaps m: ");
    if (scanf("%d", &m) != 1 || m < 0)
    {
        printf("m must not be negative.\n");
        return 0;
    }

    fprintf(stderr, "Enter the %d swaps (u v), positions are 1..%d:\n", m, n);
    for (i = 1; i <= m; i++)
    {
        if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > n || v < 1 || v > n)
        {
            printf("Invalid swap.\n");
            return 0;
        }
        unite(u, v);
    }

    /* Group the positions of every component in increasing order (CSR layout):
       start[r] = first slot of root r inside grp[] */
    for (i = 1; i <= n; i++)
        start[find(i) + 1]++;
    for (i = 1; i <= n + 1; i++)
        start[i] += start[i - 1];
    for (i = 1; i <= n; i++)
        ptr[i] = start[i];
    for (i = 1; i <= n; i++)
    {
        int r = find(i);
        grp[ptr[r]++] = i;       /* positions arrive in increasing order */
    }
    for (i = 1; i <= n; i++)
        ptr[i] = start[i];

    /* Give values 1,2,3,... to the smallest free position of their component */
    for (v = 1; v <= n; v++)
    {
        int r = find(posOf[v]);
        res[grp[ptr[r]++]] = v;
    }

    printf("Lexicographically smallest permutation:\n");
    for (i = 1; i <= n; i++)
        printf("%d%c", res[i], i == n ? '\n' : ' ');

    return 0;
}
