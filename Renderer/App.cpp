#include "App.h"
#include "Linalg.h"
#include "ObjLoader.h"
#include "Picture.h"

#include <iostream>
#include <filesystem>
#include <fstream>

namespace renderer {

Application::Application() {
    camera_id = world_.addCamera(Vector3d(0, 0, 5), Vector3d(0, 0, 0));
    initializeBasicScene();
}

void Application::run() {
    std::cout << "Entering main loop..." << std::endl;

    while (drawer_.isOpen()) {
        drawer_.processEvents();
        handleInput();

        std::cout << "Rendering frame..." << std::endl;
        Picture pic = renderer_.make(world_, camera_id);
        // Picture pic = renderer_.makeTestTriangle();
        std::cout << "Frame rendered. Drawing to screen..." << std::endl;
        drawer_.draw(pic);
        std::cout << "Frame drawn." << std::endl;
    }
}

void Application::handleInput() {
    const double move_speed = 0.1;
    const double rotate_speed = 1.0;

    static bool key1_processed = false;
    static bool key2_processed = false;
    static bool key3_processed = false;
    static bool key4_processed = false;
    static bool tab_processed = false;

    if (selection_mode_ == SelectionMode::Camera) {
        const Camera& camera = world_.getCamera(camera_id);

        if (drawer_.isKeyPressed(sf::Keyboard::W)) {
            world_.moveCamera(camera_id, camera.getDirection(), move_speed);
        }
        if (drawer_.isKeyPressed(sf::Keyboard::S)) {
            world_.moveCamera(camera_id, -camera.getDirection(), move_speed);
        }
        if (drawer_.isKeyPressed(sf::Keyboard::A)) {
            world_.moveCamera(camera_id, -camera.getRight(), move_speed);
        }
        if (drawer_.isKeyPressed(sf::Keyboard::D)) {
            world_.moveCamera(camera_id, camera.getRight(), move_speed);
        }
        if (drawer_.isKeyPressed(sf::Keyboard::Space)) {
            world_.moveCamera(camera_id, camera.getUp(), move_speed);
        }
        if (drawer_.isKeyPressed(sf::Keyboard::LShift)) {
            world_.moveCamera(camera_id, -camera.getUp(), move_speed);
        }

        static sf::Vector2i last_mouse_pos = drawer_.getMousePosition();
        sf::Vector2i current_mouse_pos = drawer_.getMousePosition();

        if (drawer_.isMouseButtonPressed(sf::Mouse::Right)) {
            int dx = current_mouse_pos.x - last_mouse_pos.x;
            int dy = current_mouse_pos.y - last_mouse_pos.y;

            world_.rotateCamera(camera_id, dx * rotate_speed, -dy * rotate_speed);
        }

        last_mouse_pos = current_mouse_pos;
    } else if (selection_mode_ == SelectionMode::Object && world_.getObjects().size() > 0) {
        /*
        if (drawer_.isKeyPressed(sf::Keyboard::W)) {
            Vector3d pos = world_.getObjectPosition(selected_object_id_);
            world_.setObjectPosition(selected_object_id_, pos + Vector3d(0, 0, -move_speed));
        }
        */
    }

    if (drawer_.isKeyPressed(sf::Keyboard::Num1)) {
        if (!key1_processed) {
            initializeBasicScene();
            key1_processed = true;
        }
    } else {
        key1_processed = false;
    }

    if (drawer_.isKeyPressed(sf::Keyboard::Num2)) {
        if (!key2_processed) {
            initializeSphereScene();
            key2_processed = true;
        }
    } else {
        key2_processed = false;
    }

    if (drawer_.isKeyPressed(sf::Keyboard::Num3)) {
        if (!key3_processed) {
            loadObjFile("../models/box.obj");
            key3_processed = true;
        }
    } else {
        key3_processed = false;
    }

    if (drawer_.isKeyPressed(sf::Keyboard::Num4)) {
        if (!key4_processed) {
            loadObjDirectory("../scene");
            key4_processed = true;
        }
    } else {
        key4_processed = false;
    }
    if (drawer_.isKeyPressed(sf::Keyboard::Tab)) {
        if (!tab_processed) {
            if (selection_mode_ == SelectionMode::Camera) {
                if (world_.getObjects().size() > 0) {
                    selection_mode_ = SelectionMode::Object;
                }
            } else {
                selection_mode_ = SelectionMode::Camera;
            }
            tab_processed = true;
        }
    } else {
        tab_processed = false;
    }
}

void Application::initializeBasicScene() {
    world_.clear();

    camera_id = world_.addCamera(Vector3d(0, 0, 0), Vector3d(0, 0, -1));

    Object triangle;
    Matrix3d points;
    points << -1.0, 0.0, -3.0, 1.0, 0.0, -3.0, 0.0, 1.0, -3.0;
    triangle.addTriangle(points);

    world_.addObject(triangle);

    selection_mode_ = SelectionMode::Camera;
}

void Application::initializeSphereScene() {
    world_.clear();

    camera_id = world_.addCamera(Vector3d(0, 0, 5), Vector3d(0, 0, 0));

    Object sphere1 = createSphere(1.0, 2);
    world_.addObject(sphere1, Vector3d(0, 0, 0));

    Object sphere2 = createSphere(0.5, 1);
    world_.addObject(sphere2, Vector3d(1.5, 0.5, 0));

    selection_mode_ = SelectionMode::Camera;
}

bool Application::loadObjFile(const std::string& filename) {
    std::cout << "Loading OBJ file: " << filename << std::endl;
    
    try {
        world_.clear();
        
        camera_id = world_.addCamera(Vector3d(0, 0, 5), Vector3d(0, 0, 0));
        
        Object model = ObjLoader::loadFromFile(filename);
        
        if (model.triangles.empty()) {
            std::cerr << "Error: Failed to load model or model has no triangles" << std::endl;
            return false;
        }
        
        world_.addObject(model, Vector3d(0, 0, 0));
        
        std::cout << "OBJ file loaded successfully: " << model.triangles.size() << " triangles" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading OBJ file: " << e.what() << std::endl;
        return false;
    }
}

void Application::loadObjDirectory(const std::string& directory) {
    std::cout << "Loading all OBJ files from directory: " << directory << std::endl;
    
    try {
        world_.clear();

        camera_id = world_.addCamera(Vector3d(0.0, 2.0, 5.0), Vector3d(0.0, 1.0, 0.0));
        Camera current_camera = world_.getCamera(camera_id);
        
        ObjLoader::loadAllFromDirectory(directory, world_);
        
        std::cout << "Loaded objects from directory" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error loading OBJ files from directory: " << e.what() << std::endl;
    }
}

}  // namespace renderer
