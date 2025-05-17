#pragma once

#include "Picture.h"

#include <SFML/Graphics.hpp>

#include <memory>

namespace renderer {

class Drawer {
public:
    Drawer();
    ~Drawer();

    void draw(const Picture& picture);

    bool isOpen() const;
    void processEvents();

    bool isKeyPressed(sf::Keyboard::Key key) const;
    bool isMouseButtonPressed(sf::Mouse::Button button) const;
    sf::Vector2i getMousePosition() const;

private:
    std::unique_ptr<sf::RenderWindow> window_;
    sf::Texture texture_;
    sf::Sprite sprite_;

    bool keys_[sf::Keyboard::KeyCount];
    bool mouse_buttons_[sf::Mouse::ButtonCount];
    sf::Vector2i mouse_position_;
};

}  // namespace renderer
