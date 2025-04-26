#include "Renderer.h"
#include <algorithm>
#include <cmath>

namespace renderer {

Picture Renderer::make(const World& world, size_t camera_id) {
    Picture picture(800, 600);
    picture.clear(sf::Color::Black);

    const Camera& camera = world.getCamera(camera_id);

    const std::vector<Object>& objects = world.getObjects();

    for (const auto& object : objects) {
        for (const auto& triangle : object.triangles) {
            renderTriangle(picture, camera, triangle);
        }
    }

    return picture;
}

void Renderer::renderTriangle(Picture& picture, const Camera& camera, const Triangle& triangle) {
    Matrix3d view_matrix = camera.getViewMatrix();

    Matrix3d transformed_points = triangle.points;
    for (int i = 0; i < 3; ++i) {
        Vector3d point = triangle.points.row(i).transpose();
        Vector3d transformed_point = view_matrix * point;
        transformed_points.row(i) = transformed_point.transpose();
    }

    bool all_behind = true;
    for (int i = 0; i < 3; ++i) {
        if (transformed_points.row(i).z() > 0) {
            all_behind = false;
            break;
        }
    }

    if (all_behind) {
        return;
    }

    Matrix3d projection_matrix = camera.getProjectionMatrix(
        45.0, static_cast<double>(picture.getWidth()) / picture.getHeight(), 0.1, 100.0);

    Matrix3d projected_points = transformed_points;
    for (int i = 0; i < 3; ++i) {
        Vector3d point = transformed_points.row(i).transpose();

        if (std::abs(point.z()) < 1e-6) {
            return;
        }

        double w = -point.z();
        Vector3d projected_point = projection_matrix * point;

        projected_point.x() /= w;
        projected_point.y() /= w;
        projected_point.z() /= w;

        projected_points.row(i) = projected_point.transpose();
    }

    std::array<sf::Vector2i, 3> screen_points;
    for (int i = 0; i < 3; ++i) {
        int x = static_cast<int>((projected_points.row(i).x() + 1.0) * 0.5 * picture.getWidth());
        int y = static_cast<int>((1.0 - (projected_points.row(i).y() + 1.0) * 0.5) *
                                 picture.getHeight());

        screen_points[i] = sf::Vector2i(x, y);
    }

    int min_x = std::min({screen_points[0].x, screen_points[1].x, screen_points[2].x});
    int min_y = std::min({screen_points[0].y, screen_points[1].y, screen_points[2].y});
    int max_x = std::max({screen_points[0].x, screen_points[1].x, screen_points[2].x});
    int max_y = std::max({screen_points[0].y, screen_points[1].y, screen_points[2].y});

    min_x = std::max(0, min_x);
    min_y = std::max(0, min_y);
    max_x = std::min(static_cast<int>(picture.getWidth()) - 1, max_x);
    max_y = std::min(static_cast<int>(picture.getHeight()) - 1, max_y);

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            if (isPointInTriangle(sf::Vector2i(x, y), screen_points)) {
                Vector3d barycentric = calculateBarycentric(sf::Vector2i(x, y), screen_points);

                double z = barycentric.x() * transformed_points.row(0).z() +
                           barycentric.y() * transformed_points.row(1).z() +
                           barycentric.z() * transformed_points.row(2).z();

                int intensity = static_cast<int>(
                    std::max(0.0, std::min(255.0, 255.0 * (1.0 - std::abs(z) / 10.0))));
                sf::Color color(intensity, intensity, intensity);

                picture.setPixel(x, y, color);
            }
        }
    }
}

bool Renderer::isPointInTriangle(const sf::Vector2i& p,
                                 const std::array<sf::Vector2i, 3>& triangle) {
    auto sign = [](const sf::Vector2i& p1, const sf::Vector2i& p2, const sf::Vector2i& p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    };

    bool b1 = sign(p, triangle[0], triangle[1]) < 0;
    bool b2 = sign(p, triangle[1], triangle[2]) < 0;
    bool b3 = sign(p, triangle[2], triangle[0]) < 0;

    return ((b1 == b2) && (b2 == b3));
}

Vector3d Renderer::calculateBarycentric(const sf::Vector2i& p,
                                        const std::array<sf::Vector2i, 3>& triangle) {
    Vector3d result;

    int x1 = triangle[0].x;
    int y1 = triangle[0].y;
    int x2 = triangle[1].x;
    int y2 = triangle[1].y;
    int x3 = triangle[2].x;
    int y3 = triangle[2].y;

    int det = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
    if (det == 0) {
        return Vector3d(1, 0, 0);
    }

    result.x() = ((y2 - y3) * (p.x - x3) + (x3 - x2) * (p.y - y3)) / static_cast<double>(det);
    result.y() = ((y3 - y1) * (p.x - x3) + (x1 - x3) * (p.y - y3)) / static_cast<double>(det);
    result.z() = 1.0 - result.x() - result.y();

    return result;
}

}  // namespace renderer
