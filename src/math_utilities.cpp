#include "../include/math_utilities.h"

ysVector c_adv::vector3d(const ysVector &b) {
    return ysMath::Mask(b, ysMath::Constants::MaskOffW);
}

float c_adv::distance(const ysVector &a, const ysVector &b) {
    ysVector d = ysMath::Sub(a, b);
    return ysMath::GetScalar(ysMath::Magnitude(vector3d(d)));
}

float c_adv::length(const ysVector &v) {
    return ysMath::GetScalar(ysMath::Magnitude(v));
}

float c_adv::length2(const ysVector &v) {
    return ysMath::GetScalar(ysMath::MagnitudeSquared3(v));
}

bool c_adv::inRange(const ysVector &a, const ysVector &b, float distance) {
    return inRangeSq(a, b, distance * distance);
}

bool c_adv::inRangeSq(const ysVector &a, const ysVector &b, float distance2) {
    ysVector d = ysMath::Sub(a, b);
    return ysMath::GetScalar(ysMath::MagnitudeSquared3(d)) <= distance2;
}

bool c_adv::getDirection(const ysVector &pos, const ysVector &target, ysVector *out) {
    ysVector d = ysMath::Sub(target, pos);
    ysVector mag2 = ysMath::MagnitudeSquared3(d);

    if (ysMath::GetScalar(mag2) <= 0.001f) return false;
    else {
        *out = vector3d(ysMath::Div(d, ysMath::Sqrt(mag2)));
        return true;
    }
}
