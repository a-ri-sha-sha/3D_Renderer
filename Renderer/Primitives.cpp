#include "Primitives.h"

namespace renderer {

bool isPointInFrontOfPlane(const Vector3d& point, const Vector3d& plane_normal,
                           const Vector3d& plane_point) {
    Vector3d v = point - plane_point;
    return v.dot(plane_normal) >= 0;
}

Vector3d linePlaneIntersection(const Vector3d& line_start, const Vector3d& line_end,
                               const Vector3d& plane_normal, const Vector3d& plane_point) {
    Vector3d line_dir = line_end - line_start;
    double t = (plane_point - line_start).dot(plane_normal) / line_dir.dot(plane_normal);
    return line_start + t * line_dir;
}

}  // namespace renderer
