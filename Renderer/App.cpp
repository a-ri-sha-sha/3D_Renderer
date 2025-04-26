#include "App.h"

namespace renderer {

Application::Application() {
    // Not implemented
    // читаем obj файл, создаем все объекты
}

void Application::run() {
    while (drawer_.isOpen()) {
        drawer_.processEvents();
        handleInput();
        Picture pic = renderer_.make(world_, camera_id);
        drawer_.draw(pic);
    }
}

void Application::handleInput() {
    const double move_speed = 0.1;
    const double rotate_speed = 1.0;

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
}

}  // namespace renderer
