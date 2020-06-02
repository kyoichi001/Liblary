
#pragma once

#include <list>
#include <memory>


template<typename T>
//オブジェクトをスマートポインタで動的に管理するリスト
//登録するクラスには
//bool Update(float)      void Draw() const void Kill()  が必要
class ObjectList {
	std::list<std::unique_ptr<T>> mObj;
public:
	ObjectList()noexcept {}
	~ObjectList()noexcept {}

	void Update(float dt = 1.0f)noexcept {
		for (auto it = mObj.begin(), ite = mObj.end(); it != ite;) {
			if (!(*it)->Update(dt)) {//Updateがfalse(そのオブジェクトが無効になった)なら削除
				it = mObj.erase(it);
				continue;
			}
			++it;
		}
	}
	void Draw()const noexcept {
		for (const auto &it : mObj)it->Draw();
	}
	void Kill()noexcept {
		for (auto &it : mObj)it->Kill();
	}

	T* SetObject(T* Obj)noexcept {
		mObj.emplace_front(Obj);
		return mObj.front().get();
	}
	auto begin() { return mObj.begin(); }
	auto end() { return mObj.end(); }
	size_t size()const { return mObj.size(); }

};
