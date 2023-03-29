#pragma once

#include <vector>
#include <stdexcept>
#include <unordered_map>

#include "GlUtils.hxx"
#include "RenderScene.hxx"

namespace GL {

class RenderingManager
{
public:
  // Bind new render scene
  void bindRenderScene(const std::string &renderSceneTag, RenderScene &&renderScene);

  // Set one of render scene as active
  void setActiveRenderScene(const std::string &renderSceneTag);

  // Render the current active render scene
  void render();

private:
  std::unordered_map<std::string, RenderScene> m_mRenderSceneContainer;
  std::string m_sActiveRenderScene;
};


}// namespace GL
