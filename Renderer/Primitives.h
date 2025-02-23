#pragma once

#include "Eigen/Dense"

namespace renderer {
struct Ray {
    double x;
    double y;
    double z;
};

struct Point {
    double x;
    double y;
    double z;
    Ray direction;
};

struct Triangle {
    Eigen::Vector3<Point> points;
};

} // namespace renderer
