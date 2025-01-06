int jmp[20][MAXN], dep[MAXN];

void addchild(int parent, int child){ // add child to parent
	jmp[0][child] = parent;
	dep[child] = dep[parent]+1;
}

void build(){ // prepare jumps
	for(int i=0; i<20; ++i)
		for(int j=0; j<MAXN; ++j)
			jmp[i][j] = jmp[i-1][jmp[i-1][j]];
}

int kth(int i, int k){ // kth ancestor of i
	for(int x=19; x>=0; --x)
		if(k & (1<<x)) i = jmp[x][i];
	return i;
}

int lca(int a, int b){ // LCA of a, b
	if(dep[a] < dep[b]) std::swap(a, b);
	a = kth(a, dep[a] - dep[b]);
	if(a == b) return a;
	for(int x=19; x>=0; --x)
		if(jmp[x][a] != jmp[x][b])
			a = jmp[x][a], b = jmp[x][b];
	return jmp[0][a];
}
