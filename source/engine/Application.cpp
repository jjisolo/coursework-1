// This file implements the `Application` class
//
// The `Application` class is the core class of the engine API, and the only
// data structure that can be seen and used by the endpoint graphics API user.
#include "Application.hpp"
#include "Logger.hpp"
#include "Sprite.hpp"

// Apple does not support modern OpenGL.
#ifdef __APPLE__
	static constexpr const unsigned _GLFW_CONTEXT_VERSION_MAJOR = 3;
	static constexpr const unsigned _GLFW_CONTEXT_VERSION_MINOR = 3;
#else
	static constexpr const unsigned _GLFW_CONTEXT_VERSION_MAJOR = 4;
	static constexpr const unsigned _GLFW_CONTEXT_VERSION_MINOR = 6;
#endif

namespace One
{
	Engine::Error Application::initializeGameEngine(void) noexcept
	{
		Engine::Logger::initialize();

		Engine::Logger::m_ApplicationLogger -> info("Engine started\n\n");
		Engine::Logger::m_GraphicsLogger    -> info("Graphics logger started");

		// Try to initialize the GLFW platform layer.
		if (glfwInit() != GLFW_TRUE)
		{
			Engine::Logger::m_ApplicationLogger->error("Unable to initialize GLFW(init function returned GLFW_FALSE)");
			return(Engine::Error::ValidationError);
		}
		Engine::Logger::m_ApplicationLogger->debug("GLFW library has been initialized");
		
		// Set the OpenGL context and profile versions.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _GLFW_CONTEXT_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _GLFW_CONTEXT_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);

		#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		// Set the window non-resizable
		glfwWindowHint(GLFW_RESIZABLE, false);

		// Try to initialize the window abstraction.
		auto& windowInstance = Engine::Window::instance();

		if (windowInstance.make() == Engine::Error::Ok)
		{
			Engine::Logger::m_ApplicationLogger->debug("Binding GLFW context");
			
			// Bind the context of the window, so all OpenGL functions are gonna affect this window instance
			glfwMakeContextCurrent(Engine::Window::instance().getWindowPointerKHR());
		} else
		{
			Engine::Logger::m_ApplicationLogger->error("Encountered error on window creation");
			
			glfwTerminate();

			return(Engine::Error::ValidationError);
		}

		// Load all OpenGL functions using the glfw loader function.
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Engine::Logger::m_ApplicationLogger->error("Unable to initialize GLAD(load function returned FALSE)");

			glfwTerminate();

			return(Engine::Error::ValidationError);
		}
		Engine::Logger::m_ApplicationLogger->debug("GLAD has been initialized");

		const auto windowDimensions = windowInstance.getWindowDimensionsKHR();

		// Set the render area(viewport) where we want to render the object.
		glViewport(0, 0, windowDimensions.x, windowDimensions.y);
		
		// Enable the blending stage in the OpenGL rendering pipeline, to make objects appear
		// transparent on the screen.
		glEnable   (GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Load the shaders, that are used for rendering Sprites on the screen.
		Engine::Logger::m_ApplicationLogger->info("Loading shaders");
		Engine::ResourceManager::loadShader("shaders/shader0.vert", "shaders/shader0.frag", nullptr, "spriteShader");

		// Try to get the shaders from the global shader/texture storage, if the get request fails
		// exit the program.
		auto shaderWrapperOrError = Engine::ResourceManager::getShader("spriteShader");
		if (shaderWrapperOrError.has_value())
		{
			(*shaderWrapperOrError).useShader();

			auto projectionMatrix = glm::ortho(
				0.0f, 
				static_cast<GLfloat>(windowDimensions.x),
				static_cast<GLfloat>(windowDimensions.y),
				0.0f,
				-1.0f,
				1.0f
			);
			
			(*shaderWrapperOrError).setMatrix4("projectionMatrix", projectionMatrix);
		}
		else 
		{
			Engine::Logger::m_ApplicationLogger->error("Program failed due to an shader loading error");

			glfwDestroyWindow(Engine::Window::instance().getWindowPointerKHR());
			glfwTerminate();
		
			return(Engine::Error::ValidationError);
		}
		
		Engine::Logger::m_ApplicationLogger->info("Creating sprite renderer");

		// Allocate and initialize the class that is used to render Sprites on the screen.
		m_SpriteRenderer = std::shared_ptr<Engine::GFX::SpriteRenderer>(new Engine::GFX::SpriteRenderer(*shaderWrapperOrError));
 
		Engine::Logger::m_ApplicationLogger->info("Application is initialized");

		return(Engine::Error::Ok);
	}

	Engine::Error One::Application::updateGameEngine(void) noexcept
	{
		// Get the reference to the window instance, and GLFW window instance pointer, so
		// not to call the singleton ::instance() function every loop iteration.
		auto& windowInstance = Engine::Window::instance();
		auto  windowPointer  = windowInstance.getWindowPointerKHR();

		// Catch and process keyboard input from the user.
		processInput(windowPointer);

		// Swap the front and back buffers of the current window.
		glfwSwapBuffers(windowPointer);

		// Process events and trigger theirs binded callbacks
		glfwPollEvents();

		return(Engine::Error::Ok);
	}

	void One::Application::processInput(GLFWwindow* windowPointer) noexcept
	{		
		// Handle the exit key binding
		if (glfwGetKey(windowPointer, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
			glfwSetWindowShouldClose(windowPointer, true);
	}

	Engine::Error One::Application::destroyGameEngine(void) noexcept
	{
		// Free all the resources that was allocated during the program execution
		Engine::ResourceManager::release();

		// Exit GLFW library
		glfwTerminate();

		return(Engine::Error::Ok);
	}

	Engine::Error Application::onUserInitialize()
	{
		return(Engine::Error::InitializationError);
	}

	Engine::Error Application::onUserRelease()
	{
		return(Engine::Error::InitializationError);
	}

	Engine::Error Application::execute()
	{
		if (FunctionSuccess(initializeGameEngine))
		{
			Engine::Logger::m_GameLogger->info("Executing onUserInitialize()");

			const Engine::Error userInitializationResult = onUserInitialize();

			if (ValidationError(userInitializationResult) || InitializationError(userInitializationResult))
			{
				Engine::Logger::m_GameLogger->error("Execution failed");

				destroyGameEngine();

				return(Engine::Error::ValidationError);
			}
		}


		// Get the reference to the window instance, and GLFW window instance pointer, so
		// not to call the singleton ::instance() function every loop iteration.
		auto& windowInstance = Engine::Window::instance();
		auto  windowPointer  = windowInstance.getWindowPointerKHR();

		Engine::Logger::m_GameLogger->error("Starting game loop");

		while (!glfwWindowShouldClose(windowPointer))
		{
			const Engine::Error userUpdateResult = onUserUpdate(1.0f);

			if(ValidationError(userUpdateResult) || InitializationError(userUpdateResult))
			{
				Engine::Logger::m_GameLogger->info("onUserUpdate returned error code");

				break;
			}

			updateGameEngine();
		}

		Engine::Logger::m_GameLogger->info("Executing onUserDestroy()");

		const Engine::Error userReleaseResult = onUserRelease();

		if (ValidationError(userReleaseResult) || InitializationError(userReleaseResult))
		{
			Engine::Logger::m_GameLogger->info("Execution failed");

			return(Engine::Error::ValidationError);
		}

		destroyGameEngine();

		return(Engine::Error::Ok);
	}

	Engine::Error Application::onUserUpdate(GLfloat elapsedTime)
	{
		return(Engine::Error::InitializationError);
	}
}
