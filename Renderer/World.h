#pragma once

#include "Linalg.h"
#include "Primitives.h"
#include "Camera.h"
#include <vector>

namespace renderer {

class World {
public:
    template <typename Iterator>
    class Iterable {
    public:
        Iterable(Iterator begin, Iterator end) : begin_(begin), end_(end) {
        }
        Iterator begin() {
            return begin_;
        }
        Iterator end() {
            return end_;
        }

    private:
        Iterator begin_;
        Iterator end_;
    };

    World(int width = 800, int height = 600);

    void moveCamera(size_t camera_id, const Vector3d& direction, double speed);
    void rotateCamera(size_t camera_id, double yaw, double pitch);
    size_t addCamera(const Vector3d& position, const Vector3d& target);
    const Camera& getCamera(size_t camera_id) const;

    void addObject(const Object& object);
    const std::vector<Object>& getObjects() const;
    void clear();

    void addObject(const Object& object, const Vector3d& position);

    auto GetObjectsIterable() const {
        return Iterable(objects_.begin(), objects_.end());
    };

    const Vector3d& getCameraPosition(size_t camera_id) const;
    Quaterniond getCameraRotation(size_t camera_id) const;

    void setCameraPosition(size_t camera_id, const Vector3d& position);
    void setCameraRotation(size_t camera_id, const Quaterniond& rotation);

    static Vector3d getOrigin() {
        return Vector3d(0, 0, 0);
    }

    class ObjectHolder {
    public:
        ObjectHolder(const Object& obj, const Vector3d& pos = getOrigin(),
                     const Quaterniond& rot = Quaterniond::Identity());

        const Object& getObject() const;
        const Vector3d& getPosition() const;
        const Quaterniond& getRotation() const;

        void setPosition(const Vector3d& pos);
        void setRotation(const Quaterniond& rot);

    private:
        Object object_;
        Vector3d position_ = getOrigin();
        Quaterniond rotation_ = Quaterniond::Identity();
    };

    const std::vector<ObjectHolder>& getObjectHolders() const;
    std::vector<ObjectHolder>& getObjectHolders();

private:
    std::vector<Object> objects_;
    std::vector<ObjectHolder> object_holders_;
    std::vector<Camera> cameras_;
};

}  // namespace renderer
