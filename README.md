# CP-Library

A collection of C++ header-only libraries for competitive programming, built as an extension of the [AtCoder Library (ACL)](https://github.com/atcoder/ac-library). The `expander.py` script is also adapted from ACL's expander.

While ACL provides foundational algorithms and data structures, this library adds implementations not covered by ACL (e.g., bipartite matching, HLPP max flow, segment tree beats, convex hull trick, geometry, etc.). Both ACL and custom headers can be included side by side — the expander handles `#include "atcoder/..."` and `#include "privates/..."` uniformly.

ACL is released under the [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/) license (public domain, no restrictions).

Each library is designed to be included via `#include "privates/<name>"` and expanded into a single submission file using `expander.py`.

## Repository Structure

```
library-codes/
├── expander.py          # Include expander for submission
├── privates/            # Library headers
│   ├── bimatching.hpp
│   ├── circulation.hpp
│   ├── convolution.hpp
│   ├── disjoint_set.hpp
│   ├── fastio.hpp
│   ├── fragmentedtree.hpp
│   ├── geometry.hpp
│   ├── linecontainer.hpp
│   ├── maxflow.hpp
│   ├── maxflow_hlpp.hpp
│   ├── maxflow_internal.hpp
│   ├── mcmf.hpp
│   ├── ordered_set.hpp
│   ├── real_rand.hpp
│   ├── segbeats.hpp
│   ├── segtree.hpp
│   ├── splay.hpp
│   └── splay
└── .gitignore
```

## expander.py

`expander.py` resolves `#include "privates/..."` (and `#include "atcoder/..."`) directives and inlines the library source into a single `combined.cpp` file ready for submission.

It automatically:
- Recursively expands nested includes
- Strips include guards, `#pragma once`, `#include <bits/stdc++.h>`, and `using namespace std;` from library headers to avoid duplication
- Deduplicates already-included files

### Usage

```bash
# Basic usage — outputs combined.cpp in the current directory
CPLUS_INCLUDE_PATH=/path/to/library-codes python3 /path/to/library-codes/expander.py solution.cpp

# Print to console instead of writing a file
CPLUS_INCLUDE_PATH=/path/to/library-codes python3 /path/to/library-codes/expander.py solution.cpp -c

# Specify library path directly
python3 /path/to/library-codes/expander.py solution.cpp --lib /path/to/library-codes
```

### VSCode Integration

You can set up a VSCode task to expand and prepare a submission file with a single shortcut. Below is an example `tasks.json` entry:

```json
{
    "label": "competitiveSubmit",
    "type": "shell",
    "command": "cd ${fileDirname} && CPLUS_INCLUDE_PATH=/path/to/library-codes python3 /path/to/library-codes/expander.py ${fileBasename}",
    "presentation": {
        "reveal": "always",
        "focus": true,
        "panel": "shared"
    },
    "problemMatcher": []
}
```

Running this task on the active file will generate `combined.cpp` in the same directory, ready to copy-paste or submit.

## Libraries

### Graph

| Library | Header | Description |
|---------|--------|-------------|
| **Bipartite Matching** | `bimatching.hpp` | Hopcroft-Karp algorithm for maximum bipartite matching. Constructs with `Bimatching(n, m)` where `n` and `m` are the sizes of each partition. Use `add_edge(u, v)`, `matching()`, and `result()`. |
| **Max Flow (Dinic)** | `maxflow.hpp` | Dinic's algorithm with capacity scaling. Inherits from `maxflow_internal.hpp`. Use `add_edge(u, v, c)` and `flow(s, t)`. Also provides `mincut(s)`. |
| **Max Flow (HLPP)** | `maxflow_hlpp.hpp` | Highest Label Preflow Push algorithm for maximum flow. Faster than Dinic on dense graphs. Same interface as the Dinic variant. **Note:** Only include one of `maxflow.hpp` or `maxflow_hlpp.hpp` per solution — both define `struct Maxflow`. |
| **Max Flow Internal** | `maxflow_internal.hpp` | Base class shared by both max flow implementations. Provides `add_edge(u, v, c)` and `mincut(s)`. |
| **Circulation** | `circulation.hpp` | Solves flow problems with lower bounds on edges. Wraps `Maxflow` internally. Use `add_edge(u, v, lower, upper)` and `solve(s, t)`. |
| **MCMF** | `mcmf.hpp` | Min-Cost Max-Flow using SPFA (Shortest Path Faster Algorithm). Use `add_edge(u, v, capacity, cost)` and `flow(s, t)` which returns `{total_flow, total_cost}`. |

### Data Structures

| Library | Header | Description |
|---------|--------|-------------|
| **Segment Tree** | `segtree.hpp` | Lazy propagation segment tree. Template parameters: node type `T`, lazy type `L`, merge `op`, apply `mapping`, compose `push`, and identity values `tid`/`lid`. Supports point/range update, point/range query, and `max_right`/`min_left` binary search on the tree. |
| **Segment Tree Beats** | `segbeats.hpp` | Segment tree with "beats" (conditional updates via `try_apply`). Same interface as `segtree.hpp` but `try_apply` returns whether the lazy was successfully applied — if it fails, the node is broken down further. Useful for problems like range chmin/chmax with sum queries. |
| **Disjoint Set Union** | `disjoint_set.hpp` | Union-Find with path compression. 0-indexed. Use `djs(n)`, `gp(u)` (find), and `un(u, v)` (union, returns `true` if newly merged). |
| **Splay Tree** | `splay.hpp` | Implicit-key splay tree with lazy propagation. Template parameters: node type `T`, lazy type `L`, merge `op`, apply `mapping`, compose `push`, and identity values `tid`/`lid`. Supports insert, erase, split, merge, reverse, shift, range update/query, and `max_right`/`min_left` binary search. |
| **Ordered Set** | `ordered_set.hpp` | Order-statistics tree backed by GNU `pb_ds`. Supports `find_by_order(k)` and `order_of_key(x)` in O(log n). |
| **Fragmented Tree** | `fragmentedtree.hpp` | Sqrt-decomposition on trees. Decomposes a tree into O(n/sqrt(n)) fragments of size O(sqrt(n)) each. Provides a framework where you implement per-fragment `get()` and `init()` for custom operations. |
| **Line Container** | `linecontainer.hpp` | Convex Hull Trick (from KACTL). Maintains a set of lines `y = kx + m` and answers maximum queries. Use `add(k, m)` and `query(x)`. |

### Math / Algebra

| Library | Header | Description |
|---------|--------|-------------|
| **Convolution** | `convolution.hpp` | FFT-based polynomial multiplication. `convolution(v, w)` for standard multiplication, `convolution_large(v, w, mod)` for modular multiplication of large values (splits coefficients to avoid precision loss). |

### Geometry

| Library | Header | Description |
|---------|--------|-------------|
| **Geometry** | `geometry.hpp` | 2D geometry primitives using `std::complex`. Provides `dot`, `cross`, `ccw` (counter-clockwise test), `angle_cmp` (angle comparator), and `convex_hull`. |

### Utility

| Library | Header | Description |
|---------|--------|-------------|
| **Fast I/O** | `fastio.hpp` | Buffered stdin reader for fast integer input. Call `_readInt()` to read the next integer. |
| **Random** | `real_rand.hpp` | Time-seeded Mersenne Twister (`mt19937`). Use with `uniform_int_distribution` or `uniform_real_distribution`. |
