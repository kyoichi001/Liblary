
#include <vector>

//UnionFind木
//参照 https://qiita.com/ofutonfuton/items/c17dfd33fc542c222396
//
//
class UnionFind {
	std::vector<int> parent;
public:

	UnionFind(int N) : parent(N) { //最初は全てが根であるとして初期化
		for (int i = 0; i < N; i++) parent[i] = i;
	}

	//初期化
	void initialize(int N) {
		parent = std::vector<int>(N);
		for (int i = 0; i < N; i++) parent[i] = i;
	}
	// データxが属する木の根を再帰で得る：root(x) = {xの木の根}
	int root(int x) {
		if (parent[x] == x) return x;
		parent[x] = root(parent[x]);
		return parent[x];
	}
	// xとyの木を併合
	void unite(int x, int y) {
		int rx = root(x); //xの根をrx
		int ry = root(y); //yの根をry
		if (rx == ry) return; //xとyの根が同じ(=同じ木にある)時はそのまま
		parent[rx] = ry; //xとyの根が同じでない(=同じ木にない)時：xの根rxをyの根ryにつける
	}
	// 2つのデータx, yが属する木が同じならtrueを返す
	bool isSame(int x, int y) {
		int rx = root(x);
		int ry = root(y);
		return rx == ry;
	}
	//属する各木の要素数をリストで返す
	//O(N)
	vector<int> GetCounts() {
		const int N = parent.size();
		vector<int> res(N,0);
		for (int i = 0; i < N; i++) {
			res[root(i)]++;
		}
		return res;
	}
};