const int MOBIUSMAX = 1e5 + 1;
ll mobius[MOBIUSMAX];

void calcmobius() {
    mobius[1] = -1;
    for (int i = 1; i < MOBIUSMAX; i++) {
        if (mobius[i]) {
            mobius[i] = -mobius[i];
            for (int j = 2 * i; j < MOBIUSMAX; j += i) { mobius[j] += mobius[i]; }
        }
    }
}
