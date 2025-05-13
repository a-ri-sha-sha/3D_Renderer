#pragma once

#include "World.h"
#include "Picture.h"

namespace renderer {
class Renderer {
public:
    Picture make(const World& world, size_t camera_id);

private:
    void renderTriangle(Picture& picture, const Camera& camera, const Triangle& triangle);

    void drawLine(Picture& picture, int x1, int y1, int x2, int y2, sf::Color color);

    bool isPointInTriangle(sf::Vector2i, std::array<sf::Vector2i, 3>);

};
}  // namespace renderer
