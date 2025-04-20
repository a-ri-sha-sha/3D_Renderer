#pragma once 

#include "Primitives.h"
#include "Camera.h"

#include <vector>

namespace renderer {
class World {
public:
    void moveCamera(/*parameters*/);
private:
    std::vector<Object> objects_;
    Camera camera_;
};

} // namespace renderer
