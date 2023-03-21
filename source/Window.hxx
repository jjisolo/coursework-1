#pragma once

// Defines for the libraries
#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../vendor/glad.h"

#include "rendering/GlUtils.hxx"

#include <iostream>
#include <cstdint>
#include <cassert>

namespace gl {

    class Window 
    {
    public:
        explicit Window(glm::vec2 aWindowSize):
            m_WindowSize(aWindowSize), m_bInitialized(false)
        {
        }

        bool initialize();

        void render() const;

        void release() const;

    private:
        GLFWwindow *m_pWindow;
        GLuint      m_bInitialized;

        Gluint      m_uVertexBufferObjects;
        Gluint      m_uVertexBufferArray;

        glm::vec2   m_WindowSize;
    };
}