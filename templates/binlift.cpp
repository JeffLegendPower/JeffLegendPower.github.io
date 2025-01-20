int jmp[20][MAXN], dep[MAXN];

// add child to parent
void addchild(int p, int v) {
	jmp[0][v] = p;
	dep[v] = dep[p] + 1;
}

// prepare jumps
void build() {
	for (int i = 1; i < 20; i++) for(int j = 0; j < MAXN; j++) jmp[i][j] = jmp[i-1][jmp[i-1][j]];
}

// kth ancestor of i
int kth(int i, int k) {
	for(int x = 19; x >= 0; x--) if (k & (1<<x)) i = jmp[x][i];
	return i;
}

// LCA of a, b
int lca(int a, int b) {
	if(dep[a] < dep[b]) swap(a, b);
	a = kth(a, dep[a] - dep[b]);
	if(a == b) return a;
	for(int x = 19; x >= 0; x--) if(jmp[x][a] != jmp[x][b]) a = jmp[x][a], b = jmp[x][b];
	return jmp[0][a];
}
