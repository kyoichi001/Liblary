
#pragma once

using EasingFunc = float(*)(float);

//Value1����Value2�܂Ŏ���NowTime��min����max�̊Ԃ���func�ŕ�Ԃ���֐�
float lerp(float Value1, float Value2, int min, int max, int NowTime, EasingFunc func)noexcept;
int lerp(int Value1, int Value2, int min, int max, int NowTime, EasingFunc func)noexcept;

//��Ԋ֐��̃e���v���[�g
//Ease<Quad>::In�Ƃ���΁A�Q���֐����g���Ă����������Đ����悭�������Ԋ֐����ł���
template <float(*Function)(float)>
class Ease{
public:
	//�ŏ��������
	static float In(float t)noexcept { return Function(t); }
	//���Ԃł������
	static float Middle(float t)noexcept { return (t < 0.5f) ? 0.5 + 0.5*In(2.0f)*In(t - 0.5f) : 0.5 - 0.5*In(2.0f)*In(t - 0.5f); }
	//�Ō�������
	static float Out(float t)noexcept { return 1.0f - In(1.0f - t); }
	//�ŏ��ƍŌ�ł������
	static float InOut(float t)noexcept { return (t < 0.5f) ? In(2.0f * t) * 0.5f : 0.5f + Out(2.0f * t - 1.0f) * 0.5f; };
};

float Liner(float t)  noexcept;
float Quad(float t)   noexcept;
float Cubic(float t)  noexcept;
float Quart(float t)  noexcept;
float Quint(float t)  noexcept;
float Expo(float t)   noexcept;
float Back(float t)   noexcept;
float Elastic(float t)noexcept;
float Sin(float t)    noexcept;

//�����l���珉���l+Value�܂Ŏ���NowTime��min����max�̊Ԃ���func�ŕ�Ԃ���֐�
float D_lerp(float Value, int min, int max, int NowTime, EasingFunc func)noexcept;
int D_lerp(int Value, int min, int max, int NowTime, EasingFunc func)noexcept;

//��Ԋ֐��̃e���v���[�g
//Ease�ƈႢ�A������͕�Ԃ���Ƃ��ɒl�����ł͂Ȃ����Z�ŕ�Ԃ��Ă���
//�����l�ɂ�炸�������������ɂ�����l��r���ŕς������ꍇ��
template <float(*Function)(float)>
class D_Ease {
public:
	static float In(float t)noexcept { return Function(t); }
	//static float Middle(float t)noexcept { return (t < 0.5f) ? 0.5 + 0.5*In(2.0f)*In(t - 0.5f) : 0.5 - 0.5*In(2.0f)*In(t - 0.5f); }
	static float Out(float t)noexcept { return In(1.0f - t); }
	static float InOut(float t)noexcept { return (t < 0.5f) ? In(2.0f * t) : Out(2.0f * t - 1.0f); };
};

float D_Liner(float t)noexcept;
float D_Quad(float t) noexcept;
float D_Cubic(float t)noexcept;
float D_Quart(float t)noexcept;
float D_Sin(float t)  noexcept;
