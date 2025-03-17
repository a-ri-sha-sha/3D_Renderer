#pragma once 

#include "World.h"
#include "Renderer.h"
#include "Drawer.h"

namespace renderer {
class Application {
public:
    Application();
    void run();
private:
    World world_;
    Renderer renderer_;
    Drawer drawer_;
};
} // namespace renderer
