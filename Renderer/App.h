#pragma once

#include "Camera.h"
#include "Drawer.h"
#include "Primitives.h"
#include "Renderer.h"
#include "World.h"

namespace renderer {

class Application {
public:
    Application();
    void run();

private:
    enum class SelectionMode {
        Camera,
        Object
    };

    void handleInput();
    void initializeBasicScene();
    void initializeSphereScene();
    
    bool loadObjFile(const std::string& filename);
    void loadObjDirectory(const std::string& directory);

    Drawer drawer_;
    Renderer renderer_;
    World world_;
    size_t camera_id;
    SelectionMode selection_mode_ = SelectionMode::Camera;
    size_t selected_object_id_ = 0;
};

}  // namespace renderer
