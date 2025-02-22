template <class T, int V>
class HeavyLight {
    int parent[V], heavy[V], depth[V];
    int root[V], treePos[V], subtree[V];
    Segtree<T> tree;

    template <class G>
    int dfs(const G& graph, int v) {
        int size = 1, maxSubtree = 0;
        for (int u : graph[v]) if (u != parent[v]) {
            parent[u] = v;
            depth[u] = depth[v] + 1;
            int subtree = dfs(graph, u);
            if (subtree > maxSubtree) heavy[v] = u, maxSubtree = subtree;
            size += subtree;
        }
        subtree[v] = size;
        return size;
    }

    template <class G>
    void dfs2(const G& graph, int v, int& i) {
        treePos[v] = i++;
        if (heavy[v] != -1) dfs2(graph, heavy[v], i);
        for (int u : graph[v]) if (u != parent[v] && u != heavy[v]) dfs2(graph, u, i);
    }

    template <class BinaryOperation>
    void processPath(int u, int v, BinaryOperation op) {
        for (; root[u] != root[v]; v = parent[root[v]]) {
            if (depth[root[u]] > depth[root[v]]) swap(u, v);
            op(treePos[root[v]], treePos[v] + 1);
        }
        if (depth[u] > depth[v]) swap(u, v); op(treePos[u], treePos[v] + 1);
    }

public:
    template <class G>
    // Pass in adjacency list vector<vector<int>>
    HeavyLight(const G& graph) : tree(graph.size()) {
        int n = graph.size();
        fill_n(heavy, n, -1);
        parent[0] = -1;
        depth[0] = 0;
        dfs(graph, 0);
        for (int i = 0, currentPos = 0; i < n; ++i)
            if (parent[i] == -1 || heavy[parent[i]] != i)
                for (int j = i; j != -1; j = heavy[j])
                    root[j] = i;
        
        int k = 0;
        dfs2(graph, 0, k);
        }

    void set(int v, const T& value) {
        tree.modify(treePos[v], value);
    }

    // Need lazy segtree for this one
    void modifyPath(int u, int v, const T& value) {
        processPath(u, v, [this, &value](int l, int r) { tree.modify(l, r, value); });
    }

    T queryPath(int u, int v) {
        T res = T();
        processPath(u, v, [this, &res](int l, int r) { 
            // Combine
            res = combine(res, tree.query(l, r));
        });
        return res;
    }

    void modifySubtree(int v, const T& value) {
        tree.modify(treePos[v], treePos[v] + subtree[v], value);
    }

    void querySubtree(int v, const T& value) {
        return tree.query(treePos[v], treePos[v] + subtree[v]);
    }
};
