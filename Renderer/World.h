#pragma once

#include "Primitives.h"
#include "Camera.h"

#include <vector>

namespace renderer {

class World {
public:
    void moveCamera(size_t camera_id, const Vector3d& direction, double speed);
    void rotateCamera(size_t camera_id, double yaw, double pitch);
    size_t addCamera(const Vector3d& position, const Vector3d& target);
    const Camera& getCamera(size_t camera_id) const;

    void addObject(const Object& object);
    const std::vector<Object>& getObjects() const;

    void clear();

private:
    std::vector<Object> objects_;
    std::vector<Camera> cameras_;
};

}  // namespace renderer
