#include "Window.hxx"

#include <memory>

#include "rendering/RenderScene.hxx"
#include "rendering/RenderManager.hxx"

#include "game/Card.hxx"

int main(int argc, char *argv[])
{
  std::shared_ptr<GL::Window> applicationWindow(new GL::Window({ 640, 480 }));
  std::shared_ptr<GL::RenderingManager> globalRenderManager(new GL::RenderingManager());

  GL::RenderScene menuScene;
  GL::RenderScene gameScene;

  gameScene.pushToRenderGroup(1, std::move(GL::RenderObject("data/test.png")));

  gameScene.enableRenderGroup(1);
  globalRenderManager->bindRenderScene("Game Scene", std::move(gameScene));
  globalRenderManager->setActiveRenderScene("Game Scene");

  applicationWindow->bindRenderManager(globalRenderManager);
  applicationWindow->initialize();
  applicationWindow->render();
  applicationWindow->release();

  return 0;
}
