
#include "Collision.h"
#include "Geometory.h"
#include "../Primitive/Primitive.h"
#include "../Func.h"

bool IsHit(const Point& Obj1, const Point& Obj2)noexcept { return Obj1 == Obj2; }
bool IsHit(const Point& Obj1, const Circle& Obj2)noexcept {
	//if (Obj2.r <= 0)return false;
	return Obj2.p.lengthSqu(Obj1) < Obj2.r*Obj2.r;
}
bool IsHit(const Point& Obj1, const Segment& Obj2)noexcept {
	Vec2 p = Obj1.toVec2();
	return (p - Obj2.begin).closs(p - Obj2.end) == 0.0f;
}
bool IsHit(const Point& Obj1, const Rect& Obj2)noexcept {
	return RANGE(Obj2.tl.x, Obj1.x, Obj2.tl.x + Obj2.width) &&
		RANGE(Obj2.tl.x, Obj1.x, Obj2.tl.x + Obj2.width);
}
bool IsHit(const Point& Obj1, const Capsule& Obj2)noexcept {
	return distanceSqu(Obj1, Obj2) < Obj2.r*Obj2.r;
}

bool IsHit(const Circle& Obj1, const Point& Obj2)noexcept { return IsHit(Obj2, Obj1); }
bool IsHit(const Circle& Obj1, const Circle& Obj2)noexcept {
	//if (Obj1.r <= 0 || Obj2.r <= 0)return false;
	return Obj1.p.lengthSqu(Obj2.p) < (Obj1.r + Obj2.r) * (Obj1.r + Obj2.r);
}
bool IsHit(const Circle& Obj1, const Segment& Obj2)noexcept {
	return distanceSqu(Obj1.p, Obj2) < (Obj1.r)*Obj1.r;
}
bool IsHit(const Circle& Obj1, const Rect& Obj2)noexcept {
	//if (Obj1.r <= 0)return false;
	return
		RANGE(Obj2.tl.x - Obj1.r, Obj1.p.x, Obj2.tl.x + Obj2.width + Obj1.r) &&
		RANGE(Obj2.tl.y - Obj1.r, Obj1.p.y, Obj2.tl.y + Obj2.height + Obj1.r);
}
bool IsHit(const Circle& Obj1, const Capsule& Obj2)noexcept {
	return distanceSqu(Obj1.p, Obj2.segment) < (Obj1.r + Obj2.r)*(Obj1.r + Obj2.r);
}

bool IsHit(const Segment& Obj1, const Point& Obj2)noexcept { return IsHit(Obj2, Obj1); }
bool IsHit(const Segment& Obj1, const Circle& Obj2)noexcept { return IsHit(Obj2, Obj1); }
bool IsHit(const Segment& Obj1, const Segment& Obj2)noexcept {
	return false;
}
bool IsHit(const Segment& Obj1, const Rect& Obj2)noexcept {
	return false;
}
bool IsHit(const Segment& Obj1, const Capsule& Obj2)noexcept {
	return false;
}


bool IsHit(const Rect& Obj1, const Point& Obj2)noexcept { return IsHit(Obj2, Obj1); }
bool IsHit(const Rect& Obj1, const Circle& Obj2)noexcept { return IsHit(Obj2, Obj1); }
bool IsHit(const Rect& Obj1, const Segment& Obj2)noexcept { return IsHit(Obj2, Obj1); }
bool IsHit(const Rect& Obj1, const Rect& Obj2)noexcept {
	return false;
}
bool IsHit(const Rect& Obj1, const Capsule& Obj2)noexcept {
	return false;
}

bool IsHit(const Capsule& Obj1, const Point& Obj2) noexcept { return IsHit(Obj2, Obj1); }
bool IsHit(const Capsule& Obj1, const Circle& Obj2) noexcept { return IsHit(Obj2, Obj1); }
bool IsHit(const Capsule& Obj1, const Segment& Obj2)noexcept { return IsHit(Obj2, Obj1); }
bool IsHit(const Capsule& Obj1, const Rect& Obj2) noexcept { return IsHit(Obj2, Obj1); }
bool IsHit(const Capsule& Obj1, const Capsule& Obj2)noexcept {
	return false;
}