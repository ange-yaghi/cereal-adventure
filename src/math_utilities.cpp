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

uint64_t c_adv::bitwiseInterleaveWithZeros(uint32_t input) {
    uint64_t word = input;
    word = (word ^ (word << 16)) & 0x0000ffff0000ffff;
    word = (word ^ (word << 8)) & 0x00ff00ff00ff00ff;
    word = (word ^ (word << 4)) & 0x0f0f0f0f0f0f0f0f;
    word = (word ^ (word << 2)) & 0x3333333333333333;
    word = (word ^ (word << 1)) & 0x5555555555555555;
    return word;
}

uint64_t c_adv::bitwiseInterleave(uint32_t x, uint32_t y) {
    return bitwiseInterleaveWithZeros(x) | (bitwiseInterleaveWithZeros(y) << 1);
}

uint64_t c_adv::bitwiseReverse(uint64_t input, int bits) {
    uint64_t result = 0;
    for (int i = 0; i < bits; ++i) {
        const uint64_t bit = input & 0x1;
        result |= bit << (bits - i - 1);

        input >>= 1;
    }

    return result;
}
