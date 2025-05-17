#include "Screen.h"

#include <limits>

namespace renderer {

RGB::RGB(double r, double g, double b) : r_(r), g_(g), b_(b) {
}

RGB::RGB(const sf::Color& color) : r_(color.r / 255.0), g_(color.g / 255.0), b_(color.b / 255.0) {
}

double RGB::r() const {
    return r_;
}
double RGB::g() const {
    return g_;
}
double RGB::b() const {
    return b_;
}

void RGB::r(double value) {
    r_ = value;
}
void RGB::g(double value) {
    g_ = value;
}
void RGB::b(double value) {
    b_ = value;
}

sf::Color RGB::toSFColor() const {
    return sf::Color(static_cast<uint8_t>(std::min(255.0, std::max(0.0, r_ * 255.0))),
                     static_cast<uint8_t>(std::min(255.0, std::max(0.0, g_ * 255.0))),
                     static_cast<uint8_t>(std::min(255.0, std::max(0.0, b_ * 255.0))));
}

RGB& RGB::operator+=(const RGB& other) {
    r_ += other.r_;
    g_ += other.g_;
    b_ += other.b_;
    return *this;
}

RGB RGB::operator+(const RGB& other) const {
    RGB result = *this;
    result += other;
    return result;
}

RGB& RGB::operator*=(double scalar) {
    r_ *= scalar;
    g_ *= scalar;
    b_ *= scalar;
    return *this;
}

RGB RGB::operator*(double scalar) const {
    RGB result = *this;
    result *= scalar;
    return result;
}

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
