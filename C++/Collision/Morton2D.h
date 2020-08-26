#pragma once

#include <vector>
#include <cmath>


int bitSeparate(int n) {
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	return (n | (n << 1)) & 0x55555555;
}

int morton(int x, int y)noexcept {
	return bitSeparate(x) | (bitSeparate(y) << 1);
}
int getMortonLevel(int morton)noexcept {
	int level = 0;
	for (; morton != 0; morton >>= 2, level++);
	return level;
}
int getMorton(const Point& p, const Point& O, int cellX, int cellY)noexcept {
	int X = (p.x - O.x) / cellX, Y = (p.y - O.y) / cellY;
	return morton(X,Y);
}
int getMorton(const Circle& p, const Point& O, int cellX, int cellY)noexcept {
	const Point tl = { p.p.x - p.r,p.p.y - p.r };
	const Point br = { p.p.x + p.r,p.p.y + p.r };
	int tlmtn = getMorton(tl, O, cellX, cellY);
	int brmtn = getMorton(br, O, cellX, cellY);
	int mtn = tlmtn ^ brmtn;
	int level = getMortonLevel(mtn);
	return (brmtn >> (level * 2)) + ((int)pow(4, 3 - level) - 1) / 3;
}

template<class T>
class QuadTree {
	std::vector<std::vector<T*>> mList;
	Point mOrigin;
	Point mEnd;
	int mLevel;
	int mCellX, mCellY;
public:
	template<class Obj>
	void forEachChildren(int morton, Obj* object) {
		if (morton >= mList.size())return;
		if (morton < 0)return;
		for (auto it : mList[morton])object->Collided(*it);
		forEachChildren(morton * 4 + 1, object);
		forEachChildren(morton * 4 + 2, object);
		forEachChildren(morton * 4 + 3, object);
		forEachChildren(morton * 4 + 4, object);
	}

	template<class Obj>
	void forEachChildren(int morton, Obj* object, void(*callback)(const Obj&, const T&)) {
		if (morton >= mList.size())return;
		if (morton < 0)return;
		for (auto it : mList[morton])callback(*object, *it);
		forEachChildren(morton * 4 + 1, object, callback);
		forEachChildren(morton * 4 + 2, object, callback);
		forEachChildren(morton * 4 + 3, object, callback);
		forEachChildren(morton * 4 + 4, object, callback);
	}

	template<class Obj>
	void forEachParent(int morton, Obj* object) {
		if (morton <= 0)return;
		if (morton >= mList.size())return;
		for (int i = (morton - 1) / 4; i > 0; i = (i - 1) / 4) {
			for (auto it : mList[i]) {
				object->Collided(*it);
			}
		}
		for (auto it : mList[0]) object->Collided(*it);
	}

	template<class Obj>
	void forEachParent(int morton, Obj* object, void(*callback)(const Obj&, const T&)) {
		if (morton <= 0)return;
		if (morton >= mList.size())return;
		for (int i = (morton - 1) / 4; i > 0; i = (i - 1) / 4) {
			for (auto it : mList[i]) {
				callback(*object, *it);
			}
		}
		for (auto it : mList[0]) callback(*object, *it);
	}

	template<class Obj>
	void forEachThis(int morton, Obj* object) {
		if (morton < 0)return;
		if (morton >= mList.size())return;
		for (auto it : mList[morton]) object->Collided(*it);
	}

	template<class Obj>
	void forEachThis(int morton, Obj* object, void(*callback)(const Obj&, const T&)) {
		if (morton < 0)return;
		if (morton >= mList.size())return;
		for (auto it : mList[morton]) callback(*object, *it);
	}

	QuadTree(int level, const Rect& region) noexcept
		: mLevel(level), mOrigin(region.tl), mList((pow(4, level + 1) - 1) / 3), mEnd(region.br()){
		int s = pow(2, level); 
		mCellX = region.width / s;
		mCellY = region.height / s;
	}
	void push(int morton, T* obj) {
		if (morton < 0)morton = 0;
		if (morton >= mList.size())morton = 0;
		mList[morton].push_back(obj);
	}
	void push(T* obj) {
		push(GetMorton(obj), obj);
	}
	void clear() noexcept {
		for (auto& list : mList) list.clear();
	}
	template<class Obj>
	void Collided(Obj* object) {
		int morton = getMorton(object, mOrigin, mCellX, mCellY);
		//parent
		forEachParent(morton, object);
		//child and this node
		forEachChildren(morton, object);
	}

	template<class Obj>
	int GetMorton(Obj* obj) {
		return getMorton(obj, mOrigin, mCellX, mCellY);
	}
};