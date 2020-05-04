#ifndef CEREAL_ADVENTURE_MATH_UTILITIES_H
#define CEREAL_ADVENTURE_MATH_UTILITIES_H

#include "delta.h"

namespace c_adv {

    ysVector vector3d(const ysVector &b);
    float distance(const ysVector &a, const ysVector &b);
    float length(const ysVector &v);
    float length2(const ysVector &v);
    bool inRange(const ysVector &a, const ysVector &b, float distance);
    bool inRangeSq(const ysVector &a, const ysVector &b, float distance2);
    bool getDirection(const ysVector &pos, const ysVector &target, ysVector *out);

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_MATH_UTILITIES_H */