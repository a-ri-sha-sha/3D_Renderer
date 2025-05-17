#include "App.h"
#include "Except.h"
#include <iostream>

int main() {
  try {
    renderer::Application app;    
    app.run();
  } catch(const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    except::react();
  } catch(...) {
    std::cerr << "Unknown error occurred!" << std::endl;
    except::react();
  }
  return 0;
}
