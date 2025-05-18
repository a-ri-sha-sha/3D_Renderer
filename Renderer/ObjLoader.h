#pragma once

#include "World.h"
#include "Primitives.h"

namespace renderer {

class ObjLoader {
public:
    static Object loadFromFile(const std::string& filePath);
    
    static void loadAllFromDirectory(const std::string& directoryPath, World& world);
};

} // namespace renderer
