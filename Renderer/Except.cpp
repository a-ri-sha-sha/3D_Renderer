#include "Except.h"

#include <exception>
#include <iostream>

namespace except {

void react() {
    try {
        throw;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred!" << std::endl;
    }
}

}  // namespace except
