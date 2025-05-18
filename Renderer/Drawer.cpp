#include "Drawer.h"

#include <iostream>

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

    texture_.create(800, 600);
}

Drawer::~Drawer() {
    if (window_ && window_->isOpen()) {
        window_->close();
    }
}

void Drawer::draw(const Picture& picture) {
    if (!window_->isOpen()) {
        std::cerr << "Window is not open!" << std::endl;
        return;
    }

    std::cout << "Clearing window with blue color..." << std::endl;
    window_->clear(sf::Color::Blue);

    std::cout << "Getting image from picture..." << std::endl;
    const sf::Image& image = picture.getImage();

    std::cout << "Image size: " << image.getSize().x << "x" << image.getSize().y << std::endl;

    bool has_red_pixels = false;
    for (unsigned int y = 0; y < image.getSize().y; ++y) {
        for (unsigned int x = 0; x < image.getSize().x; ++x) {
            sf::Color pixel = image.getPixel(x, y);
            if (pixel.r > 0 && pixel.g == 0 && pixel.b == 0) {
                has_red_pixels = true;
                break;
            }
        }
        if (has_red_pixels) {
            break;
        }
    }
    std::cout << "Image has red pixels: " << (has_red_pixels ? "YES" : "NO") << std::endl;

    sf::Texture texture;
    if (!texture.loadFromImage(image)) {
        std::cerr << "Failed to load texture from image!" << std::endl;
    }

    sprite_.setTexture(texture, true);

    std::cout << "Drawing sprite..." << std::endl;
    window_->draw(sprite_);

    std::cout << "Displaying window..." << std::endl;
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
