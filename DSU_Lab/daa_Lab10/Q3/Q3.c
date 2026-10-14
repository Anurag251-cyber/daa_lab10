#include <stdio.h>
#include <stdlib.h>

/*
   Maximum weight component with size <= K, edges arriving online.

   DSU keeps for every root: size_[] and sum[] (total vertex weight).
   A max-heap with LAZY DELETION keeps candidate (sum, root) pairs.
   A heap entry is valid only if
        root is still a DSU root, sum[root] == entry.w, size_[root] <= K.
*/

typedef long long ll;

typedef struct { ll w; int root; } Entry;

int V, K, E;
int *parent, *size_;
ll *sum, *weight;

Entry *heap;
int heapSize = 0;

/* ---------- max-heap on Entry.w ---------- */
void heapPush(ll w, int root)
{
    int i = heapSize++;
    while (i > 0 && heap[(i - 1) / 2].w < w)
    {
        heap[i] = heap[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap[i].w = w;
    heap[i].root = root;
}

void heapPop()
{
    Entry last = heap[--heapSize];
    int i = 0, child;
    while ((child = 2 * i + 1) < heapSize)
    {
        if (child + 1 < heapSize && heap[child + 1].w > heap[child].w)
            child++;
        if (heap[child].w <= last.w)
            break;
        heap[i] = heap[child];
        i = child;
    }
    if (heapSize > 0)
        heap[i] = last;
}

/* ---------- DSU ---------- */
int find(int x)
{
    while (parent[x] != x)
    {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

int isValid(Entry e)
{
    return parent[e.root] == e.root && sum[e.root] == e.w
           && size_[e.root] <= K;
}

void addEdge(int u, int v)
{
    int ru = find(u), rv = find(v);
    if (ru == rv)
        return;
    if (size_[ru] < size_[rv])
    {
        int t = ru; ru = rv; rv = t;
    }
    parent[rv] = ru;
    size_[ru] += size_[rv];
    sum[ru] += sum[rv];
    if (size_[ru] <= K)          /* only a valid component is a candidate */
        heapPush(sum[ru], ru);
}

/* Prints the current answer; removes stale entries from the heap top */
void report(const char *label, int idx)
{
    while (heapSize > 0 && !isValid(heap[0]))
        heapPop();

    if (heapSize == 0)
        printf("%s %d: no component has size <= K\n", label, idx);
    else
        printf("%s %d: maximum weight = %lld (component size %d)\n",
               label, idx, heap[0].w, size_[heap[0].root]);
}

int main()
{
    int i, u, v;
    ll w;

    fprintf(stderr, "Enter V, K and E: ");
    if (scanf("%d %d %d", &V, &K, &E) != 3 || V <= 0 || K <= 0 || E < 0)
    {
        printf("V and K must be positive and E must not be negative.\n");
        return 0;
    }

    parent = (int *)malloc((V + 1) * sizeof(int));
    size_  = (int *)malloc((V + 1) * sizeof(int));
    sum    = (ll *)malloc((V + 1) * sizeof(ll));
    weight = (ll *)malloc((V + 1) * sizeof(ll));
    /* V initial entries + at most V-1 successful unions -> fewer than 2V entries */
    heap   = (Entry *)malloc((size_t)(2 * V + 2) * sizeof(Entry));
    if (!parent || !size_ || !sum || !weight || !heap)
    {
        printf("Memory allocation failed.\n");
        return 0;
    }

    fprintf(stderr, "Enter the %d vertex weights: ", V);
    for (i = 1; i <= V; i++)
    {
        if (scanf("%lld", &weight[i]) != 1)
        {
            printf("Invalid input.\n");
            return 0;
        }
        parent[i] = i;
        size_[i] = 1;
        sum[i] = weight[i];
        heapPush(sum[i], i);     /* every singleton is valid since K >= 1 */
    }

    report("Initially, before edge", 0);

    fprintf(stderr, "Enter the %d edges (u v edgeWeight):\n", E);
    for (i = 1; i <= E; i++)
    {
        fprintf(stderr, "Edge %d: ", i);
        if (scanf("%d %d %lld", &u, &v, &w) != 3 ||
            u < 1 || u > V || v < 1 || v > V)
        {
            printf("Invalid edge.\n");
            return 0;
        }
        /* The edge weight w does not change vertex weights, so it is not used. */
        addEdge(u, v);
        report("After edge", i);
    }

    free(parent); free(size_); free(sum); free(weight); free(heap);
    return 0;
}
