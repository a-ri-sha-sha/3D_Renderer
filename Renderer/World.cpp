#include "World.h"

#include <algorithm>

namespace renderer {

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
    cameras_.emplace_back(position, target);
    return cameras_.size() - 1;
}

const Camera& World::getCamera(size_t camera_id) const {
    return cameras_[camera_id];
}

void World::addObject(const Object& object) {
    objects_.push_back(object);
}

const std::vector<Object>& World::getObjects() const {
    return objects_;
}

void World::clear() {
    objects_.clear();
    cameras_.clear();
}

}  // namespace renderer
