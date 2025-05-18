#include "Camera.h"

#include <cmath>
#include <cassert>

namespace renderer {

Camera::Camera(int width, int height, const Vector3d& position, const Vector3d& target)
    : position_(position), width_(width), height_(height) {
    direction_ = (target - position).normalized();
    yaw_ = std::atan2(direction_.y(), direction_.x()) * 180.0 / M_PI;
    pitch_ = std::asin(direction_.z()) * 180.0 / M_PI;
    updateVectors();

    initConstants(width, height);
    initPerspective();
    initPlanes();
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

const Vector3d& Camera::getPosition() const {
    return position_;
}

const Vector3d& Camera::getDirection() const {
    return direction_;
}

const Vector3d& Camera::getUp() const {
    return up_;
}

const Vector3d& Camera::getRight() const {
    return right_;
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

Matrix3d Camera::getViewMatrix() const {
    Matrix3d view = Matrix3d::Identity();

    view.col(0) = right_;
    view.col(1) = up_;
    view.col(2) = -direction_;

    return view;
}

Matrix4d Camera::getFullViewMatrix() const {
    Matrix4d view = Matrix4d::Identity();

    view.block<3, 3>(0, 0) = getViewMatrix();
    view.block<3, 1>(0, 3) = -getViewMatrix() * position_;

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

Matrix4d Camera::getFullProjectionMatrix() const {
    return perspective_matrix_;
}

void Camera::initConstants(int width, int height) {
    double dwidth = static_cast<double>(width);
    double dheight = static_cast<double>(height);
    assert("correct width and height given to camera" && dwidth > 0 && dheight > 0);
    double aspect_ratio = dheight / dwidth;
    l_ = -1;
    r_ = 1;
    b_ = -aspect_ratio;
    t_ = -b_;
    fov_ = 2.0 * std::atan(1 / near_plane_distance_) * 180.0 / M_PI;
}

void Camera::initPerspective() {
    perspective_matrix_ = Matrix4d::Zero();
    perspective_matrix_(0, 0) = (2.0 * near_plane_distance_ / (r_ - l_));
    perspective_matrix_(0, 2) = ((r_ + l_) / (r_ - l_));

    perspective_matrix_(1, 1) = (2 * near_plane_distance_ / (t_ - b_));
    perspective_matrix_(1, 2) = ((t_ + b_) / (t_ - b_));
    perspective_matrix_(2, 2) = -1;
    perspective_matrix_(2, 3) = -2.0 * near_plane_distance_;
    perspective_matrix_(3, 2) = -1;
    inverse_prespective_matrix_ = perspective_matrix_.inverse();
}
void Camera::initPlanes() {
    double focal_length = near_plane_distance_;
    double x_norm_coef = std::sqrt(focal_length * focal_length + 1);
    double y_norm_coef = std::sqrt(focal_length * focal_length + t_ * t_);

    planes_ = Matrix54d::Zero();
    planes_ << 0, 0, -1.0, near_plane_distance_, focal_length / x_norm_coef, 0, -1.0 / x_norm_coef,
        0, -focal_length / x_norm_coef, 0, -1.0 / x_norm_coef, 0, 0, focal_length / y_norm_coef,
        -t_ / y_norm_coef, 0, 0, -focal_length / y_norm_coef, -t_ / y_norm_coef, 0;
}

Matrix34d Camera::applyPerspectiveTransformation(const Matrix34d& vertices) const {
    Matrix34d ans = vertices;
    ans = (perspective_matrix_ * ans.transpose()).transpose();
    for (int i = 0; i < ans.rows(); ++i) {
        if (std::abs(ans.row(i).w()) > 1e-6) {
            ans.row(i).topLeftCorner<1, 3>() /= ans.row(i).w();
        }
    }
    return ans;
}

Vector4d Camera::applyInversePerspectiveTransformation(const Vector4d& vec) const {
    Vector4d ans = vec;
    ans.topLeftCorner<3, 1>() *= ans.w();
    return inverse_prespective_matrix_ * ans;
}

const Matrix54d& Camera::getClippingPlanes() const {
    return planes_;
}

}  // namespace renderer
