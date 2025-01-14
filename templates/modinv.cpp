// REQUIRES binpow
ll modinv(ll x, ll MOD) {
    return binpow(x, MOD - 2, MOD);
}
