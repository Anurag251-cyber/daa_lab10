#include <stdio.h>
#include <stdlib.h>

/*
   Online bipartiteness testing using DSU with parity.

   parent[x] : parent of x in the DSU forest
   parity[x] : colour difference between x and parent[x] (0 = same, 1 = different)
   rnk[x]    : rank (upper bound on height) used for union by rank
*/

int V, E;
int *parent, *parity, *rnk;
int components;

/* Find root of x. After the call, parent[x] is the root (or x is the root)
   and parity[x] is the colour difference between x and its root.
   Iterative, with path compression, so there is no recursion depth problem. */
int find(int x)
{
    int root = x, p = 0, cur, cp, next, np;

    /* pass 1: find the root and the parity of x relative to the root */
    while (parent[root] != root)
    {
        p ^= parity[root];
        root = parent[root];
    }

    /* pass 2: path compression, keeping every parity correct */
    cur = x;
    cp = p;
    while (parent[cur] != cur)
    {
        next = parent[cur];
        np = cp ^ parity[cur];   /* parity of next relative to the root */
        parent[cur] = root;
        parity[cur] = cp;
        cur = next;
        cp = np;
    }
    return root;
}

/* Returns 1 if edge (u,v) is accepted, 0 if it would create an odd cycle */
int addEdge(int u, int v)
{
    int ru, rv;

    if (u == v)                  /* self-loop = odd cycle of length 1 */
        return 0;

    ru = find(u);
    rv = find(v);

    if (ru == rv)
    {
        /* same component: u and v must have different colours */
        if (parity[u] == parity[v])
            return 0;            /* odd cycle would be formed */
        return 1;                /* even cycle, graph stays bipartite */
    }

    /* different components: merge them so that u and v get different colours */
    if (rnk[ru] < rnk[rv])
    {
        parent[ru] = rv;
        parity[ru] = parity[u] ^ parity[v] ^ 1;
    }
    else
    {
        parent[rv] = ru;
        parity[rv] = parity[u] ^ parity[v] ^ 1;
        if (rnk[ru] == rnk[rv])
            rnk[ru]++;
    }
    components--;
    return 1;
}

int main()
{
    int i, u, v, accepted = 0, rejected = 0;

    fprintf(stderr, "Enter number of vertices V: ");
    if (scanf("%d", &V) != 1 || V <= 0)
    {
        printf("Number of vertices must be positive.\n");
        return 0;
    }
    fprintf(stderr, "Enter number of edges E: ");
    if (scanf("%d", &E) != 1 || E < 0)
    {
        printf("Number of edges must not be negative.\n");
        return 0;
    }

    parent = (int *)malloc((V + 1) * sizeof(int));
    parity = (int *)calloc(V + 1, sizeof(int));
    rnk    = (int *)calloc(V + 1, sizeof(int));
    if (!parent || !parity || !rnk)
    {
        printf("Memory allocation failed.\n");
        return 0;
    }
    for (i = 1; i <= V; i++)
        parent[i] = i;
    components = V;

    fprintf(stderr, "Enter the %d edges (u v), vertices are 1..%d:\n", E, V);
    for (i = 1; i <= E; i++)
    {
        fprintf(stderr, "Edge %d: ", i);
        if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > V || v < 1 || v > V)
        {
            printf("Invalid edge. Vertices must be between 1 and %d.\n", V);
            return 0;
        }

        if (addEdge(u, v))
        {
            accepted++;
            printf("Edge %d (%d, %d): ACCEPTED\n", i, u, v);
        }
        else
        {
            rejected++;
            printf("Edge %d (%d, %d): REJECTED (odd cycle)\n", i, u, v);
        }
    }

    printf("\nAccepted edges = %d, rejected edges = %d\n", accepted, rejected);
    printf("Connected components = %d\n", components);
    printf("The accepted graph is bipartite.\n");

    /* colour of a vertex = parity relative to its root; a root gets colour 0 */
    printf("\nA valid 2-colouring:\n");
    for (i = 1; i <= V; i++)
    {
        find(i);
        printf("Vertex %d -> side %d\n", i, (i == parent[i]) ? 0 : parity[i]);
    }

    free(parent); free(parity); free(rnk);
    return 0;
}
