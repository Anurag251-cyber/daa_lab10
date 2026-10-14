#include <stdio.h>
#include <stdlib.h>

/*
   Dynamic connected components in a grid (Number of Islands II).
   Cell (r,c) is flattened to id = r*C + c.
   parent[id] = -1  -> the cell is water
   otherwise it is a land cell inside a DSU tree.
*/

int R, C, K;
int *parent, *sz;
int islands = 0;

int find(int x)
{
    while (parent[x] != x)
    {
        parent[x] = parent[parent[x]];   /* path halving */
        x = parent[x];
    }
    return x;
}

/* Merge the sets of a and b; returns 1 if they were different */
int unite(int a, int b)
{
    int ra = find(a), rb = find(b);
    if (ra == rb)
        return 0;
    if (sz[ra] < sz[rb])
    {
        int t = ra; ra = rb; rb = t;
    }
    parent[rb] = ra;
    sz[ra] += sz[rb];
    return 1;
}

/* Turn cell (r,c) into land. Returns 0 if it was already land. */
int addLand(int r, int c)
{
    int id = r * C + c, d;
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    if (parent[id] != -1)
        return 0;

    parent[id] = id;
    sz[id] = 1;
    islands++;                           /* a new island is born */

    for (d = 0; d < 4; d++)
    {
        int nr = r + dr[d], nc = c + dc[d];
        if (nr < 0 || nr >= R || nc < 0 || nc >= C)
            continue;
        if (parent[nr * C + nc] == -1)
            continue;                    /* neighbour is water */
        if (unite(id, nr * C + nc))
            islands--;                   /* two islands became one */
    }
    return 1;
}

int main()
{
    int i, r, c;

    fprintf(stderr, "Enter rows R, columns C and number of operations K: ");
    if (scanf("%d %d %d", &R, &C, &K) != 3 || R <= 0 || C <= 0 || K < 0)
    {
        printf("R and C must be positive and K must not be negative.\n");
        return 0;
    }

    parent = (int *)malloc((size_t)R * C * sizeof(int));
    sz     = (int *)calloc((size_t)R * C, sizeof(int));
    if (!parent || !sz)
    {
        printf("Memory allocation failed.\n");
        return 0;
    }
    for (i = 0; i < R * C; i++)
        parent[i] = -1;                  /* whole grid is water */

    fprintf(stderr, "Enter K positions (row col), 0-indexed:\n");
    for (i = 1; i <= K; i++)
    {
        fprintf(stderr, "Operation %d: ", i);
        if (scanf("%d %d", &r, &c) != 2)
        {
            printf("Invalid input.\n");
            return 0;
        }
        if (r < 0 || r >= R || c < 0 || c >= C)
        {
            printf("Operation %d (%d, %d): outside grid, ignored. "
                   "islands = %d\n", i, r, c, islands);
            continue;
        }
        if (!addLand(r, c))
            printf("Operation %d (%d, %d): already land. islands = %d\n",
                   i, r, c, islands);
        else
            printf("Operation %d (%d, %d): islands = %d\n", i, r, c, islands);
    }

    if (R <= 30 && C <= 30)
    {
        printf("\nFinal grid (# = land, . = water):\n");
        for (r = 0; r < R; r++)
        {
            for (c = 0; c < C; c++)
                putchar(parent[r * C + c] == -1 ? '.' : '#');
            putchar('\n');
        }
    }

    free(parent); free(sz);
    return 0;
}
