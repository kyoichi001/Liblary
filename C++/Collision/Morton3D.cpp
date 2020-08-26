#include "VecInt3.cpp"
#include "Vec3.cpp"

namespace myLib {
	ll max(ll a, ll b, ll c) {
		int res = a < b ? b : a;
		res = res < c ? c : res;
		return res;
	}
	ll min(ll a, ll b, ll c) {
		int res = a > b ? b : a;
		res = res > c ? c : res;
		return res;
	}
	VecInt3 max(const VecInt3& a, const VecInt3& b, const VecInt3& c) {
		return 	{ myLib::max(a.x, b.x, c.x), myLib::max(a.y, b.y, c.y), myLib::max(a.z, b.z, c.z) };
	}
	VecInt3 min(const VecInt3& a, const VecInt3& b, const VecInt3& c) {
		return 	{ myLib::min(a.x, b.x, c.x), myLib::min(a.y, b.y, c.y), myLib::min(a.z, b.z, c.z) };
	}
}

ll BitSeparateFor3D(ll n)
{
	ll s = n;
	s = (s | s << 8) & 0x0000f00f;
	s = (s | s << 4) & 0x000c30c3;
	s = (s | s << 2) & 0x00249249;
	return s;
}
ll morton(ll x, ll y, ll z)noexcept {
	return BitSeparateFor3D(x) | (BitSeparateFor3D(y) << 1) | (BitSeparateFor3D(z) << 2);
}
ll getMortonLevel(ll morton)noexcept {
	int level = 0;
	for (; morton != 0; morton >>= 3, level++);
	return level;
}
ll getMorton(const VecInt3& p, const VecInt3& O, ll cellX, ll cellY, ll cellZ)noexcept {
	int X = (p.x - O.x) / cellX, Y = (p.y - O.y) / cellY, Z = (p.z - O.z) / cellZ;
	return morton(X, Y, Z);
}

ll getMorton(const VecInt3& tl,const VecInt3& br,const VecInt3& O, ll cellX, ll cellY, ll cellZ, ll level_){
	ll tlmtn = getMorton(tl, O, cellX, cellY, cellZ);
	ll brmtn = getMorton(br, O, cellX, cellY, cellZ);
	ll mtn = tlmtn ^ brmtn;
	ll level = getMortonLevel(mtn);
	return (brmtn >> (level * 3)) + ((ll)pow(8, level_ - level) - 1) / 7;
}

ll getMorton(const Triangle& p, const VecInt3& O, ll cellX, ll cellY, ll cellZ, ll level_)noexcept {
	const VecInt3 tl = myLib::min(p.a, p.b, p.c);
	const VecInt3 br = myLib::max(p.a, p.b, p.c);
	return getMorton(tl,br,O,cellX,cellY,cellZ,level_);
}

ll getMorton(const Sphere& c, const VecInt3& O, ll cellX, ll cellY, ll cellZ, ll level_)noexcept {
	const VecInt3 tl = { c.o.x - c.radius,c.o.y - c.radius,c.o.z - c.radius };
	const VecInt3 br = { c.o.x + c.radius,c.o.y + c.radius,c.o.z + c.radius };
	return getMorton(tl,br,O,cellX,cellY,cellZ,level_);
}

template<class T>
class OctTree {
	std::vector<std::vector<T*>> mList;
	VecInt3 mOrigin;
	VecInt3 mEnd;
	ll mLevel;
	ll mCellX, mCellY, mCellZ;
public:

	template<class Obj, typename Func>
	void forEachChildren(ll morton, Obj& object, Func callback) {
		if (morton >= mList.size())return;
		if (morton < 0)return;
		for (auto it : mList[morton])callback(object, *it);
		forEachChildren(morton * 8 + 1, object, callback);
		forEachChildren(morton * 8 + 2, object, callback);
		forEachChildren(morton * 8 + 3, object, callback);
		forEachChildren(morton * 8 + 4, object, callback);
		forEachChildren(morton * 8 + 5, object, callback);
		forEachChildren(morton * 8 + 6, object, callback);
		forEachChildren(morton * 8 + 7, object, callback);
		forEachChildren(morton * 8 + 8, object, callback);
	}


	template<class Obj, typename Func>
	void forEachParent(ll morton, Obj& object, Func callback) {
		if (morton <= 0)return;
		if (morton >= mList.size())return;
		for (int i = (morton - 1) >>3; i > 0; i = (i - 1) >>3) {
			for (auto it : mList[i]) {
				callback(object, *it);
			}
		}
		for (auto it : mList[0]) callback(object, *it);
	}

	template<class Obj>
	void forEachThis(ll morton, Obj& object, void(*callback)(const Obj&, const T&)) {
		if (morton < 0)return;
		if (morton >= mList.size())return;
		for (auto it : mList[morton]) callback(object, *it);
	}

	OctTree(ll level, const VecInt3& origin, const VecInt3& end) noexcept
		: mLevel(level), mOrigin(origin), mList((pow(8, level + 1) - 1) / 7), mEnd(end) {
		int s = pow(2, level);
		mCellX = (mEnd.x - mOrigin.x) / s;
		mCellY = (mEnd.y - mOrigin.y) / s;
		mCellZ = (mEnd.z - mOrigin.z) / s;
	}
	void push(ll morton, T* obj) {
		if (morton < 0)morton = 0;
		if (morton >= mList.size())morton = 0;
		mList[morton].push_back(obj);
	}

	template<class Obj>
	int GetMorton(Obj* obj) {
		return getMorton(*obj, mOrigin, mCellX, mCellY, mCellZ, mLevel);
	}

	void push(T* obj) {
		push(this->GetMorton(obj), obj);
	}
	void clear() noexcept {
		for (auto& list : mList) list.clear();
	}

	template<class Obj, typename Func>
	void Check(Obj& object, Func callback) {
		int morton = getMorton(object, mOrigin, mCellX, mCellY, mCellZ, mLevel);
		//parent
		forEachParent(morton, object, callback);
		//child and this node
		forEachChildren(morton, object, callback);
	}

};