#pragma once

#include "Eigen/Dense"
#include "Primitives.h"

namespace renderer {

class Camera {
public:
    Camera(const Vector3d& position = Vector3d(0, 0, 0),
           const Vector3d& target = Vector3d(0, 0, -1));

    void changePosition(const Vector3d& new_position);
    void rotate(double yaw, double pitch);

    const Vector3d& getPosition() const {
        return position_;
    }
    const Vector3d& getDirection() const {
        return direction_;
    }
    const Vector3d& getUp() const {
        return up_;
    }
    const Vector3d& getRight() const {
        return right_;
    }

    void clipping(const Triangle& triangle, const Vector3d& plane);
    Matrix3d getViewMatrix() const;
    Matrix3d getProjectionMatrix(double fov, double aspect, double near, double far) const;

private:
    void updateVectors();

    Vector3d position_;
    Vector3d direction_;
    Vector3d up_;
    Vector3d right_;

    double yaw_ = -90.0;
    double pitch_ = 0.0;
};

}  // namespace renderer
