#include "Renderer.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>

namespace renderer {

Renderer::Renderer() {
}

Picture Renderer::make(const World& world, size_t camera_id) {
    Picture picture(800, 600);
    picture.clear(sf::Color::Black);
    
    z_buffer_.resize(picture.getWidth() * picture.getHeight(), std::numeric_limits<double>::infinity());
    
    const Camera& camera = world.getCamera(camera_id);
    
    const auto& objects = world.getObjects();
    size_t obj_count = world.getObjects().size();
    
    for (size_t i = 0; i < obj_count; ++i) {
        const Object& object = objects[i];
        
        Vector3d obj_position = Vector3d::Zero();
        Quaterniond obj_rotation = Quaterniond::Identity();
        
        if (i < world.getObjectHolders().size()) {
            obj_position = world.getObjectHolders()[i].getPosition();
            obj_rotation = world.getObjectHolders()[i].getRotation();
        }
        
        for (const auto& triangle : object.triangles) {
            if (show_wireframe_) {
                renderWireframe(picture, camera, triangle, obj_position, obj_rotation);
            } else {
                renderTriangle(picture, camera, triangle, obj_position, obj_rotation);
            }
        }
    }
    
    return picture;
}

Picture Renderer::makeTestTriangle() {
    Picture picture(800, 600);
    picture.clear(sf::Color::Black);
    
    int x1 = 400, y1 = 100;
    int x2 = 200, y2 = 500;
    int x3 = 600, y3 = 500;
    
    drawLine(picture, x1, y1, x2, y2, sf::Color::Red);
    drawLine(picture, x2, y2, x3, y3, sf::Color::Red);
    drawLine(picture, x3, y3, x1, y1, sf::Color::Red);
    
    for (int y = 0; y < picture.getHeight(); ++y) {
        for (int x = 0; x < picture.getWidth(); ++x) {
            if (isPointInTriangle(sf::Vector2i(x, y), {sf::Vector2i(x1, y1), sf::Vector2i(x2, y2), sf::Vector2i(x3, y3)})) {
                picture.setPixel(x, y, sf::Color::Red);
            }
        }
    }
    
    return picture;
}


Matrix3d Renderer::transformTriangleToWorldSpace(const Matrix3d& triangle, 
                                               const Vector3d& position,
                                               const Quaterniond& rotation) {
    Matrix3d result = triangle;
    
    Eigen::Matrix3d rot_matrix = rotation.toRotationMatrix();
    
    for (int i = 0; i < 3; ++i) {
        result.row(i) = (rot_matrix * triangle.row(i).transpose()).transpose();
    }
    
    for (int i = 0; i < 3; ++i) {
        result.row(i) += position.transpose();
    }
    
    return result;
}

Matrix3d Renderer::transformTriangleToViewSpace(const Matrix3d& triangle, const Camera& camera) {
    Matrix3d result = triangle;
    
    for (int i = 0; i < 3; ++i) {
        Vector3d point_relative_to_camera = triangle.row(i).transpose() - camera.getPosition();
        
        Vector3d point_in_view_space = camera.getViewMatrix() * point_relative_to_camera;
        
        result.row(i) = point_in_view_space.transpose();
    }
    
    return result;
}

void Renderer::renderTriangle(Picture& picture, const Camera& camera, const Triangle& triangle, 
                             const Vector3d& obj_position, const Quaterniond& obj_rotation) {
    Matrix3d world_triangle = transformTriangleToWorldSpace(triangle.points, obj_position, obj_rotation);
    
    Matrix3d view_triangle = transformTriangleToViewSpace(world_triangle, camera);
    
    bool all_behind = true;
    for (int i = 0; i < 3; ++i) {
        if (view_triangle.row(i).z() < 0) {
            all_behind = false;
            break;
        }
    }

    if (all_behind) {
        return;
    }
    
    std::list<Matrix3d> triangles;
    triangles.push_back(view_triangle);
    
    // Vector4d near_plane;
    // near_plane << 0, 0, 1, -0.1;
    // clipAllTriangles(near_plane, &triangles);
    
    // if (triangles.empty()) {
    //     return;
    // }
    
    for (const auto& clipped_triangle : triangles) {
        Matrix3d projection = camera.getProjectionMatrix(
            45.0, static_cast<double>(picture.getWidth()) / picture.getHeight(), 0.1, 100.0);
        
        Matrix3d projected_triangle = clipped_triangle;
        for (int i = 0; i < 3; ++i) {
            Vector3d point = clipped_triangle.row(i).transpose();
            
            double w = -point.z();
            if (std::abs(w) < 1e-6) {
                continue;
            }
            
            Vector3d projected_point = projection * point;
            
            projected_point.x() /= w;
            projected_point.y() /= w;
            projected_point.z() /= w;
            
            projected_triangle.row(i) = projected_point.transpose();
        }
        
        std::array<sf::Vector2i, 3> screen_points;
        for (int i = 0; i < 3; ++i) {
            int x = static_cast<int>((projected_triangle.row(i).x() + 1.0) * 0.5 * picture.getWidth());
            int y = static_cast<int>((1.0 - (projected_triangle.row(i).y() + 1.0) * 0.5) * 
                                   picture.getHeight());
                                   
            screen_points[i] = sf::Vector2i(x, y);
        }
        
        int min_x = std::min({screen_points[0].x, screen_points[1].x, screen_points[2].x});
        int min_y = std::min({screen_points[0].y, screen_points[1].y, screen_points[2].y});
        int max_x = std::max({screen_points[0].x, screen_points[1].x, screen_points[2].x});
        int max_y = std::max({screen_points[0].y, screen_points[1].y, screen_points[2].y});
        
        min_x = std::max(0, min_x);
        min_y = std::max(0, min_y);
        max_x = std::min(picture.getWidth() - 1, max_x);
        max_y = std::min(picture.getHeight() - 1, max_y);
        
        for (int y = min_y; y <= max_y; ++y) {
            for (int x = min_x; x <= max_x; ++x) {
                if (isPointInTriangle(sf::Vector2i(x, y), screen_points)) {
                    Vector3d barycentric = calculateBarycentric(sf::Vector2i(x, y), screen_points);
                    
                    double z = barycentric.x() * clipped_triangle.row(0).z() +
                               barycentric.y() * clipped_triangle.row(1).z() +
                               barycentric.z() * clipped_triangle.row(2).z();
                               
                    int idx = y * picture.getWidth() + x;
                    if (z < z_buffer_[idx]) {
                        z_buffer_[idx] = z;
                        
                        // int intensity = std::max(0.0, std::min(255.0, 255.0 * (1.0 - std::abs(z) / 10.0)));
                        // sf::Color color(intensity, intensity, intensity);
                        sf::Color color(255, 0, 0);

                        picture.setPixel(x, y, color);
                    }
                }
            }
        }
        drawLine(picture, screen_points[0].x, screen_points[0].y,
                screen_points[1].x, screen_points[1].y, sf::Color::White);
        drawLine(picture, screen_points[1].x, screen_points[1].y,
                screen_points[2].x, screen_points[2].y, sf::Color::White);
        drawLine(picture, screen_points[2].x, screen_points[2].y,
                screen_points[0].x, screen_points[0].y, sf::Color::White);
    }
}

void Renderer::renderWireframe(Picture& picture, const Camera& camera, const Triangle& triangle,
                              const Vector3d& obj_position, const Quaterniond& obj_rotation) {
    Matrix3d world_triangle = transformTriangleToWorldSpace(triangle.points, obj_position, obj_rotation);
    
    Matrix3d view_triangle = transformTriangleToViewSpace(world_triangle, camera);
    
    bool all_behind = true;
    for (int i = 0; i < 3; ++i) {
        if (view_triangle.row(i).z() > 0) {
            all_behind = false;
            break;
        }
    }
    
    if (all_behind) {
        return;
    }
    
    Matrix3d projection = camera.getProjectionMatrix(
        45.0, static_cast<double>(picture.getWidth()) / picture.getHeight(), 0.1, 100.0);
    
    Matrix3d projected_triangle = view_triangle;
    for (int i = 0; i < 3; ++i) {
        Vector3d point = view_triangle.row(i).transpose();
        
        double w = -point.z();
        if (std::abs(w) < 1e-6) {
            return;
        }
        
        Vector3d projected_point = projection * point;
        
        projected_point.x() /= w;
        projected_point.y() /= w;
        projected_point.z() /= w;
        
        projected_triangle.row(i) = projected_point.transpose();
    }
    
    std::array<sf::Vector2i, 3> screen_points;
    for (int i = 0; i < 3; ++i) {
        int x = static_cast<int>((projected_triangle.row(i).x() + 1.0) * 0.5 * picture.getWidth());
        int y = static_cast<int>((1.0 - (projected_triangle.row(i).y() + 1.0) * 0.5) * 
                               picture.getHeight());
                               
        screen_points[i] = sf::Vector2i(x, y);
    }
    
    sf::Color wireframe_color(255, 255, 255);
    
    drawLine(picture, screen_points[0].x, screen_points[0].y,
                     screen_points[1].x, screen_points[1].y, wireframe_color);
    drawLine(picture, screen_points[1].x, screen_points[1].y,
                     screen_points[2].x, screen_points[2].y, wireframe_color);
    drawLine(picture, screen_points[2].x, screen_points[2].y,
                     screen_points[0].x, screen_points[0].y, wireframe_color);
}

void Renderer::drawLine(Picture& picture, int x1, int y1, int x2, int y2, sf::Color color) {
    bool steep = false;
    if (std::abs(x1 - x2) < std::abs(y1 - y2)) {
        std::swap(x1, y1);
        std::swap(x2, y2);
        steep = true;
    }
    
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    
    int dx = x2 - x1;
    int dy = std::abs(y2 - y1);
    int error = dx / 2;
    int ystep = (y1 < y2) ? 1 : -1;
    int y = y1;
    
    for (int x = x1; x <= x2; ++x) {
        if (steep) {
            if (y >= 0 && y < picture.getWidth() && x >= 0 && x < picture.getHeight()) {
                picture.setPixel(y, x, color);
            }
        } else {
            if (x >= 0 && x < picture.getWidth() && y >= 0 && y < picture.getHeight()) {
                picture.setPixel(x, y, color);
            }
        }
        
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}
bool Renderer::isPointInTriangle(const sf::Vector2i& p, const std::array<sf::Vector2i, 3>& triangle) {
    auto sign = [](const sf::Vector2i& p1, const sf::Vector2i& p2, const sf::Vector2i& p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    };
    
    bool b1 = sign(p, triangle[0], triangle[1]) < 0;
    bool b2 = sign(p, triangle[1], triangle[2]) < 0;
    bool b3 = sign(p, triangle[2], triangle[0]) < 0;
    
    return ((b1 == b2) && (b2 == b3));
}

Vector3d Renderer::calculateBarycentric(const sf::Vector2i& p, const std::array<sf::Vector2i, 3>& triangle) {
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

bool Renderer::isPointInFrontOfPlane(const Vector4d& plane, const Vector3d& point) {
    return plane.head<3>().dot(point) + plane.w() > 0;
}

Vector3d Renderer::planeLineIntersection(const Vector4d& plane, const Vector3d& point1, const Vector3d& point2) {
    Vector3d line_dir = point2 - point1;
    double t = -(plane.head<3>().dot(point1) + plane.w()) / plane.head<3>().dot(line_dir);
    return point1 + t * line_dir;
}
bool Renderer::clipTriangle(const Vector4d& plane, const Matrix3d& triangle, std::list<Matrix3d>* result) {
    std::vector<bool> inside(3);
    int inside_count = 0;
    
    for (int i = 0; i < 3; ++i) {
        inside[i] = isPointInFrontOfPlane(plane, triangle.row(i));
        if (inside[i]) {
            inside_count++;
        }
    }
    
    if (inside_count == 3) {
        return false;
    }
    
    if (inside_count == 0) {
        return true;
    }
    
    if (inside_count == 1) {
        int inside_vertex = -1;
        for (int i = 0; i < 3; ++i) {
            if (inside[i]) {
                inside_vertex = i;
                break;
            }
        }
        
        Vector3d p0 = triangle.row(inside_vertex);
        Vector3d p1 = triangle.row((inside_vertex + 1) % 3);
        Vector3d p2 = triangle.row((inside_vertex + 2) % 3);
        
        Vector3d intersection1 = planeLineIntersection(plane, p0, p1);
        Vector3d intersection2 = planeLineIntersection(plane, p0, p2);
        
        Matrix3d new_triangle;
        new_triangle.row(0) = p0;
        new_triangle.row(1) = intersection1;
        new_triangle.row(2) = intersection2;
        
        result->push_back(new_triangle);
    }
    
    if (inside_count == 2) {
        int outside_vertex = -1;
        for (int i = 0; i < 3; ++i) {
            if (!inside[i]) {
                outside_vertex = i;
                break;
            }
        }
        
        Vector3d p0 = triangle.row(outside_vertex);
        Vector3d p1 = triangle.row((outside_vertex + 1) % 3);
        Vector3d p2 = triangle.row((outside_vertex + 2) % 3);
        
        Vector3d intersection1 = planeLineIntersection(plane, p0, p1);
        Vector3d intersection2 = planeLineIntersection(plane, p0, p2);
        
        Matrix3d new_triangle1;
        new_triangle1.row(0) = p1;
        new_triangle1.row(1) = intersection1;
        new_triangle1.row(2) = intersection2;
        
        Matrix3d new_triangle2;
        new_triangle2.row(0) = p1;
        new_triangle2.row(1) = intersection2;
        new_triangle2.row(2) = p2;
        
        result->push_back(new_triangle1);
        result->push_back(new_triangle2);
    }
    
    return true;
}

void Renderer::clipAllTriangles(const Vector4d& plane, std::list<Matrix3d>* triangles) {
    if (triangles->empty()) {
        return;
    }
    
    std::list<Matrix3d> result;
    
    for (auto it = triangles->begin(); it != triangles->end();) {
        bool clipped = clipTriangle(plane, *it, &result);
        if (clipped) {
            it = triangles->erase(it);
        } else {
            result.push_back(*it);
            ++it;
        }
    }
    
    *triangles = std::move(result);
}

}  // namespace renderer
