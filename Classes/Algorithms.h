#ifndef RETRY_ALGORITHMS
#define RETRY_ALGORITHMS

#include "cocos2d.h"


namespace Retry
{
using cocos2d::Vec2;

float clamp(float v, float lo, float hi);

long convertVec2ToLong(const Vec2 &v);

Vec2 convertLongToVec2(const long &l);

}


#endif
