#pragma once

//不完全

#include <unordered_map>
#include <memory>

template<typename Key,typename T>
//オブジェクトをスマートポインタで動的に管理するリスト
//クラスはKeyと結び付けられる
//登録するクラスには
//bool Update(float)      void Draw() const void Kill()  が必要
class ObjectMap {
	std::unordered_map<Key,std::unique_ptr<T>> mMap;
public:
	ObjectMap() {}
	void Update(float dt=1.0f) {
		for (auto it = mMap.begin(), ite = mMap.end(); it != ite;) {
			if (!it->second->Update(dt)) {//Updateがfalse(そのオブジェクトが無効になった)なら削除
				it = mMap.erase(it);
				continue;
			}
			++it;
		}
	}
	void Draw() const {
		for (const auto& it : mMap) {
			it.second->Draw();
		}
	}
	void clear(){}
	int size()const { return 0; }
	void push(Key key, T* Obj) { mMap.emplace(key,Obj); }
};