#pragma once

#include <stdexcept>
#include <string>

#include <unordered_map>
#include <vector>
#include <list>

namespace GL {

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
  using RenderGroupID = unsigned int;
  using RenderGroup = std::vector<RenderObject>;

  friend class RenderingManager;

public:
  // Get the unique ID for the freshly-baked render group
  RenderGroupID getDistinctRenderGroupID(void) const noexcept;

  // Alias render group to some unique string
  void aliasRenderGroup(const RenderGroupID renderGroupID, const std::string &renderGroupName);

  // Push the render object to the render group using ID
  void pushToRenderGroup(const RenderGroupID renderGroupID, const RenderObject &renderObject) noexcept;
  // Push the render object to the render group using aliased Name
  void pushToRenderGroup(const std::string &renderGroupName, const RenderObject &renderObject) noexcept;

  // Disable the render group using its ID
  void enableRenderGroup(const RenderGroupID renderGroupID) noexcept;
  // Disable the render group using its Name
  void enableRenderGroup(const std::string &renderGroupName);

  // Disable the render group using its ID
  void releaseRenderGroup(const RenderGroupID renderGroupID) noexcept;
  // Disable render group using its aliased name
  void releaseRenderGroup(const std::string &renderGroupName);

private:
  //  Get the render group using its ID
  std::ptrdiff_t getRenderGroupByID(const RenderGroupID renderGroupID) const;

private:
  std::vector<RenderGroup> m_vRenderGroups;
  std::vector<RenderGroupID> m_vRenderGroupIDtoInternalID;
  std::list<RenderGroupID> m_vEnabledRenderGroups;
  std::unordered_map<std::string, RenderGroupID> m_mRenderGroupNames;
};

}// namespace GL