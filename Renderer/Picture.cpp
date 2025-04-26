#include "Picture.h"

namespace renderer {

Picture::Picture(unsigned int width, unsigned int height) : width_(width), height_(height) {
    image_.create(width, height, sf::Color::Black);
    texture_.create(width, height);
}

void Picture::setPixel(unsigned int x, unsigned int y, sf::Color color) {
    if (x < width_ && y < height_) {
        image_.setPixel(x, y, color);
        texture_needs_update_ = true;
    }
}

sf::Color Picture::getPixel(unsigned int x, unsigned int y) const {
    if (x < width_ && y < height_) {
        return image_.getPixel(x, y);
    }
    return sf::Color::Black;
}

void Picture::clear(sf::Color color) {
    image_.create(width_, height_, color);
    texture_needs_update_ = true;
}

const sf::Texture& Picture::getTexture() const {
    if (texture_needs_update_) {
        texture_.loadFromImage(image_);
        texture_needs_update_ = false;
    }
    return texture_;
}

}  // namespace renderer
