#pragma once

#include <list>

template<typename T>
//�P��N���X�̃I�u�W�F�N�g�����X�g�ɐ������A���I�ɊǗ����郊�X�g
//�o�^����N���X�ɂ�
//bool Update(float)      void Draw() const void Kill()  ���K�v
//���z�֐����g�p���Ă���N���X�ɂ͎g���܂���
class FixedObjectList {
	std::list<T> mObj;
public:
	FixedObjectList()noexcept {}
	~FixedObjectList()noexcept {}

	void Update(float dt = 1.0f)noexcept {
		for (auto it = mObj.begin(), ite = mObj.end(); it != ite;) {
			if (!(*it).Update(dt)) {//Update��false(���̃I�u�W�F�N�g�������ɂȂ���)�Ȃ�폜
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

	//�I�u�W�F�N�g�����X�g�ɒ��ڐ�������
	//�����̓N���X�̃R���X�g���N�^�����Ɠ���
	template<typename ...Args>
	T* Emplace(const Args&... args)noexcept {
		mObj.emplace_front(args...);
		return &mObj.front();
	}
	auto begin() {return mObj.begin();}
	auto end() {return mObj.end();}
	size_t size() const { return mObj.size(); }

};
