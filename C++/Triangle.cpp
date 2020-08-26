#include <iostream>
#include "Vec3.cpp"
#include "VecInt3.cpp"

class Triangle {
public:
	VecInt3 a, b, c;
	Vec3 norm;
	Triangle() {}
	Triangle(ll ax, ll ay, ll az, ll bx, ll by, ll bz, ll cx, ll cy, ll cz) :a(ax, ay, az), b(bx, by, bz), c(cx, cy, cz) {
		CalcNorm();
	}
	Triangle(const VecInt3& a_, const VecInt3& b_, const VecInt3& c_) :a(a_), b(b_), c(c_) {
		CalcNorm();
	}
	void CalcNorm() {
		norm = (b - a).closs(c - a).Normalized();
	}
	bool isInsideTriangle(const Vec3& p)const {
		Vec3 nab = (b - a).ToDouble();
		Vec3 nca = (a - c).ToDouble();
		Vec3 nbc = (c - b).ToDouble();

		Vec3 nap = (p - a);
		Vec3 nbp = (p - b);
		Vec3 ncp = (p - c);

		Vec3 c1 = nab.closs(nbp);
		Vec3 c2 = nbc.closs(ncp);
		Vec3 c3 = nca.closs(nap);

		//内積で順方向か逆方向か調べる
		return c1.dot(c2) > 0 && c1.dot(c3) > 0;
	}
};