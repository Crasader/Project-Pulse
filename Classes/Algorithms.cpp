#include "Algorithms.h"
#include "cocos2d.h"
#include <algorithm>

namespace Retry
{

using cocos2d::Vec2;


float clamp(float v, float lo, float hi) { if (hi < lo) std::swap(hi, lo); return v < lo ? lo : v > hi ? hi : v; }

long convertVec2ToLong(const Vec2 &v)
{
	return ((long(v.x) & 0xFFFF) << 0x10) | (long(v.y) & 0xFFFF);
}

Vec2 convertLongToVec2(const long &l)
{
	return Vec2((l & 0xFFFF0000) >> 0x10, l & 0xFFFF);
}

}