// Can replace with any other function as long as any f[i] intersects with any other f[j] at most once
struct line {
    ll a, b;

    ll calc(ll x) { return a*x + b; }
    ld intersect(line two) { return (ld) (two.b - b) / (a - two.a); };
};

deque<line> hull;
vector<int> ints(MAXN + 1);
// iota(ints.begin(), ints.end(), 0);

// Find which f to use for some value of x
auto cmp = [&hull](int idx, ll x) { return hull[idx].intersect(hull[idx + 1]) < x; };
// int idx = *lower_bound(ints.begin(), ints.begin() + hull.size() - 1, x, cmp);
