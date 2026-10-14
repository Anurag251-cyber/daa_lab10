#include <stdio.h>
#include <stdlib.h>

/*
   Reverse time-step DSU.
   Deleting vertices p1..pV one by one is hard for DSU (it cannot split sets),
   so we run time backwards: start with an empty graph and ADD
   pV, pV-1, ..., p1 and record the best component sum before each addition.
*/

typedef long long ll;

int V, E;
int *head, *nxt, *to;          /* adjacency list stored in arrays */
int *parent, *size_, *active;
ll *weight, *sum, *ans;
int *P;

int find(int x)
{
    while (parent[x] != x)
    {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

void addEdgeToList(int a, int b, int *cnt)
{
    to[*cnt] = b;
    nxt[*cnt] = head[a];
    head[a] = (*cnt)++;
}

int main()
{
    int i, a, b, cnt = 0, e;
    ll best = 0;

    fprintf(stderr, "Enter number of vertices V and edges E: ");
    if (scanf("%d %d", &V, &E) != 2 || V <= 0 || E < 0)
    {
        printf("V must be positive and E must not be negative.\n");
        return 0;
    }

    head   = (int *)malloc((V + 1) * sizeof(int));
    nxt    = (int *)malloc((size_t)(2 * E + 1) * sizeof(int));
    to     = (int *)malloc((size_t)(2 * E + 1) * sizeof(int));
    parent = (int *)malloc((V + 1) * sizeof(int));
    size_  = (int *)malloc((V + 1) * sizeof(int));
    active = (int *)calloc(V + 1, sizeof(int));
    weight = (ll *)malloc((V + 1) * sizeof(ll));
    sum    = (ll *)malloc((V + 1) * sizeof(ll));
    ans    = (ll *)malloc((V + 2) * sizeof(ll));
    P      = (int *)malloc((V + 1) * sizeof(int));
    if (!head || !nxt || !to || !parent || !size_ || !active ||
        !weight || !sum || !ans || !P)
    {
        printf("Memory allocation failed.\n");
        return 0;
    }
    for (i = 1; i <= V; i++)
        head[i] = -1;

    fprintf(stderr, "Enter the %d vertex values (non-negative): ", V);
    for (i = 1; i <= V; i++)
    {
        if (scanf("%lld", &weight[i]) != 1 || weight[i] < 0)
        {
            printf("Vertex values must be non-negative integers.\n");
            return 0;
        }
    }

    fprintf(stderr, "Enter the %d edges (u v):\n", E);
    for (i = 1; i <= E; i++)
    {
        if (scanf("%d %d", &a, &b) != 2 || a < 1 || a > V || b < 1 || b > V)
        {
            printf("Invalid edge.\n");
            return 0;
        }
        addEdgeToList(a, b, &cnt);
        addEdgeToList(b, a, &cnt);
    }

    fprintf(stderr, "Enter the deletion order P (a permutation of 1..%d): ", V);
    for (i = 1; i <= V; i++)
    {
        if (scanf("%d", &P[i]) != 1 || P[i] < 1 || P[i] > V || active[P[i]])
        {
            printf("P must be a permutation of 1..%d.\n", V);
            return 0;
        }
        active[P[i]] = 1;        /* temporarily used as a "seen" flag */
    }
    for (i = 1; i <= V; i++)
        active[i] = 0;

    /* ---- reverse process ---- */
    for (i = V; i >= 0; i--)
    {
        ans[i] = best;           /* best sum after deleting p1..pi */
        if (i == 0)
            break;

        a = P[i];                /* re-insert vertex p_i */
        active[a] = 1;
        parent[a] = a;
        size_[a] = 1;
        sum[a] = weight[a];
        if (sum[a] > best)
            best = sum[a];

        for (e = head[a]; e != -1; e = nxt[e])
        {
            int u = to[e];
            int ra, rb;
            if (!active[u])
                continue;
            ra = find(a);
            rb = find(u);
            if (ra == rb)
                continue;
            if (size_[ra] < size_[rb])
            {
                int t = ra; ra = rb; rb = t;
            }
            parent[rb] = ra;
            size_[ra] += size_[rb];
            sum[ra] += sum[rb];
            if (sum[ra] > best)
                best = sum[ra];
        }
    }

    printf("Before any deletion: maximum component sum = %lld\n", ans[0]);
    for (i = 1; i <= V; i++)
        printf("After deleting vertex %d: maximum component sum = %lld\n",
               P[i], ans[i]);

    return 0;
}
