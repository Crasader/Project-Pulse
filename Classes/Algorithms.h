#ifndef RETRY_ALGORITHMS
#define RETRY_ALGORITHMS

#include "cocos2d.h"


namespace Retry
{

float clamp(const float& v, const float& lo, const float& hi);

int sign(const float& n);

float lerp(const float& p0, const float& p1, const float& t);

long convertVec2ToLong(const cocos2d::Vec2 &v);

cocos2d::Vec2 convertLongToVec2(const long &l);

}


#endif
