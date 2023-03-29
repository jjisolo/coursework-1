#pragma once

// Defines for the libraries
#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../vendor/glad.h"

#include "rendering/GlUtils.hxx"
#include "rendering/RenderManager.hxx"

#include <iostream>
#include <cstdint>
#include <cassert>

namespace GL {
struct VertexBufferChain
{
  unsigned int vertexBufferObject;
  unsigned int vertexBufferArray;
};

class Window
{
public:
  explicit Window(glm::vec2 aWindowSize) : m_WindowSize(aWindowSize), m_bInitialized(false)
  {
  }

  bool initialize();

  void render() const;

  void release() const;

  void inline bindRenderManager(std::shared_ptr<RenderingManager> renderManager)
  {
    m_pBindedRenderManager = std::move(renderManager);
  }

private:
  std::shared_ptr<RenderingManager> m_pBindedRenderManager;

  GLFWwindow *m_pWindow;
  GLuint m_bInitialized;

  glm::vec2 m_WindowSize;
};
}// namespace GL
