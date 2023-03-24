#include "Window.hxx"

#include "game/Card.hxx"

int main(int argc, char *argv[])
{
  GL::Window applicationWindow({ 640, 480 });
  applicationWindow.initialize();
  applicationWindow.render();
  applicationWindow.release();

  return 0;
}
