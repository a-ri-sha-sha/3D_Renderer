#pragma once

#include <SFML/Graphics.hpp>

#include "Linalg.h"
#include "Picture.h"

namespace renderer {

class RGB {
public:
    RGB(double r = 0.0, double g = 0.0, double b = 0.0);

    RGB(const sf::Color& color);

    double r() const;
    double g() const;
    double b() const;

    void r(double value);
    void g(double value);
    void b(double value);

    sf::Color toSFColor() const;

    RGB& operator+=(const RGB& other);

    RGB operator+(const RGB& other) const;

    RGB& operator*=(double scalar);

    RGB operator*(double scalar) const;

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
    MatrixXd z_buffer_;
};

}  // namespace renderer
