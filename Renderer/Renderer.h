#pragma once 

#include "World.h"
#include "Picture.h"

namespace renderer {
class Renderer {
public:
    Picture make(const World& world, size_t camera_id);
    
private:
    Picture make_triangle(const World& world, size_t camera_id, const Triangle& triangle);
    void clipping(const World& world, size_t camera_id, const Triangle& triangle);
};
} // namespace renderer
