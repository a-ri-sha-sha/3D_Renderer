#include "Screen.h"
#include <limits>

namespace renderer {

Screen::Screen(size_t width, size_t height)
    : screen_matrix_(height, width), z_buffer_(height, width) {
    z_buffer_.setConstant(std::numeric_limits<double>::infinity());
    clear();
}

void Screen::setPixel(size_t row, size_t column, const RGB& color) {
    if (row < static_cast<size_t>(screen_matrix_.rows()) && 
        column < static_cast<size_t>(screen_matrix_.cols())) {
        screen_matrix_(row, column) = color;
    }
}

void Screen::setZ(size_t row, size_t column, double z) {
    if (row < static_cast<size_t>(z_buffer_.rows()) && 
        column < static_cast<size_t>(z_buffer_.cols())) {
        z_buffer_(row, column) = z;
    }
}

double Screen::getZ(size_t row, size_t column) const {
    if (row < static_cast<size_t>(z_buffer_.rows()) && 
        column < static_cast<size_t>(z_buffer_.cols())) {
        return z_buffer_(row, column);
    }
    return std::numeric_limits<double>::infinity();
}

const RGB& Screen::getPixel(size_t row, size_t column) const {
    static RGB defaultColor(0.0, 0.0, 0.0);
    if (row < static_cast<size_t>(screen_matrix_.rows()) && 
        column < static_cast<size_t>(screen_matrix_.cols())) {
        return screen_matrix_(row, column);
    }
    return defaultColor;
}

int Screen::getWidth() const {
    return screen_matrix_.cols();
}

int Screen::getHeight() const {
    return screen_matrix_.rows();
}

void Screen::clear(const RGB& color) {
    for (int row = 0; row < screen_matrix_.rows(); ++row) {
        for (int col = 0; col < screen_matrix_.cols(); ++col) {
            screen_matrix_(row, col) = color;
        }
    }
    
    z_buffer_.setConstant(std::numeric_limits<double>::infinity());
}

Picture Screen::toPicture() const {
    Picture picture(getWidth(), getHeight());
    
    for (int y = 0; y < getHeight(); ++y) {
        for (int x = 0; x < getWidth(); ++x) {
            picture.setPixel(x, y, getPixel(y, x).toSFColor());
        }
    }
    
    return picture;
}

const Eigen::Matrix<RGB, Eigen::Dynamic, Eigen::Dynamic>& Screen::getScreenMatrix() const {
    return screen_matrix_;
}

}  // namespace renderer
