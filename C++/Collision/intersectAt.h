#pragma once

#include <vector>
#include "../Primitive/Primitive.h"

std::vector<Point> IntersectAt(const HalfLine& obj1, const Circle& obj2);
std::vector<Point> IntersectAt(const HalfLine& obj1, const Segment& obj2);
std::vector<Point> IntersectAt(const HalfLine& obj1, const Rect& obj2);
