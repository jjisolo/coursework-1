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

struct RenderObject
{
  RenderObject(const std::string &texturePath, const unsigned int textureDescriptor) : texturePath(texturePath), textureDescriptor(textureDescriptor), isInitialized(false)
  {
  }

  std::string texturePath;
  unsigned int textureDescriptor;
  bool isInitialized;
};


// Render scene is a container for a different render groups(which are build
// from the groups of render objects). The common practise is to split the
// render scene in different render groups and then enable or disable some
// of them.
//
// Render group management is handled using render group ID's and names
// (which can be aliased to some ID).
class RenderScene
{
  friend class RenderingManager;

public:
  // Get the unique ID for the freshly-baked render group
  RenderGroupID getDistinctRenderGroupID(void) const noexcept;

  // Alias render group to some unique string
  void aliasRenderGroup(const RenderGroupID &renderGroupID, const std::string &renderGroupName) const noexcept;

  // Push the texture descriptor to the render group using ID
  void pushToRenderGroup(const RenderGroupID &renderGroupID, const RenderObject &renderObject) const noexcept;

  // Push the texture descriptor to the render group using aliased Name
  void pushToRenderGroup(const std::string &renderGroupName, const RenderObject &renderObject) const noexcept;

  // Enable the current render group
  void enableRenderGroup(const RenderGroupID &renderGroupID) const noexcept;

private:
  RenderGroupContainer m_vRenderGroups;
  RenderGroupIDList m_lEnabledRenderGroups;
  RenderGroupIDContainer m_vRenderGroupIDtoInternalID;
  RenderGroupNameContainer m_mRenderGroupNames;
};

}// namespace GL