#pragma once

#include "World.h"
#include "Picture.h"
#include "Linalg.h"

#include <vector>
#include <list>

namespace renderer {

class Renderer {
public:
    Renderer();

    Picture make(const World& world, size_t camera_id);

    void toggleWireframe();
    bool isWireframeMode() const;

private:
    void renderTriangle(Picture& picture, const Camera& camera, const Triangle& triangle,
                        const Vector3d& obj_position, const Quaterniond& obj_rotation);

    void renderWireframe(Picture& picture, const Camera& camera, const Triangle& triangle,
                         const Vector3d& obj_position, const Quaterniond& obj_rotation);

    void drawLine(Picture& picture, int x1, int y1, int x2, int y2, sf::Color color);

    bool isPointInTriangle(const sf::Vector2i& p, const std::array<sf::Vector2i, 3>& triangle);

    Vector3d calculateBarycentric(const sf::Vector2i& p,
                                  const std::array<sf::Vector2i, 3>& triangle);

    bool clipTriangle(const Vector4d& plane, const Matrix3d& triangle, std::list<Matrix3d>* result);
    void clipAllTriangles(const Vector4d& plane, std::list<Matrix3d>* triangles);

    bool isPointInFrontOfPlane(const Vector4d& plane, const Vector3d& point);
    Vector3d planeLineIntersection(const Vector4d& plane, const Vector3d& point1,
                                   const Vector3d& point2);

    Matrix3d transformTriangleToWorldSpace(const Matrix3d& triangle, const Vector3d& position,
                                           const Quaterniond& rotation);

    Matrix3d transformTriangleToViewSpace(const Matrix3d& triangle, const Camera& camera);

    std::vector<double> z_buffer_;

    bool show_wireframe_ = false;
};

}  // namespace renderer
