vector<int> root(MAXN, -1);

int find(int i){
	if(root[i] < 0) return i;
	return root[i] = find(root[i]);
}

void merge(int a, int b){
	if((a = find(a)) == (b = find(b))) return;
	if(root[a] < root[b]) root[a] += root[b], root[b] = a;
	else root[b] += root[a], root[a] = b;
}
