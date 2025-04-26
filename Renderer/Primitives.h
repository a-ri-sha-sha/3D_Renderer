#pragma once

#include "Linalg.h"

#include <vector>

namespace renderer {

struct Triangle {
    Matrix3d points;
};

struct Object {
    std::vector<Triangle> triangles;
};

bool isPointInFrontOfPlane(const Vector3d& point, const Vector3d& plane_normal,
                           const Vector3d& plane_point);
Vector3d linePlaneIntersection(const Vector3d& line_start, const Vector3d& line_end,
                               const Vector3d& plane_normal, const Vector3d& plane_point);

}  // namespace renderer
