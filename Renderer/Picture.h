#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>

namespace renderer {

class Picture {
public:
    Picture(int width = 800, int height = 600);

    void setPixel(int x, int y, sf::Color color);
    sf::Color getPixel(int x, int y) const;

    void clear(sf::Color color = sf::Color::Black);

    int getWidth() const {
        return width_;
    }
    int getHeight() const {
        return height_;
    }

    const sf::Texture& getTexture() const;

private:
    int width_;
    int height_;
    sf::Image image_;
    sf::Texture texture_;
    bool texture_needs_update_ = true;
};

}  // namespace renderer
