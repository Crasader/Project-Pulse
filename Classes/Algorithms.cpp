#include "Algorithms.h"
#include <algorithm>

namespace Retry
{

float clamp(float v, float lo, float hi) { if (hi < lo) std::swap(hi, lo); return v < lo ? lo : v > hi ? hi : v; }

}