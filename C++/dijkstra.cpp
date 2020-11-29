
#include<limits>
#include <queue>
//
//https://nexusuica.hatenablog.jp/entry/2018/08/14/114524
//片方向ダイクストラ
class Graph {
public :
	using Type = long long;
private:
	struct edge { Type to, cost; };
	Type V;
public:
	std::vector<std::vector<edge> > G;//隣接頂点リスト
	const Type INF;
	Graph(Type n) :INF(std::numeric_limits<Type>::max()) {
		init(n);
	}

	void init(Type n) {
#ifdef _DEBUG
		if (n <= 0) {
			std::cerr << "n is 0 or negative" << std::endl;
			return;
		}
#endif // _DEBUG

		V = n;
		G.resize(V);
	}

	void add_edge(Type s, Type t, Type cost) {
#ifdef _DEBUG
		if (s >= V || s < 0) {
			std::cerr << "s (" << s << ") is out of length" << std::endl;
			return;
		}
		if (t >= V || s < 0) {
			std::cerr << "t (" << t << ") is out of length" << std::endl;
			return;
		}
		if (cost < 0) {
			std::cerr << "cost (" << cost << ") is negative" << std::endl;
			return;
		}
#endif // _DEBUG
		edge e;
		e.to = t, e.cost = cost;
		G[s].push_back(e);
	}

	int VertexCount()const { return V; }
};


std::vector<Graph::Type> dijkstra(const Graph& G,Graph::Type s) {
	auto V = G.VertexCount();
#ifdef _DEBUG
	if (s < 0 || s >= V) {
		std::cerr << "s (" << s << ") is out of length" << std::endl;
		return std::vector<Graph::Type>();
	}
#endif // _DEBUG

	std::vector<Graph::Type> d;//スタートからの頂点間の最小距離
	typedef std::pair<Graph::Type, Graph::Type> P;
	for (int i = 0; i < V; i++) d[i] = G.INF;
	d[s] = 0;
	std::priority_queue<P, std::vector<P>, std::greater<P> > que;
	que.push(P(0, s));//コストとエッジの開始頂点
	while (!que.empty()) {
		P p = que.top(); que.pop();
		Graph::Type v = p.second;
		if (d[v] < p.first) continue;//自分の頂点距離が次頂点の頂点距離より大きかったらスキップ
		for (auto e : G.G[v]) {
			if (d[e.to] > d[v] + e.cost) {
				d[e.to] = d[v] + e.cost;
				que.push(P(d[e.to], e.to));
			}
		}
	}
	return d;
}