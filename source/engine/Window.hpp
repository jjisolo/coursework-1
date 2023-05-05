// This class define the `Window` class
//
// The `Window` class is the core class of the engine API.
#pragma once

#include "_EngineIncludes.hpp"

// This namespace is polluted with code for the game engine
namespace Engine
{
	// The window class is an abstraction under the GLFW's window routine.
	// the purpose of this class is to provide the engine with the window
	// callbacks, window creation and destroy. 
	class Window
	{
	private:
		 Window() = default;
		~Window();

	public:
		Window(const Window&)       = delete;
		Window(Window&&)            = delete;
		Window& operator=(Window&&) = delete;

		// This function is the way to realize the Singleton OOP programming pattern,
		// so that this class can only be instantiated only once.
		static Window& instance()
		{
			static Window _instance;
			return(_instance);
		}

		// The formal(by the parameters) callback that is used for the GLFW window callbacks.
		// This function(static formal function, that is calling an internal function) is kinda
		// workaround about the C background of the GLFW library.
		static void frameBufferResizeCallback(GLFWwindow* window, int newWidth, int newHeight)
		{
			UnreferencedParameter(window);
				
			// Call the internal calllback function(that has access to the class members).
			instance().frameBufferResizeCallbackImplementation(newWidth, newHeight);
		}

		// Get the pointer to the GLFW's GLFWwindow structure.
		inline GLFWwindow* getWindowPointerKHR(void) noexcept
		{
			return(m_ApplicationWindow);
		}

		// Get the current initialized window dimensions.
		inline glm::ivec2 getWindowDimensionsKHR(void)
		{
			return(m_WindowDimensions);
		}

	public:	
		// Initialize the window(using GLFW API)
		Error make(void) noexcept;

	private:
		// This function is called whenever the window is being resized.
		void frameBufferResizeCallbackImplementation(int newWidth, int newHeight) noexcept;

	private:
		GLFWwindow*  m_ApplicationWindow;
		glm::ivec2   m_WindowDimensions;
	};
}
