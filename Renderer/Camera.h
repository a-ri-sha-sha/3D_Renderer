#pragma once

#include "Eigen/Dense"
#include "Primitives.h"

namespace renderer {
class Camera {
public:
    Camera() = default;
    void ChangePosition(const Point& new_position);
private:
    Point position_;
    Matrix3d A_;
    Vector3d p_;
};

} // namespace renderer
