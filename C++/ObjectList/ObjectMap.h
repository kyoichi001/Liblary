#pragma once

//�s���S

#include <unordered_map>
#include <memory>

template<typename Key,typename T>
//�I�u�W�F�N�g���X�}�[�g�|�C���^�œ��I�ɊǗ����郊�X�g
//�N���X��Key�ƌ��ѕt������
//�o�^����N���X�ɂ�
//bool Update(float)      void Draw() const void Kill()  ���K�v
class ObjectMap {
	std::unordered_map<Key,std::unique_ptr<T>> mMap;
public:
	ObjectMap() {}
	void Update(float dt=1.0f) {
		for (auto it = mMap.begin(), ite = mMap.end(); it != ite;) {
			if (!it->second->Update(dt)) {//Update��false(���̃I�u�W�F�N�g�������ɂȂ���)�Ȃ�폜
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