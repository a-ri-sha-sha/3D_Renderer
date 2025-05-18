#include "ObjLoader.h"
#include "../OBJ-Loader/Source/OBJ_Loader.h"

#include <iostream>
#include <filesystem>

namespace renderer {

Object ObjLoader::loadFromFile(const std::string& filePath) {
    Object object;
    objl::Loader loader;
    
    std::cout << "Loading OBJ file: " << filePath << std::endl;
    
    if (!loader.LoadFile(filePath)) {
        std::cerr << "Failed to load OBJ file: " << filePath << std::endl;
        return object;
    }
    
    std::cout << "Successfully loaded OBJ file with " << loader.LoadedMeshes.size() << " meshes." << std::endl;
    
    // Проходим по всем сеткам в OBJ файле
    for (const auto& mesh : loader.LoadedMeshes) {
        std::cout << "Processing mesh: " << mesh.MeshName << " with " 
                  << mesh.Indices.size() / 3 << " triangles." << std::endl;
        
        // Добавим информацию о границах модели для отладки
        Vector3d min_bounds(std::numeric_limits<double>::max(), 
                           std::numeric_limits<double>::max(), 
                           std::numeric_limits<double>::max());
        Vector3d max_bounds(std::numeric_limits<double>::lowest(), 
                           std::numeric_limits<double>::lowest(), 
                           std::numeric_limits<double>::lowest());
        
        // Проходим по всем индексам и создаем треугольники
        for (size_t i = 0; i < mesh.Indices.size(); i += 3) {
            // Проверяем, что у нас есть 3 индекса для треугольника
            if (i + 2 >= mesh.Indices.size()) {
                break;
            }
            
            // Получаем индексы вершин треугольника
            unsigned int idx1 = mesh.Indices[i];
            unsigned int idx2 = mesh.Indices[i + 1];
            unsigned int idx3 = mesh.Indices[i + 2];
            
            // Проверяем, что индексы действительны
            if (idx1 >= mesh.Vertices.size() || idx2 >= mesh.Vertices.size() || idx3 >= mesh.Vertices.size()) {
                std::cerr << "Invalid vertex index in OBJ file." << std::endl;
                continue;
            }
            
            // Создаем матрицу с вершинами треугольника
            Matrix3d points;
            
            // Заполняем матрицу координатами вершин и обновляем границы модели
            for (int j = 0; j < 3; j++) {
                int idx = mesh.Indices[i + j];
                points(j, 0) = mesh.Vertices[idx].Position.X;
                points(j, 1) = mesh.Vertices[idx].Position.Y;
                points(j, 2) = mesh.Vertices[idx].Position.Z;
                
                // Обновляем границы модели
                min_bounds.x() = std::min(min_bounds.x(), points(j, 0));
                min_bounds.y() = std::min(min_bounds.y(), points(j, 1));
                min_bounds.z() = std::min(min_bounds.z(), points(j, 2));
                max_bounds.x() = std::max(max_bounds.x(), points(j, 0));
                max_bounds.y() = std::max(max_bounds.y(), points(j, 1));
                max_bounds.z() = std::max(max_bounds.z(), points(j, 2));
            }
            
            // Добавляем треугольник в объект
            object.addTriangle(points);
        }
        
        std::cout << "Model bounds: Min(" << min_bounds.x() << ", " << min_bounds.y() << ", " << min_bounds.z() 
                  << ") Max(" << max_bounds.x() << ", " << max_bounds.y() << ", " << max_bounds.z() << ")" << std::endl;
    }
    
    std::cout << "Created object with " << object.triangles.size() << " triangles." << std::endl;
    return object;
}

void ObjLoader::loadAllFromDirectory(const std::string& directoryPath, World& world) {
    try {
        std::cout << "Loading OBJ files from directory: " << directoryPath << std::endl;
        
        if (!std::filesystem::exists(directoryPath)) {
            std::filesystem::create_directory(directoryPath);
            std::cout << "Created directory: " << directoryPath << std::endl;
        }
        
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            std::string filePath = entry.path().string();
            
            if (filePath.size() >= 4 && 
                filePath.substr(filePath.size() - 4) == ".obj") {
                std::cout << "Found OBJ file: " << filePath << std::endl;
                
                Object object = loadFromFile(filePath);
                
                if (!object.triangles.empty()) {
                    world.addObject(object);
                    std::cout << "Added object to world." << std::endl;
                } else {
                    std::cout << "Object has no triangles, skipping." << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading files from directory: " << e.what() << std::endl;
    }
}

} // namespace renderer
