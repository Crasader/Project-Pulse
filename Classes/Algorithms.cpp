#include "Algorithms.h"
#include "cocos2d.h"
#include <algorithm>

namespace Retry {
using cocos2d::Vec2;

float clamp(const float& v, const float& lo, const float& hi) {
	if (lo < hi)
		return v < lo ? lo : v > hi ? hi : v;
	else
		return v < hi ? hi : v > lo ? lo : v;
}

int sign(const float& n) {
	return n != 0 ? n / abs(n) : 0;
}

float lerp(const float& p0, const float& p1, const float& t) {
	return p1 * t + p0 * (1 - clamp(t, 0, 1));
}

long convertVec2ToLong(const Vec2 &v) {
	return ((long(v.x) & 0xFFFF) << 0x10) | (long(v.y) & 0xFFFF);
}

Vec2 convertLongToVec2(const long &l) {
	return Vec2((l & 0xFFFF0000) >> 0x10, l & 0xFFFF);
}

}