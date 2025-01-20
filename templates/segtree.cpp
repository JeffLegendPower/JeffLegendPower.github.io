template <class T>
class Segtree {
private:
    int N;  // array size
    vector<T> t;

    T combine(T a, T b) {
        return a + b;
    }

public:
    Segtree(int size) : N(size), t(2 * size, T()) {}

    void build() {  // Build the tree
        for (int i = N - 1; i > 0; i--) 
            t[i] = combine(t[i<<1], t[i<<1|1]);
    }

    void modify(int p, T value) {  // Set value at position p
        for (t[p += N] = value; p > 1; p >>= 1) 
            t[p >> 1] = combine(t[p], t[p^1]);
    }

    T query(int l, int r) {  // Query on interval [l, r)
        T res = T();
        for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
            if (l&1) res = combine(res, t[l++]);
            if (r&1) res = combine(res, t[--r]);
        }
        return res;
    }
};
