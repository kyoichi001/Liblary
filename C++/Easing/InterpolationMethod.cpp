
#define DEBUG

#include "InterpolationMethod.h"
#include "../Math/Constant.h"
#include <cmath>

float lerp(float Value1, float Value2, int min, int max, int NowTime, EasingFunc func)noexcept {

	if (NowTime <= min)return Value1;
	else if (NowTime >= max) return Value2;

	float t = float(NowTime - min) / float(max - min);

	float rate = (*func)(t);
	return Value1 * (1.0f - rate) + Value2 * rate;
}
int lerp(int Value1, int Value2, int min, int max, int NowTime, EasingFunc func)noexcept {

	if (NowTime <= min)return Value1;
	else if (NowTime >= max) return Value2;

	float t = float(NowTime - min) / float(max - min);

	float rate = (*func)(t);
	return float(Value1) * (1.0f - rate) + float(Value2) * rate;
}

float D_lerp(float Value, int min, int max, int NowTime, EasingFunc func)noexcept {
	if (NowTime <= min || NowTime >= max)return 0.0f;
	float t = float(NowTime - min) / float(max - min);
	return (Value / float(max - min))*(*func)(t);
}
int D_lerp(int Value, int min, int max, int NowTime, EasingFunc func)noexcept {
	if (NowTime < min || NowTime > max)return 0;
	float t = float(NowTime - min) / float(max - min);
	return (float(Value) / float(max - min))*(*func)(t);
}

float Liner(float t)  noexcept{ return t; }
float Quad(float t)   noexcept{ return t*t; }
float Cubic(float t)  noexcept{ return t*t*t; }
float Quart(float t)  noexcept{ return t*t*t*t; }
float Quint(float t)  noexcept{ return t*t*t*t*t; }
float Expo(float t)   noexcept{ return t == 0.0f ? 0.0f : pow(2.0f, 10.0f * (t - 1)); }
float Back(float t)   noexcept{ return t*t*(2.70158f*t - 1.70158f); }
float Elastic(float t)noexcept{ return -pow(2.0f, 10.0f * (t - 1))*sin((t - 1.278f) * 9.0f * PI); }
float Sin(float t)    noexcept{ return 1.0f - cos(t*HALF_PI); }

float D_Liner(float t)noexcept { return 1.0f; }
float D_Quad(float t) noexcept { return 2.0f*t; }
float D_Cubic(float t)noexcept { return 3.0f*t*t; }
float D_Quart(float t)noexcept { return 4.0f*t*t*t; }
float D_Sin(float t)  noexcept { return HALF_PI*sin(t*HALF_PI); }


