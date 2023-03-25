#include "RenderScene.hxx"

#include "mgr/TextureManager.hxx"

#define VectorFind(vector, item) (std::find((vector).begin(), (vector).end(), (item)))
#define VectorFindBinary(vector, item) ((VectorFind((vector), (item)) != (vector).end()))

namespace GL {

RenderScene::RenderGroupID RenderScene::getDistinctRenderGroupID(void) const noexcept
{
  RenderGroupID renderGroupIdIterator{ 0 };

  // Iterate the indexes until we hit non-used number
  while (!VectorFindBinary(m_vRenderGroupIDtoInternalID, renderGroupIdIterator))
    ++renderGroupIdIterator;

  return renderGroupIdIterator;
}

void RenderScene::aliasRenderGroup(const RenderGroupID renderGroupID, const std::string &renderGroupName)
{
  // If the render group with the same name is already exists
  if (m_mRenderGroupNames.contains(renderGroupName)) {
    throw std::runtime_error("The name " + renderGroupName + " is already aliased to another render group");
  }

  // Otherwise perform the aliasing
  m_mRenderGroupNames[renderGroupName] = renderGroupID;
}

void RenderScene::pushToRenderGroup(const RenderGroupID renderGroupID, const RenderObject &renderObject) noexcept
{
  // The index of the render group associated with the renderGroupID
  std::ptrdiff_t renderGroupIndex{ 0 };

  try {
    // Find out the index of the render group by its ID
    renderGroupIndex = getRenderGroupByID(renderGroupID);
  } catch (const std::invalid_argument &exception) {
    // As soon as the specified ID is not in the table, the new
    // render group should be created
    m_vRenderGroupIDtoInternalID.push_back(renderGroupID);
    m_vRenderGroups.emplace_back();

    renderGroupIndex = static_cast<std::ptrdiff_t>(m_vRenderGroupIDtoInternalID.size() - 1);
  }

  // Move the element to the render group
  m_vRenderGroups.at(renderGroupIndex).push_back(std::move(renderObject));
}

void RenderScene::pushToRenderGroup(const std::string &renderGroupName, const RenderObject &renderObject) noexcept
{
  // Check if this name exists in the name container, if it
  // does not create it and asign to the current id.
  if (!m_mRenderGroupNames.contains(renderGroupName)) {
    aliasRenderGroup(getDistinctRenderGroupID(), renderGroupName);
  }

  // Otherwise id is exists, so we just pull it from the map
  pushToRenderGroup(m_mRenderGroupNames[renderGroupName], std::move(renderObject));
}

void RenderScene::enableRenderGroup(const RenderGroupID renderGroupID) noexcept
{
  // Mark the current scene as `enabled`
  m_vEnabledRenderGroups.push_back(renderGroupID);

  // Iterate through each render object at the render group, which id is provided by the user
  // and load the each render object to the graphics card RAM
  for (auto &renderObject : m_vRenderGroups.at(getRenderGroupByID(renderGroupID)))
    renderObject.textureDescriptor = Utils::TextureManager::loadTexture2D(renderObject.texturePath);
}

}// namespace GL