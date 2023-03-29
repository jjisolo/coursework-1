#include "RenderManager.hxx"

void GL::RenderingManager::bindRenderScene(const std::string &renderSceneTag, RenderScene &&renderScene)
{
  // If we are rewriting the render scene, it means we throw the exception
  // because we can't disable it
  if (m_mRenderSceneContainer.contains(renderSceneTag))
    throw std::invalid_argument("Attempted to bind existing render scene");

  m_mRenderSceneContainer.emplace(renderSceneTag, std::move(renderScene));
}

void GL::RenderingManager::setActiveRenderScene(const std::string &renderSceneTag)
{
  if (!m_mRenderSceneContainer.contains(renderSceneTag))
    throw std::invalid_argument("Attempted to activate render scene with invalid tag");

  m_sActiveRenderScene = renderSceneTag;
}

void GL::RenderingManager::render()
{
  try {
    auto &activeRenderScene = m_mRenderSceneContainer.at(m_sActiveRenderScene);
    auto enabledRenderGroupsList = activeRenderScene.getEnabledRenderGroups();

    // Clear the screen before any rendering happens
    GL::Utils::ClearScreen(GL::Utils::ColorCode::Black);

  } catch (const std::out_of_range &exception) {
    throw std::runtime_error("Cannot get acces to the " + m_sActiveRenderScene + " render scene");
  }
}
