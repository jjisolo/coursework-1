#include "Window.hpp"

static constexpr const char* DEF_WINDOW_TITLE  = "Game 101";
static constexpr const char* LOG_FILE_DEF_PATH = "logs/window_logs.txt";
static constexpr const char* LOG_LOGGER_NAME   = "Window";

namespace Engine
{
	void Window::setupLoggingSubsystem() noexcept
	{
		m_WindowLogger = spdlog::basic_logger_mt(LOG_LOGGER_NAME, LOG_FILE_DEF_PATH, true);
		m_WindowLogger->set_level(spdlog::level::err);
		m_WindowLogger->debug("Logging subsystem engaged");
	}

	Error Window::make(void) noexcept
	{
		setupLoggingSubsystem();
		m_WindowLogger->debug("Initializing GLFW window");

		auto primaryMonitor = glfwGetPrimaryMonitor();
		auto monitorMode    = glfwGetVideoMode(primaryMonitor);
		glfwWindowHint(GLFW_RED_BITS,     monitorMode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS,   monitorMode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS,    monitorMode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, monitorMode->refreshRate);

		m_ApplicationWindow = glfwCreateWindow(monitorMode->width, monitorMode->height, DEF_WINDOW_TITLE, glfwGetPrimaryMonitor(), nullptr);
		if (m_ApplicationWindow == nullptr) {
			m_WindowLogger->error("Unable to initialize GLFWwindow(create function returned NULL)");
			return(Engine::Error::ValidationError);
		}
		
		m_WindowLogger->debug("Setting up window callbacks");
		m_WindowDimensions = { monitorMode->width , monitorMode->height };
		glfwSetFramebufferSizeCallback(m_ApplicationWindow, &Window::frameBufferResizeCallback);

		return(Engine::Error::Ok);
	}

	void Window::frameBufferResizeCallbackImplementation(int newWidth, int newHeight) noexcept
	{
		const auto windowDimensions = getWindowDimensionsKHR();

		m_WindowLogger->debug(
			"Window resized from (%d; %d) -> (%d; %d)",
			windowDimensions.x, windowDimensions.y,
			newWidth, newHeight);
		glViewport(0, 0, newWidth, newHeight);
	}
}