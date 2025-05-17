#pragma once

#include "Eigen/Dense"
#include "Primitives.h"

namespace renderer {

class Camera {
public:
    using Matrix3d = Eigen::Matrix3d;
    using Matrix4d = Eigen::Matrix4d;
    using Vector3d = Eigen::Vector3d;
    using Vector4d = Eigen::Vector4d;
    using Matrix34d = Eigen::Matrix<double, 3, 4>;
    using Matrix54d = Eigen::Matrix<double, 5, 4>;

    Camera(int width = 800, int height = 600,
           const Vector3d& position = Vector3d(0, 0, 0),
           const Vector3d& target = Vector3d(0, 0, -1));

    void changePosition(const Vector3d& new_position);
    void rotate(double yaw, double pitch);

    const Vector3d& getPosition() const { return position_; }
    const Vector3d& getDirection() const { return direction_; }
    const Vector3d& getUp() const { return up_; }
    const Vector3d& getRight() const { return right_; }

    Matrix3d getViewMatrix() const;
    Matrix4d getFullViewMatrix() const;
    Matrix3d getProjectionMatrix(double fov, double aspect, double near, double far) const;
    Matrix4d getFullProjectionMatrix() const;

    Matrix34d applyPerspectiveTransformation(const Matrix34d& vertices) const;
    Vector4d applyInversePerspectiveTransformation(const Vector4d& vec) const;
    const Matrix54d& getClippingPlanes() const;

    void clipping(const Triangle& triangle, const Vector3d& plane);

private:
    void updateVectors();
    void initPerspective();
    void initPlanes();
    void initConstants(int width, int height);

    Vector3d position_;
    Vector3d direction_;
    Vector3d up_;
    Vector3d right_;

    double yaw_ = -90.0;
    double pitch_ = 0.0;

    static constexpr double kDefaultNearPlaneDistance = 2.0;
    Matrix54d planes_;
    double r_, l_, t_, b_;
    Matrix4d perspective_matrix_ = Matrix4d::Zero();
    Matrix4d inverse_prespective_matrix_ = Matrix4d::Zero();
    double near_plane_distance_ = kDefaultNearPlaneDistance;
    double fov_ = -1;
    int width_;
    int height_;
};

}  // namespace renderer
