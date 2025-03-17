#pragma once 

#include "World.h"
#include "Picture.h"

namespace renderer {
class Renderer {
public:
    Picture make(const World& world);
    
private:
    // ???
};
} // namespace renderer
