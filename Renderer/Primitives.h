#pragma once

#include "Linalg.h"
#include <vector>

namespace renderer {

struct Triangle {
    Matrix3d points;

    Triangle() = default;
    Triangle(const Matrix3d& p) : points(p) {}
    
    Vector3d getPointFromBarycentric(const Vector3d& barycentric) const {
        return Vector3d(
            points.row(0) * barycentric.x() +
            points.row(1) * barycentric.y() +
            points.row(2) * barycentric.z()
        );
    }
};

struct Object {
    std::vector<Triangle> triangles;
    
    void addTriangle(const Matrix3d& points) {
        triangles.emplace_back(points);
    }
    
    void addTriangle(const Triangle& triangle) {
        triangles.push_back(triangle);
    }
};

bool isPointInFrontOfPlane(const Vector3d& point, const Vector3d& plane_normal,
                           const Vector3d& plane_point);
Vector3d linePlaneIntersection(const Vector3d& line_start, const Vector3d& line_end,
                               const Vector3d& plane_normal, const Vector3d& plane_point);

Object createCube(double size = 1.0);
Object createSphere(double radius = 1.0, int detail_level = 2);
Object createPlane(double size = 1.0);
Object createPyramid(double base_size = 1.0, double height = 1.0);

}  // namespace renderer
