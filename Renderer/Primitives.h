#pragma once

#include "Linalg.h"

#include <vector>

namespace renderer {
struct Ray {
    Vector3d ray;
};

struct Point {
    Vector3d coordinates;
};

struct Triangle {
    Matrix3d points;
};

struct Object {
    std::vector<Triangle> trisngles;
};
} // namespace renderer
