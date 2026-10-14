# Disjoint Set Union (Union-Find) and Connected Components — DAA Lab

Five problems on the Union-Find data structure and connected components of a graph. Each folder contains the C program, a full README (problem, approaches, algorithm, dry run, complexity analysis, viva questions) and a running-time graph.

| # | Problem | Time complexity | Space |
|---|---|---|---|
| Q1 | [Online Graph Bipartiteness Testing using DSU with Parity](Q1/README.md) | `O(V + E α(V))` | `O(V)` |
| Q2 | [Dynamic Connected Components in a Grid (Number of Islands II)](Q2/README.md) | `O(R·C + k α(R·C))` | `O(R·C)` |
| Q3 | [Maximum Weight Component with Size Constraint](Q3/README.md) | `O(E α(V) + V log V)` | `O(V)` |
| Q4 | [Reverse Time-Step DSU: Maximum Component Sum under Vertex Deletions](Q4/README.md) | `O(V + E α(V))` | `O(V + E)` |
| Q5 | [Lexicographically Smallest Permutation using Allowed Swaps](Q5/README.md) | `O(n + m α(n))` | `O(n)` |

`α` is the inverse Ackermann function (at most 4 for any practical input size).

## Compile and run

```bash
cd Q1
gcc Q1.c -o Q1
./Q1 < input.txt
```

Every program prints its prompts on `stderr` and its results on `stdout`.

## Files

```text
Q1/ Q1.c  README.md  Q1_complexity.png
Q2/ Q2.c  README.md  Q2_complexity.png
Q3/ Q3.c  README.md  Q3_complexity.png
Q4/ Q4.c  README.md  Q4_complexity.png
Q5/ Q5.c  README.md  Q5_complexity.png
```
