#pragma once

#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace renderer {

using Vector3d = Eigen::Vector3d;
using Vector4d = Eigen::Vector4d;
using Matrix3d = Eigen::Matrix3d;
using Matrix4d = Eigen::Matrix4d;
using MatrixXd = Eigen::MatrixXd;
using Matrix34d = Eigen::Matrix<double, 3, 4>;
using Matrix54d = Eigen::Matrix<double, 5, 4>;
using Quaterniond = Eigen::Quaterniond;
using Index = Eigen::Index;

}  // namespace renderer
