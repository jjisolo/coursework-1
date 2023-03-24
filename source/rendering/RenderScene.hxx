#pragma once

#include <stdexcept>
#include <string>

#include <vector>
#include <list>

namespace GL {

using TextureDescriptor = unsigned int;
using RenderGroup = std::vector<TextureDescriptor>;
using RenderGroupCointainer = std::vector<RenderGroup>;

using RenderGroupID = unsigned int;
using RenderGroupIDList = std::list<RenderGroupID>;
using RenderGroupIDContainer = std::vector<RenderGroupID>;
using RenderGroupNameContainer = std::unordered_map<std::string, RenderGroupID>;

class RenderScene
{
  friend class RenderingManager;

public:
  // Get the unique ID for the freshly-baked render group
  RenderGroupID getDistinctRenderGroupID(void) const noexcept;

  // Alias render group to some unique string
  void aliasRenderGroup(const RenderGroupID &renderGroupID, const std::string &renderGroupName) const noexcept;

  // Push the texture descriptor to the render group using ID
  void pushToRenderGroup(const RenderGroupID &renderGroupID, const TextureDescriptor &textureDescriptor) const noexcept;

  // Push the texture descriptor to the render group using aliased Name
  void pushToRenderGroup(const std::string &renderGroupName, const TextureDescriptor &textureDescriptor) const noexcept;

private:
  RenderGroupContainer m_vRenderGroups;
  RenderGroupIDList m_lEnabledRenderGroups;
  RenderGroupIDContainer m_vRenderGroupIDtoInternalID;
  RenderGroupNameContainer m_mRenderGroupNames;
};

}// namespace GL