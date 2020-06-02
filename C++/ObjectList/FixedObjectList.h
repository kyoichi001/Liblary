#pragma once

#include <list>

template<typename T>
//単一クラスのオブジェクトをリストに生成し、動的に管理するリスト
//登録するクラスには
//bool Update(float)      void Draw() const void Kill()  が必要
//仮想関数を使用しているクラスには使えません
class FixedObjectList {
	std::list<T> mObj;
public:
	FixedObjectList()noexcept {}
	~FixedObjectList()noexcept {}

	void Update(float dt = 1.0f)noexcept {
		for (auto it = mObj.begin(), ite = mObj.end(); it != ite;) {
			if (!(*it).Update(dt)) {//Updateがfalse(そのオブジェクトが無効になった)なら削除
				it = mObj.erase(it);
				continue;
			}
			++it;
			}
	}
	void Draw()noexcept {
		for (const auto &it : mObj)it.Draw();
	}
	void Kill()noexcept {
		for (auto &it : mObj)it.Kill();
	}

	//オブジェクトをリストに直接生成する
	//引数はクラスのコンストラクタ引数と同じ
	template<typename ...Args>
	T* Emplace(const Args&... args)noexcept {
		mObj.emplace_front(args...);
		return &mObj.front();
	}
	auto begin() {return mObj.begin();}
	auto end() {return mObj.end();}
	size_t size() const { return mObj.size(); }

};
