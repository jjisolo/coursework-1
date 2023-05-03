#include "Window.hpp"
#include "Logger.hpp"

static constexpr const char* DEF_WINDOW_TITLE  = "Game 101";

namespace Engine
{
	Error Window::make(void) noexcept
	{
		Logger::m_ApplicationLogger->debug("Initializing GLFW window");

		// Get the primary monitor(if the user uses multi-monitor setup), retrieve its 
		// mode and adjust the window options relating on this information.
		auto primaryMonitor = glfwGetPrimaryMonitor();
		auto monitorMode    = glfwGetVideoMode(primaryMonitor);
		glfwWindowHint(GLFW_RED_BITS,     monitorMode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS,   monitorMode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS,    monitorMode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, monitorMode->refreshRate);

		// Create the window using the retrieved monitor information.
		m_ApplicationWindow = glfwCreateWindow(monitorMode->width, monitorMode->height, DEF_WINDOW_TITLE, glfwGetPrimaryMonitor(), nullptr);
		
		// Assert that the window is created with no errors(if not raise an error that
		// leads to the program exit).
		if (m_ApplicationWindow == nullptr) {
			Logger::m_ApplicationLogger->error("Unable to initialize GLFWwindow(create function returned NULL)");
			return(Engine::Error::ValidationError);
		}
		
		Logger::m_ApplicationLogger->debug("Setting up window callbacks");
		m_WindowDimensions = { monitorMode->width , monitorMode->height };

		// Setup window callbacks
		glfwSetFramebufferSizeCallback(m_ApplicationWindow, &Window::frameBufferResizeCallback);

		return(Engine::Error::Ok);
	}

	void Window::frameBufferResizeCallbackImplementation(int newWidth, int newHeight) noexcept
	{
		const auto windowDimensions = getWindowDimensionsKHR();

		Logger::m_ApplicationLogger->debug(
			"Window resized from (%d; %d) -> (%d; %d)",
			windowDimensions.x, windowDimensions.y,
			newWidth, newHeight);
		glViewport(0, 0, newWidth, newHeight);
	}

	Window::~Window()
	{
		// Safely release the memory that GLFW's GLFWindow structure was using.
		glfwDestroyWindow(m_ApplicationWindow);
	}
}