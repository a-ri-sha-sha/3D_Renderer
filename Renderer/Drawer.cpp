#include "Drawer.h"

namespace renderer {

Drawer::Drawer() {
    window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "3D Renderer",
                                                 sf::Style::Default);

    for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
        keys_[i] = false;
    }

    for (int i = 0; i < sf::Mouse::ButtonCount; ++i) {
        mouse_buttons_[i] = false;
    }

    mouse_position_ = sf::Vector2i(0, 0);
}

Drawer::~Drawer() {
    if (window_ && window_->isOpen()) {
        window_->close();
    }
}

void Drawer::draw(const Picture& picture) {
    if (!window_->isOpen()) {
        return;
    }

    window_->clear(sf::Color::Black);
    sprite_.setTexture(picture.getTexture(), true);
    window_->draw(sprite_);
    window_->display();
}

bool Drawer::isOpen() const {
    return window_->isOpen();
}

void Drawer::processEvents() {
    sf::Event event;
    while (window_->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window_->close();
                break;

            case sf::Event::KeyPressed:
                keys_[event.key.code] = true;
                break;

            case sf::Event::KeyReleased:
                keys_[event.key.code] = false;
                break;

            case sf::Event::MouseButtonPressed:
                mouse_buttons_[event.mouseButton.button] = true;
                break;

            case sf::Event::MouseButtonReleased:
                mouse_buttons_[event.mouseButton.button] = false;
                break;

            case sf::Event::MouseMoved:
                mouse_position_ = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
                break;

            default:
                break;
        }
    }
}

bool Drawer::isKeyPressed(sf::Keyboard::Key key) const {
    if (key >= 0 && key < sf::Keyboard::KeyCount) {
        return keys_[key];
    }
    return false;
}

bool Drawer::isMouseButtonPressed(sf::Mouse::Button button) const {
    if (button >= 0 && button < sf::Mouse::ButtonCount) {
        return mouse_buttons_[button];
    }
    return false;
}

sf::Vector2i Drawer::getMousePosition() const {
    return mouse_position_;
}

}  // namespace renderer
