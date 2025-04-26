#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>

namespace renderer {

class Picture {
public:
    Picture(unsigned int width = 800, unsigned int height = 600);

    void setPixel(unsigned int x, unsigned int y, sf::Color color);
    sf::Color getPixel(unsigned int x, unsigned int y) const;

    void clear(sf::Color color = sf::Color::Black);

    unsigned int getWidth() const {
        return width_;
    }
    unsigned int getHeight() const {
        return height_;
    }

    const sf::Texture& getTexture() const;

private:
    unsigned int width_;
    unsigned int height_;
    sf::Image image_;
    sf::Texture texture_;
    bool texture_needs_update_ = true;
};

}  // namespace renderer
