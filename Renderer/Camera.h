#pragma once

#include "Eigen/Dense"
#include "Primitives.h"

namespace renderer {
class Camera {
public:
    Camera() = default;
    void changePosition(const Vector3d& new_position);
    void clipping(const Triangle& triangle, const Vector3d& plane);
private:
    Matrix3d A_;
    Vector3d p_;
};

} // namespace renderer
