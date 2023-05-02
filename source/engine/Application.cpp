#include "Application.hpp"

static constexpr const char* LOG_FILE_DEF_PATH = "logs/application_logs.txt";
static constexpr const char* LOG_LOGGER_NAME   = "Application";

static constexpr const unsigned _GLFW_CONTEXT_VERSION_MAJOR = 3;
static constexpr const unsigned _GLFW_CONTEXT_VERSION_MINOR = 3;
static constexpr const unsigned _GLFW_OPENGL_PROFILE = GLFW_OPENGL_CORE_PROFILE;

namespace One
{
	void Application::setupLoggingSubsystem(void) noexcept
	{
		m_ApplicationLogger = spdlog::basic_logger_mt(LOG_LOGGER_NAME, LOG_FILE_DEF_PATH, true);
		m_ApplicationLogger->set_level(spdlog::level::err);
		m_ApplicationLogger->debug("Logging subsystem engaged");
	} 

	Engine::Error Application::make(void) noexcept
	{
		setupLoggingSubsystem();

		if (glfwInit() != GLFW_TRUE) {
			m_ApplicationLogger->error("Unable to initialize GLFW(init function returned GLFW_FALSE)");
			return(Engine::Error::ValidationError);
		}
		m_ApplicationLogger->debug("GLFW library has been initialized");
			
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _GLFW_CONTEXT_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _GLFW_CONTEXT_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, _GLFW_OPENGL_PROFILE);

		#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		glfwWindowHint(GLFW_RESIZABLE, false);

		auto& windowInstance = Engine::Window::instance();
		if (windowInstance.make() == Engine::Error::Ok) {
			glfwMakeContextCurrent(Engine::Window::instance().getWindowPointerKHR());
			m_ApplicationLogger->debug("GLFW context has been binded");
		} else {
			glfwTerminate();
			m_ApplicationLogger->error("Encountered error on window creation, forwarding..");
			return(Engine::Error::ValidationError);
		}

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			m_ApplicationLogger->error("Unable to initialize GLAD(load function returned FALSE)");
			return(Engine::Error::ValidationError);
		}
		m_ApplicationLogger->debug("GLAD has been initialized");

		const auto windowDimensions = windowInstance.getWindowDimensionsKHR();
		glViewport(0, 0, windowDimensions.x, windowDimensions.y);
		
		glEnable   (GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_ApplicationLogger->debug("Application has been initialized");
		return(Engine::Error::Ok);
	}

	Engine::Error One::Application::mainLoop(void) noexcept
	{
		auto& windowInstance = Engine::Window::instance();
		auto  windowPointer  = windowInstance.getWindowPointerKHR();

		while (!glfwWindowShouldClose(windowPointer)) {
			processInput(windowPointer);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glfwSwapBuffers(windowPointer);
			glfwPollEvents();
		}

		return(Engine::Error::Ok);
	}

	void One::Application::processInput(GLFWwindow* windowPointer) noexcept
	{		
		if (glfwGetKey(windowPointer, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(windowPointer, true);
	}

	Engine::Error One::Application::release(void) noexcept
	{
		glfwTerminate();
		glfwDestroyWindow(Engine::Window::instance().getWindowPointerKHR());

		return(Engine::Error::Ok);
	}
}