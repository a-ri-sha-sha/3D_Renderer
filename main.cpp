#include "Renderer/App.h"
#include "Except.h"

int main() {
  try {
    renderer::Application app;
    app.run();
  } catch(...) {
    except::react();
  }
  return 0;
}
