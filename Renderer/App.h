#pragma once

#include "World.h"
#include "Renderer.h"
#include "Drawer.h"

namespace renderer {

class Application {
public:
    Application();
    void run();

    void initializeBasicScene();
    void initializeSphereScene();

    bool loadObjFile(const std::string& filename);

private:
    void handleInput();

    World world_;
    Renderer renderer_;
    Drawer drawer_;

    size_t camera_id = 0;

    enum class SelectionMode { Camera, Object };

    SelectionMode selection_mode_ = SelectionMode::Camera;
    size_t selected_object_id_ = 0;
};

}  // namespace renderer
