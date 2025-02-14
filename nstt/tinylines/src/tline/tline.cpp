#include "tline.hpp"
#include <assert.h>

bool Line::intersects(const Line& other) const {
    if (A == 0 && B == 0 || other.A == 0 && other.B == 0)
        return false;
    if (A == 0 && B != 0)
        return other.A != 0;
    if (A != 0 && B == 0)
        return other.B != 0;
    if (other.A == 0 || other.B == 0)
        return true;
    assert(A != 0 && B != 0 && other.A != 0 && other.B != 0);
    return A / B != other.A / other.B;
}

Line Line::getPerpendicular(const Point& p) const {
    return Line(this->B, -this->A, this->A * p.y - this->B * p.x);
}
