#pragma once

#include "Eigen/Dense"
#include "Primitives.h"

namespace renderer {
class Camera {
public:
    Camera();
    void ChangePosition(const Point& new_position);
private:
    Point position_;
};

} // namespace renderer
