#include "Window.hpp"
#include "Logger.hpp"

static constexpr const char* DEF_WINDOW_TITLE  = "Game 101";

namespace Engine
{
	Error Window::make(void) noexcept
	{
		Logger::m_ApplicationLogger->debug("Initializing GLFW window");

		// Get the primary monitor(if the user uses multi-monitor setup), retrieve its mode.
		auto primaryMonitor = glfwGetPrimaryMonitor();
		auto monitorMode    = glfwGetVideoMode(primaryMonitor);

		// Adjust the window options relating on this information.
		glfwWindowHint(GLFW_RED_BITS,     monitorMode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS,   monitorMode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS,    monitorMode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, monitorMode->refreshRate);

		// Create the window using the retrieved monitor information.
		m_ApplicationWindow = glfwCreateWindow(monitorMode->width, monitorMode->height, DEF_WINDOW_TITLE, glfwGetPrimaryMonitor(), nullptr);
		
		// Assert that the window is created with no errors(if not raise an error that leads to the program exit).
		if (m_ApplicationWindow == nullptr)
		{
			Logger::m_ApplicationLogger->error("Unable to initialize GLFWwindow(create function returned NULL)");
			
			return(Engine::Error::ValidationError);
		}

		// Set the Vertical synchronization ON.
		glfwSwapInterval(1);

		Logger::m_ApplicationLogger->debug("Setting up window callbacks");
		
		// Track the window dimensions internally
		m_WindowDimensions = { monitorMode->width , monitorMode->height };

		// Setup window callbacks
		glfwSetFramebufferSizeCallback(m_ApplicationWindow, &Window::frameBufferResizeCallback);

		return(Engine::Error::Ok);
	}

	void Window::frameBufferResizeCallbackImplementation(int newWidth, int newHeight) noexcept
	{
		// Get the current(non-resized) window dimensions.
		const auto windowDimensions = getWindowDimensionsKHR();

		Logger::m_ApplicationLogger->debug(
			"Window resized from (%d; %d) -> (%d; %d)",
			windowDimensions.x, windowDimensions.y,
			newWidth, newHeight);
		
		// Update the viewport(drawing area) according to the new window size
		glViewport(0, 0, newWidth, newHeight);
	}

	Window::~Window()
	{
		// Safely release the memory that GLFW's GLFWindow structure was using.
		glfwDestroyWindow(m_ApplicationWindow);
	}
}
