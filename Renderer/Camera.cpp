#include "Camera.h"
#include <cmath>

namespace renderer {

Camera::Camera(const Vector3d& position, const Vector3d& target) : position_(position) {
    direction_ = (target - position).normalized();
    yaw_ = std::atan2(direction_.y(), direction_.x()) * 180.0 / M_PI;
    pitch_ = std::asin(direction_.z()) * 180.0 / M_PI;
    updateVectors();
}

void Camera::changePosition(const Vector3d& new_position) {
    position_ = new_position;
}

void Camera::rotate(double yaw, double pitch) {
    yaw_ += yaw;
    pitch_ += pitch;

    if (pitch_ > 89.0) {
        pitch_ = 89.0;
    }
    if (pitch_ < -89.0) {
        pitch_ = -89.0;
    }

    updateVectors();
}

void Camera::updateVectors() {
    double yaw_rad = yaw_ * M_PI / 180.0;
    double pitch_rad = pitch_ * M_PI / 180.0;

    direction_ = Vector3d(std::cos(yaw_rad) * std::cos(pitch_rad),
                          std::sin(yaw_rad) * std::cos(pitch_rad), std::sin(pitch_rad))
                     .normalized();

    right_ = Vector3d(0, 0, 1).cross(direction_).normalized();
    up_ = direction_.cross(right_).normalized();
}

void Camera::clipping(const Triangle& triangle, const Vector3d& plane) {
    // Not implemented
}

Matrix3d Camera::getViewMatrix() const {
    Matrix3d view = Matrix3d::Identity();

    view.col(0) = right_;
    view.col(1) = up_;
    view.col(2) = -direction_;

    return view;
}

Matrix3d Camera::getProjectionMatrix(double fov, double aspect, double near, double far) const {
    Matrix3d projection = Matrix3d::Zero();

    double tan_half_fov = std::tan(fov * 0.5 * M_PI / 180.0);

    projection(0, 0) = 1.0 / (aspect * tan_half_fov);
    projection(1, 1) = 1.0 / tan_half_fov;
    projection(2, 2) = -(far + near) / (far - near);
    projection(2, 0) = 0.0;
    projection(2, 1) = 0.0;
    projection(0, 2) = 0.0;
    projection(1, 2) = 0.0;

    return projection;
}

}  // namespace renderer
