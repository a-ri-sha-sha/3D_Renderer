#pragma once
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include "Picture.h"

namespace renderer {

class RGB {
public:
    RGB(double r = 0.0, double g = 0.0, double b = 0.0) : r_(r), g_(g), b_(b) {}
    
    RGB(const sf::Color& color) 
        : r_(color.r / 255.0), g_(color.g / 255.0), b_(color.b / 255.0) {}
    
    double r() const { return r_; }
    double g() const { return g_; }
    double b() const { return b_; }
    
    void r(double value) { r_ = value; }
    void g(double value) { g_ = value; }
    void b(double value) { b_ = value; }
    
    sf::Color toSFColor() const {
        return sf::Color(
            static_cast<uint8_t>(std::min(255.0, std::max(0.0, r_ * 255.0))),
            static_cast<uint8_t>(std::min(255.0, std::max(0.0, g_ * 255.0))),
            static_cast<uint8_t>(std::min(255.0, std::max(0.0, b_ * 255.0)))
        );
    }
    
    RGB& operator+=(const RGB& other) {
        r_ += other.r_;
        g_ += other.g_;
        b_ += other.b_;
        return *this;
    }
    
    RGB operator+(const RGB& other) const {
        RGB result = *this;
        result += other;
        return result;
    }
    
    RGB& operator*=(double scalar) {
        r_ *= scalar;
        g_ *= scalar;
        b_ *= scalar;
        return *this;
    }
    
    RGB operator*(double scalar) const {
        RGB result = *this;
        result *= scalar;
        return result;
    }
    
private:
    double r_ = 0.0;
    double g_ = 0.0;
    double b_ = 0.0;
};

class Screen {
public:
    Screen(size_t width, size_t height);
    
    void setPixel(size_t row, size_t column, const RGB& color);
    void setZ(size_t row, size_t column, double z);
    double getZ(size_t row, size_t column) const;
    const RGB& getPixel(size_t row, size_t column) const;
    
    int getWidth() const;
    int getHeight() const;
    
    Picture toPicture() const;
    
    void clear(const RGB& color = RGB(0, 0, 0));
    
    const Eigen::Matrix<RGB, Eigen::Dynamic, Eigen::Dynamic>& getScreenMatrix() const;

private:
    Eigen::Matrix<RGB, Eigen::Dynamic, Eigen::Dynamic> screen_matrix_;
    Eigen::MatrixXd z_buffer_;
};

}  // namespace renderer
