#include "RenderScene.hxx"

#define VectorFind(vector, item) (std::find((vector).begin(), (vector).end(), (item)))
#define VectorFindBinary(vector, item) ((VectorFind((vector), (item)) != (vector).end()))

namespace GL {

RenderGroupID RenderScene::getDistinctRenderGroupID(void) const noexcept
{
  RenderGroupID renderGroupIdIterator{ 0 };

  // Iterate the indexes until we hit non-used number
  while (!VectorFindBinary(m_vRenderGroupIDtoInternalID, renderGroupIdIterator))
    ++renderGroupIdIterator;

  return renderGroupIdIterator;
}

void aliasRenderGroup(const RenderGroupID &renderGroupID, const std::string &renderGroupName) const noexcept
{
  // If the render group with the same name is already exists
  if (m_mRenderGroupNames.contains(renderGroupName)) {
    throw std::runtime_error("The name " + renderGroupName + " is already aliased to another render group");
  }

  // Otherwise perform the aliasing
  m_mRenderGroupNames[renderGroupName] = id;
}

void pushToRenderGroup(const RenderGroupID &renderGroupID, const TextureDescriptor &textureDescriptor) const noexcept
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

    render_group_index = static_cast<std::ptrdiff_t>(m_vRenderGroupIDtoInternalID.size() - 1);
  }

  // Move the element to the render group
  m_vRenderGroups.at(renderGroupIndex).push_back(std::move(textureDescriptor));
}

}// namespace GL