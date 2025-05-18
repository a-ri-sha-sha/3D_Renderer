#include "Primitives.h"

#include <cmath>
#include <list>

namespace renderer {

Triangle::Triangle(const Matrix3d& p) : points(p) {
}

Vector3d Triangle::getPointFromBarycentric(const Vector3d& barycentric) const {
    return Vector3d(points.row(0) * barycentric.x() + points.row(1) * barycentric.y() +
                    points.row(2) * barycentric.z());
}

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

Object createCube(double size) {
    Object cube;
    double half = size / 2.0;

    Matrix3d front1;
    front1 << -half, -half, half, half, -half, half, half, half, half;
    cube.addTriangle(front1);

    Matrix3d front2;
    front2 << -half, -half, half, half, half, half, -half, half, half;
    cube.addTriangle(front2);

    Matrix3d back1;
    back1 << -half, -half, -half, half, -half, -half, half, half, -half;
    cube.addTriangle(back1);

    Matrix3d back2;
    back2 << -half, -half, -half, half, half, -half, -half, half, -half;
    cube.addTriangle(back2);

    Matrix3d top1;
    top1 << -half, half, -half, half, half, -half, half, half, half;
    cube.addTriangle(top1);

    Matrix3d top2;
    top2 << -half, half, -half, half, half, half, -half, half, half;
    cube.addTriangle(top2);

    Matrix3d bottom1;
    bottom1 << -half, -half, -half, half, -half, -half, half, -half, half;
    cube.addTriangle(bottom1);

    Matrix3d bottom2;
    bottom2 << -half, -half, -half, half, -half, half, -half, -half, half;
    cube.addTriangle(bottom2);

    Matrix3d left1;
    left1 << -half, -half, -half, -half, -half, half, -half, half, half;
    cube.addTriangle(left1);

    Matrix3d left2;
    left2 << -half, -half, -half, -half, half, half, -half, half, -half;
    cube.addTriangle(left2);

    Matrix3d right1;
    right1 << half, -half, -half, half, -half, half, half, half, half;
    cube.addTriangle(right1);

    Matrix3d right2;
    right2 << half, -half, -half, half, half, half, half, half, -half;
    cube.addTriangle(right2);

    return cube;
}

std::list<Matrix3d> createIcosahedron() {
    std::list<Matrix3d> triangles;

    double phi = (1.0 + std::sqrt(5.0)) / 2.0;
    double a = 1.0;
    double b = 1.0 / phi;

    Vector3d v1(0, b, -a);
    Vector3d v2(b, a, 0);
    Vector3d v3(-b, a, 0);
    Vector3d v4(0, b, a);
    Vector3d v5(0, -b, a);
    Vector3d v6(-a, 0, b);
    Vector3d v7(0, -b, -a);
    Vector3d v8(a, 0, -b);
    Vector3d v9(a, 0, b);
    Vector3d v10(-a, 0, -b);
    Vector3d v11(b, -a, 0);
    Vector3d v12(-b, -a, 0);

    auto makeTriangle = [](const Vector3d& a, const Vector3d& b, const Vector3d& c) {
        Matrix3d m;
        m.row(0) = a;
        m.row(1) = b;
        m.row(2) = c;
        return m;
    };

    triangles.push_back(makeTriangle(v3, v2, v1));
    triangles.push_back(makeTriangle(v2, v3, v4));
    triangles.push_back(makeTriangle(v6, v5, v4));
    triangles.push_back(makeTriangle(v5, v9, v4));
    triangles.push_back(makeTriangle(v8, v7, v1));
    triangles.push_back(makeTriangle(v7, v10, v1));
    triangles.push_back(makeTriangle(v12, v11, v5));
    triangles.push_back(makeTriangle(v11, v12, v7));
    triangles.push_back(makeTriangle(v10, v6, v3));
    triangles.push_back(makeTriangle(v6, v10, v12));
    triangles.push_back(makeTriangle(v9, v8, v2));
    triangles.push_back(makeTriangle(v8, v9, v11));
    triangles.push_back(makeTriangle(v3, v6, v4));
    triangles.push_back(makeTriangle(v9, v2, v4));
    triangles.push_back(makeTriangle(v10, v3, v1));
    triangles.push_back(makeTriangle(v2, v8, v1));
    triangles.push_back(makeTriangle(v12, v10, v7));
    triangles.push_back(makeTriangle(v8, v11, v7));
    triangles.push_back(makeTriangle(v6, v12, v5));
    triangles.push_back(makeTriangle(v11, v9, v5));

    for (auto& triangle : triangles) {
        for (int i = 0; i < 3; ++i) {
            triangle.row(i).normalize();
        }
    }

    return triangles;
}

void subdivideTriangle(const Matrix3d& triangle, std::list<Matrix3d>* triangles) {
    Vector3d mid01 = (triangle.row(0) + triangle.row(1)) / 2.0;
    Vector3d mid12 = (triangle.row(1) + triangle.row(2)) / 2.0;
    Vector3d mid20 = (triangle.row(2) + triangle.row(0)) / 2.0;

    mid01.normalize();
    mid12.normalize();
    mid20.normalize();

    Matrix3d t1;
    t1.row(0) = triangle.row(0);
    t1.row(1) = mid01;
    t1.row(2) = mid20;
    triangles->push_back(t1);

    Matrix3d t2;
    t2.row(0) = mid01;
    t2.row(1) = triangle.row(1);
    t2.row(2) = mid12;
    triangles->push_back(t2);

    Matrix3d t3;
    t3.row(0) = mid01;
    t3.row(1) = mid12;
    t3.row(2) = mid20;
    triangles->push_back(t3);

    Matrix3d t4;
    t4.row(0) = mid20;
    t4.row(1) = mid12;
    t4.row(2) = triangle.row(2);
    triangles->push_back(t4);
}

Object createSphere(double radius, int detail_level) {
    Object sphere;

    std::list<Matrix3d> triangles = createIcosahedron();

    for (int i = 0; i < detail_level; ++i) {
        std::list<Matrix3d> result;
        for (const Matrix3d& triangle : triangles) {
            subdivideTriangle(triangle, &result);
        }
        triangles = std::move(result);
    }

    for (const Matrix3d& triangle : triangles) {
        sphere.addTriangle(triangle * radius);
    }

    return sphere;
}

Object createPlane(double size) {
    Object plane;
    double half = size / 2.0;

    Matrix3d t1;
    t1 << -half, 0, -half, half, 0, -half, half, 0, half;
    plane.addTriangle(t1);

    Matrix3d t2;
    t2 << -half, 0, -half, half, 0, half, -half, 0, half;
    plane.addTriangle(t2);

    return plane;
}

Object createPyramid(double base_size, double height) {
    Object pyramid;
    double half = base_size / 2.0;
    double half_height = height / 2.0;

    Vector3d apex(0, half_height, 0);
    Vector3d b1(-half, -half_height, -half);
    Vector3d b2(half, -half_height, -half);
    Vector3d b3(half, -half_height, half);
    Vector3d b4(-half, -half_height, half);

    Matrix3d f1;
    f1.row(0) = apex;
    f1.row(1) = b1;
    f1.row(2) = b2;
    pyramid.addTriangle(f1);

    Matrix3d f2;
    f2.row(0) = apex;
    f2.row(1) = b2;
    f2.row(2) = b3;
    pyramid.addTriangle(f2);

    Matrix3d f3;
    f3.row(0) = apex;
    f3.row(1) = b3;
    f3.row(2) = b4;
    pyramid.addTriangle(f3);

    Matrix3d f4;
    f4.row(0) = apex;
    f4.row(1) = b4;
    f4.row(2) = b1;
    pyramid.addTriangle(f4);

    Matrix3d base1;
    base1.row(0) = b1;
    base1.row(1) = b3;
    base1.row(2) = b2;
    pyramid.addTriangle(base1);

    Matrix3d base2;
    base2.row(0) = b1;
    base2.row(1) = b4;
    base2.row(2) = b3;
    pyramid.addTriangle(base2);

    return pyramid;
}

}  // namespace renderer
