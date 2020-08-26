#include <iostream>
#include "Vec3.cpp"

using  ll = long long;

class VecInt3 {
public:
	ll x, y, z;
	VecInt3()noexcept :x(0), y(0), z(0) {}
	VecInt3(ll X, ll Y, ll Z)noexcept : x(X), y(Y), z(Z) {}
	~VecInt3()noexcept {}

	ll dot(const VecInt3& Obj)const  noexcept { return x * Obj.x + y * Obj.y + z * Obj.z; }
	ll lengthSqu()const noexcept { return dot(*this); }
	double length()const noexcept { return sqrt(lengthSqu()); }
	VecInt3 closs(const VecInt3& Obj)const  noexcept {
		return {
		  y * Obj.z - z * Obj.y,
		  z * Obj.x - x * Obj.z,
		  x * Obj.y - y * Obj.x
		};
	}
	Vec3 Normalized()const noexcept { return *this / length(); }

	VecInt3 operator+() const noexcept { return *this; }
	VecInt3 operator-() const noexcept { return { -x, -y, -z }; }
	bool operator ==(const VecInt3& Obj)const noexcept { return  x == Obj.x && y == Obj.y && z == Obj.z; }
	bool operator !=(const VecInt3& Obj)const noexcept { return x != Obj.x || y != Obj.y || z != Obj.z; }
	VecInt3 operator +(const VecInt3& Obj)const noexcept { return { x + Obj.x, y + Obj.y, z + Obj.z }; }
	VecInt3 operator -(const VecInt3& Obj)const noexcept { return { x - Obj.x, y - Obj.y, z - Obj.z }; }
	Vec3 operator *(double a)const noexcept { return { (double)x * a, (double)y * a, (double)z * a }; }
	Vec3 operator /(double a)const noexcept { return { (double)x / a, (double)y / a, (double)z / a }; }
	void operator +=(const VecInt3& Obj) noexcept { x += Obj.x; y += Obj.y; z += Obj.z; }
	void operator -=(const VecInt3& Obj) noexcept { x -= Obj.x; y -= Obj.y; z -= Obj.z; }

	Vec3 ToDouble()const noexcept { return Vec3(x, y, z); }
};

std::ostream& operator<<(std::ostream& os, const VecInt3& dt)
{
	os << "(" << dt.x << ", " << dt.y << ", " << dt.z << ")";
	return os;
}

Vec3 operator*(double a, const VecInt3& obj) { return obj * a; }

Vec3 operator+(const Vec3& a, const VecInt3& b) { return { a.x + (double)b.x, a.y + (double)b.y, a.z + (double)b.z }; }
Vec3 operator-(const Vec3& a, const VecInt3& b) { return { a.x - (double)b.x, a.y - (double)b.y, a.z - (double)b.z }; }

Vec3 operator+(const VecInt3& a, const Vec3& b) { return b + a; }
Vec3 operator-(const VecInt3& a, const Vec3& b) { return  -b + a; }