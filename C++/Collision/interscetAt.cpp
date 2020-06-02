#include "intersectAt.h"


std::vector<Point> IntersectAt(const HalfLine& obj1, const Circle& obj2) {
	float vecXcirO = obj1.getDirection().dot(obj2.p - obj1.p);
	float distanceSqu = obj1.p.lengthSqu(obj2.p);
	float D = vecXcirO * vecXcirO - distanceSqu + obj2.r*obj2.r;
	if (D == 0) {
		return { obj1.getCood(vecXcirO) };
	}
	else if (D > 0) {
		float rootD = sqrt(D);
		return { obj1.getCood(vecXcirO - rootD),obj1.getCood(vecXcirO + rootD) };
	}
	return {};
}
std::vector<Point> IntersectAt(const HalfLine& obj1, const Segment& obj2) {
	const Vec2 dir = obj1.getDirection(), segV = obj2.vector(), pb = obj1.p - obj2.begin;
	float dirxV = dir.closs(segV);
	if (dirxV == 0)return {};
	float t = segV.closs(pb) / dirxV;
	float s = dir.closs(pb) / dirxV;
	if (0.0f <= t && 0.0f <= s && s <= 1.0f) {
		return { obj1.p + dir * t };
	}
	return {};
	//https://spphire9.wordpress.com/2013/01/14/%E7%9B%B4%E7%B7%9A%E3%81%A8%E7%B7%9A%E5%88%86%E3%81%AE%E5%BD%93%E3%81%9F%E3%82%8A%E5%88%A4%E5%AE%9A/
}
std::vector<Point> IntersectAt(const HalfLine& obj1, const Rect& obj2) {
	Segment seg[4] = { obj2.top(),obj2.btm(),obj2.lft(),obj2.rgt() };
	std::vector<Point> a;
	for (int i = 0; i < 4; i++) {
		for (auto& i : IntersectAt(obj1, seg[i]))
			a.push_back(i);
	}
	return a;
}