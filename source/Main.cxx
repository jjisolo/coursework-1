#include "Window.hxx"

int main(int argc, char* argv[]) {
  gl::Window applicationWindow({640, 480});
  applicationWindow.initialize();
  applicationWindow.render();
  applicationWindow.release();

  return 0;
}