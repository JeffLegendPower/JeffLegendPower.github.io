// https://cp-algorithms.com/graph/kuhn_maximum_bipartite_matching.html#implementation
// only has edges from one side of bipartite graph to other
vector<vector<int>> kuhngraph;
// the matching vertex for each vertex on the other side of the graph
// if matching vertex won't be -1
vector<int> mt;
// only for side 1
vector<bool> used;

bool try_kuhn(int v) {
    if (used[v])
        return false;
    used[v] = true;
    for (int to : g[v]) {
        if (mt[to] == -1 || try_kuhn(mt[to])) {
            mt[to] = v;
            return true;
        }
    }
    return false;
}

// if N vertices on left side and M on right side
mt.assign(M, -1);
for (int v = 0; v < N; v++) {
    used.assign(N, false);
    try_kuhn(v);
}

for (int i = 0; i < M; i++) if (mt[i] != -1) {/* i on right side matches with mt[i] on left side */}
