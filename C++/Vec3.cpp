#include<iostream>

class Vec3 {
public:
	double x, y, z;
	Vec3()noexcept :x(0.0f), y(0.0f), z(0.0f) {}
	Vec3(double X, double Y, double Z)noexcept : x(X), y(Y), z(Z) {}
	~Vec3()noexcept {}

	double dot(const Vec3& Obj)const  noexcept { return x * Obj.x + y * Obj.y + z * Obj.z; }
	double lengthSqu()const noexcept { return dot(*this); }
	double length()const noexcept { return sqrt(lengthSqu()); }
	Vec3 closs(const Vec3& Obj)const  noexcept {
		return {
		   y * Obj.z - z * Obj.y,
		   z * Obj.x - x * Obj.z,
		   x * Obj.y - y * Obj.x
		};
	}
	Vec3 Normalized()const noexcept { return *this / length(); }

	Vec3 operator+() const noexcept { return *this; }
	Vec3 operator-() const noexcept { return { -x, -y, -z }; }
	bool operator ==(const Vec3& Obj)const noexcept { return x == Obj.x && y == Obj.y && z == Obj.z; }
	bool operator !=(const Vec3& Obj)const noexcept { return x != Obj.x || y != Obj.y || z != Obj.z; }
	Vec3 operator +(const Vec3& Obj)const  noexcept { return { x + Obj.x, y + Obj.y, z + Obj.z }; }
	Vec3 operator -(const Vec3& Obj)const  noexcept { return { x - Obj.x, y - Obj.y, z - Obj.z }; }
	Vec3 operator *(double a)const noexcept { return { x * a, y * a, z * a }; }
	Vec3 operator /(double a)const noexcept { return { x / a, y / a, z / a }; }
	void operator +=(const Vec3& Obj) noexcept { x += Obj.x; y += Obj.y; z += Obj.z; }
	void operator -=(const Vec3& Obj) noexcept { x -= Obj.x; y -= Obj.y; z -= Obj.z; }

};

std::ostream& operator<<(std::ostream& os, const Vec3& dt)
{
	os << "(" << dt.x << ", " << dt.y << ", " << dt.z << ")";
	return os;
}

Vec3 operator*(double a, const Vec3& obj) { return obj * a; }
