template <class T, class K>
class LazySegtree {
private:
    int N;  // array size
    int h;
    vector<T> t;
    vector<K> d;

    T combine(T a, T b) {
        return a + b;
    }

    // k is the length of the segment
    T calc(T a, T b, K d, int k) {
        return max(a, b) + d;
    }

    // Update d[p]
    // k is the length of segment
    void apply(int p, K value, int k) {
        // Update t[p] as if it was already affected by d[p]
        t[p] += value;
        if (p < N) d[p] += value;
    }

public:
    LazySegtree(int size) : N(size), h(sizeof(int) * 8 - __builtin_clz(N)), t(2 * size, T()), d(size) {}

    void build(int l, int r) {
        int k = 2;
        for (l += N, r += N-1; l > 1; k <<= 1) {
            l >>= 1, r >>= 1;
            for (int i = r; i >= l; --i) t[i] = calc(t[i<<1], t[i<<1|1], d[i], k);
        }
    }

    void push(int l, int r) {
        int s = h, k = 1 << (h-1);
        for (l += N, r += N-1; s > 0; --s, k >>= 1) for (int i = l >> s; i <= r >> s; ++i) if (d[i] != 0) {
            apply(i<<1, d[i], k);
            apply(i<<1|1, d[i], k);
            d[i] = 0;
        }
    }

    void modify(int l, int r, K value) {
        if (value == 0) return;
        push(l, l + 1);
        push(r - 1, r);
        int l0 = l, r0 = r, k = 1;
        for (l += N, r += N; l < r; l >>= 1, r >>= 1, k <<= 1) {
            if (l&1) apply(l++, value, k);
            if (r&1) apply(--r, value, k);
        }
        build(l0, l0 + 1);
        build(r0 - 1, r0);
    }

    T query(int l, int r) {
        push(l, l + 1);
        push(r - 1, r);
        T res = 0;
        for (l += N, r += N; l < r; l >>= 1, r >>= 1) {
            if (l&1) res = calc(res, t[l++], 0, -1);
            if (r&1) res = calc(res, t[--r], 0, -1);
        }
        return res;
    }
};
