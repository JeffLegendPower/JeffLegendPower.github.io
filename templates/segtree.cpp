class Segtree {
private:
    int N;  // array size
    vector<ll> t;

    ll oper(ll a, ll b) {
        return a + b;
    }

public:
    Segtree(int size) : N(size), t(2 * size, 0ll) {}

    void build() {  // Build the tree
        for (int i = N - 1; i > 0; i--) 
            t[i] = oper(t[i<<1], t[i<<1|1]);
    }

    void modify(int p, ll value) {  // Set value at position p
        for (t[p += N] = value; p > 1; p >>= 1) 
            t[p >> 1] = oper(t[p], t[p^1]);
    }

    ll query(int l, int r) {  // Query on interval [l, r)
        ll res = 0;
        for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
            if (l&1) res = oper(res, t[l++]);
            if (r&1) res = oper(res, t[--r]);
        }
        return res;
    }
};
