#pragma once

#include "expected"
#include "memory"

#include "utility/Error.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"

namespace Engine
{
	class Window
	{
	private:
		 Window() = default;
		~Window();

	public:
		Window(const Window&)       = delete;
		Window(Window&&)            = delete;
		Window& operator=(Window&&) = delete;

		static Window& instance()
		{
			static Window _instance;
			return(_instance);
		}

		static void frameBufferResizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
			instance().frameBufferResizeCallbackImplementation(newWidth, newHeight);
		}

		inline GLFWwindow* getWindowPointerKHR(void) noexcept {
			return(m_ApplicationWindow);
		}

		inline glm::ivec2 getWindowDimensionsKHR(void) {
			return(m_WindowDimensions);
		}

	public:	
		Error make(void) noexcept;

	private:
		void frameBufferResizeCallbackImplementation(int newWidth, int newHeight) noexcept;

	private:
		GLFWwindow*  m_ApplicationWindow;
		glm::ivec2   m_WindowDimensions;
	};
}