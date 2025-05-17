#include "Picture.h"

namespace renderer {

Picture::Picture(int width, int height) : width_(width), height_(height) {
    image_.create(width, height, sf::Color::Black);
}

void Picture::setPixel(int x, int y, sf::Color color) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        image_.setPixel(x, y, color);
    }
}

sf::Color Picture::getPixel(int x, int y) const {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        return image_.getPixel(x, y);
    }
    return sf::Color::Black;
}

void Picture::clear(sf::Color color) {
    image_.create(width_, height_, color);
}

int Picture::getWidth() const {
    return width_;
}
int Picture::getHeight() const {
    return height_;
}

const sf::Image& Picture::getImage() const {
    return image_;
}

}  // namespace renderer
