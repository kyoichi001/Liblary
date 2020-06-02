
#pragma once

#include "InterpolationMethod.h"

using call_back = void(*)();

//operator +(T,T)が定義されているクラスで動作する補間クラス
template<typename T>
class Easing{
	T* mValue;//動かす変数
	T mMax, mMin;//最初と最後の値
	int mTime, mLimit;
	EasingFunc mEasing;
	call_back mCallBack;//終わったとき実行する関数
	bool mStartFlag;
public:
	Easing()noexcept :mStartFlag(false){}

	Easing(T* Value, const T& min, const T& max, int Time, EasingFunc Easing = Ease<Liner>::In, call_back CallBack = [](){}) noexcept :
		mValue(Value), mMax(max), mMin(min), mLimit(Time), mEasing(Easing), mTime(0), mCallBack(CallBack), mStartFlag(true){}
	~Easing()noexcept{}
	void Init(T* Value, const T& min, const T& max, int Time, EasingFunc Easing = Ease<Liner>::In, call_back CallBack = [](){})noexcept {
		mValue = Value;
		mMin = min;
		mMax = max;
		mLimit = Time;
		mEasing = Easing;
		mTime = 0;
		mCallBack = CallBack;
		mStartFlag = true;
	}
	void InitReverce()noexcept {
		T a = mMin;
		mMin = mMax;
		mMax = a;
		mTime = 0;
		mStartFlag = true;
	}
	void Update(float t=1.0f)noexcept {
		if (mStartFlag){
			if (mTime<mLimit ){
				float s = float(mTime) / float(mLimit);// 時間を媒介変数に
				*mValue = mMin * (1.0f - mEasing(s)) + mMax * mEasing(s);
				mTime+=t;
			}
			else if (mTime >= mLimit){
				mCallBack();
				mStartFlag = false;
			}
		}
	}
	bool IsFinished()const noexcept { return !mStartFlag; }//もう終わっているかどうか
	bool IsFinish()const noexcept { return mTime==mLimit; }//終わる瞬間かどうか
	void Stop()noexcept { mStartFlag = false; }//一時停止
	void Restart()noexcept { mStartFlag = true; }//再開
	void Redo()noexcept {//最初からやり直す
		mStartFlag = true;
		mTime = 0;
	}
};

//operator +(T,T)が定義されているクラスで動作する補間クラス
template<typename T>
class D_Easing {
	T* mValue;
	T mMax;
	int mTime, mLimit;
	EasingFunc mEasing;
	call_back mCallBack;
	bool mStartFlag;
public:
	D_Easing()noexcept : mStartFlag(false) {}

	D_Easing(T* Value,const T& max, int Time, EasingFunc Easing = Ease<Liner>::In, call_back CallBack = []() {}) noexcept :
		mValue(Value), mMax(max), mLimit(Time), mEasing(Easing), mTime(0), mCallBack(CallBack), mStartFlag(true) {}
	~D_Easing()noexcept {}
	void Init(T* Value, const T& max, int Time, EasingFunc Easing = Ease<Liner>::In, call_back CallBack = []() {})noexcept {
		mValue = Value;
		mMax = max;
		mLimit = Time;
		mEasing = Easing;
		mTime = 0;
		mCallBack = CallBack;
		mStartFlag = true;
	}
	void Update(float t = 1.0f)noexcept {
		if (mStartFlag) {
			if (mTime < mLimit) {
				float s = float(mTime) / float(mLimit);// 時間を媒介変数に
				*mValue += (mMax / mLimit)*mEasing(s);
				mTime += t;
			}
			else if (mTime >= mLimit) {
				mCallBack();
				mStartFlag = false;
			}
		}
	}
	bool IsFinished()const noexcept { return !mStartFlag; }
	bool IsFinish()const noexcept { return mTime == mLimit; }
	void Stop()noexcept { mStartFlag = false; }
	void Restart()noexcept { mStartFlag = true; }
	void Redo()noexcept {
		mStartFlag = true;
		mTime = 0;
	}
};