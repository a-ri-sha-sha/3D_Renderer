#include "World.h"
#include <cassert>

namespace renderer {

World::World(int width, int height) {
    cameras_.emplace_back();
}

void World::moveCamera(size_t camera_id, const Vector3d& direction, double speed) {
    assert(camera_id < cameras_.size() &&
           "moveCamera: camera_id should be less than count of cameras");
    Vector3d new_position = cameras_[camera_id].getPosition() + direction * speed;
    cameras_[camera_id].changePosition(new_position);
}

void World::rotateCamera(size_t camera_id, double yaw, double pitch) {
    assert(camera_id < cameras_.size() &&
           "rotateCamera: camera_id should be less than count of cameras");
    cameras_[camera_id].rotate(yaw, pitch);
}

size_t World::addCamera(const Vector3d& position, const Vector3d& target) {
    cameras_.emplace_back(800, 600, position, target);
    return cameras_.size() - 1;
}

const Camera& World::getCamera(size_t camera_id) const {
    assert(camera_id < cameras_.size());
    return cameras_[camera_id];
}

void World::addObject(const Object& object) {
    objects_.push_back(object);
    object_holders_.emplace_back(object);
}

void World::addObject(const Object& object, const Vector3d& position) {
    objects_.push_back(object);
    object_holders_.emplace_back(object, position);
}

const std::vector<Object>& World::getObjects() const {
    return objects_;
}

void World::clear() {
    objects_.clear();
    object_holders_.clear();
    cameras_.clear();
}

const Vector3d& World::getCameraPosition(size_t camera_id) const {
    assert(camera_id < cameras_.size());
    return cameras_[camera_id].getPosition();
}

Quaterniond World::getCameraRotation(size_t camera_id) const {
    assert(camera_id < cameras_.size());

    const Camera& camera = cameras_[camera_id];

    Vector3d forward = camera.getDirection();
    Vector3d up = camera.getUp();
    Vector3d right = camera.getRight();

    Eigen::Matrix3d rot_matrix;
    rot_matrix.col(0) = right;
    rot_matrix.col(1) = up;
    rot_matrix.col(2) = forward;

    return Quaterniond(rot_matrix);
}

void World::setCameraPosition(size_t camera_id, const Vector3d& position) {
    assert(camera_id < cameras_.size());
    cameras_[camera_id].changePosition(position);
}

void World::setCameraRotation(size_t camera_id, const Quaterniond& rotation) {
    assert(camera_id < cameras_.size());

    Eigen::Matrix3d rot_matrix = rotation.toRotationMatrix();

    Eigen::Vector3d euler = rot_matrix.eulerAngles(0, 1, 2);

    cameras_[camera_id].rotate(euler[1] * 180.0 / M_PI, euler[0] * 180.0 / M_PI);
}

World::ObjectHolder::ObjectHolder(const Object& obj, const Vector3d& pos, const Quaterniond& rot)
    : object_(obj), position_(pos), rotation_(rot) {
}

const Object& World::ObjectHolder::getObject() const {
    return object_;
}

const Vector3d& World::ObjectHolder::getPosition() const {
    return position_;
}
const Quaterniond& World::ObjectHolder::getRotation() const {
    return rotation_;
}

void World::ObjectHolder::setPosition(const Vector3d& pos) {
    position_ = pos;
}
void World::ObjectHolder::setRotation(const Quaterniond& rot) {
    rotation_ = rot;
}

const std::vector<World::ObjectHolder>& World::getObjectHolders() const {
    return object_holders_;
}
std::vector<World::ObjectHolder>& World::getObjectHolders() {
    return object_holders_;
}

}  // namespace renderer
