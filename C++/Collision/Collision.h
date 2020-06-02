#pragma once

class Point;
class Segment;
class Capsule;
class Circle;
class Rect;

bool IsHit(const Point& Obj1, const Point& Obj2)noexcept;
bool IsHit(const Point& Obj1, const Circle& Obj2)noexcept;
bool IsHit(const Point& Obj1, const Segment& Obj2)noexcept;
bool IsHit(const Point& Obj1, const Rect& Obj2)noexcept;
bool IsHit(const Point& Obj1, const Capsule& Obj2)noexcept;

bool IsHit(const Circle& Obj1, const Point& Obj2)noexcept;
bool IsHit(const Circle& Obj1, const Circle& Obj2)noexcept;
bool IsHit(const Circle& Obj1, const Segment& Obj2)noexcept;
bool IsHit(const Circle& Obj1, const Rect& Obj2)noexcept;
bool IsHit(const Circle& Obj1, const Capsule& Obj2)noexcept;

bool IsHit(const Segment& Obj1, const Point& Obj2)noexcept;
bool IsHit(const Segment& Obj1, const Circle& Obj2)noexcept;
bool IsHit(const Segment& Obj1, const Segment& Obj2)noexcept;
bool IsHit(const Segment& Obj1, const Rect& Obj2)noexcept;
bool IsHit(const Segment& Obj1, const Capsule& Obj2)noexcept;

bool IsHit(const Rect& Obj1, const Point& Obj2)noexcept;
bool IsHit(const Rect& Obj1, const Circle& Obj2)noexcept;
bool IsHit(const Rect& Obj1, const Segment& Obj2)noexcept;
bool IsHit(const Rect& Obj1, const Rect& Obj2)noexcept;
bool IsHit(const Rect& Obj1, const Capsule& Obj2)noexcept;

bool IsHit(const Capsule& Obj1, const Point& Obj2) noexcept;
bool IsHit(const Capsule& Obj1, const Circle& Obj2) noexcept;
bool IsHit(const Capsule& Obj1, const Segment& Obj2)noexcept;
bool IsHit(const Capsule& Obj1, const Rect& Obj2) noexcept;
bool IsHit(const Capsule& Obj1, const Capsule& Obj2)noexcept;
