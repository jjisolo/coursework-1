#include "Application.hpp"
#include "Logger.hpp"
#include "Sprite.hpp"

static constexpr const unsigned _GLFW_CONTEXT_VERSION_MAJOR = 3;
static constexpr const unsigned _GLFW_CONTEXT_VERSION_MINOR = 3;
static constexpr const unsigned _GLFW_OPENGL_PROFILE = GLFW_OPENGL_CORE_PROFILE;

namespace One
{
	Engine::Error Application::make(void) noexcept
	{
		Engine::Logger::m_ApplicationLogger -> info("Engine started\n\n");
		Engine::Logger::m_GraphicsLogger    -> info("Graphics logger started");

		if (glfwInit() != GLFW_TRUE) {
			Engine::Logger::m_ApplicationLogger->error("Unable to initialize GLFW(init function returned GLFW_FALSE)");
			return(Engine::Error::ValidationError);
		}
		Engine::Logger::m_ApplicationLogger->debug("GLFW library has been initialized");
			
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
			Engine::Logger::m_ApplicationLogger->debug("GLFW context has been binded");
		} else {
			glfwTerminate();
			Engine::Logger::m_ApplicationLogger->error("Encountered error on window creation, forwarding..");
			return(Engine::Error::ValidationError);
		}

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			Engine::Logger::m_ApplicationLogger->error("Unable to initialize GLAD(load function returned FALSE)");
			return(Engine::Error::ValidationError);
		}
		Engine::Logger::m_ApplicationLogger->debug("GLAD has been initialized");

		const auto windowDimensions = windowInstance.getWindowDimensionsKHR();
		glViewport(0, 0, windowDimensions.x, windowDimensions.y);
		
		glEnable   (GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		Engine::Core::ResourceManager::loadShader("shaders/shader0.vert", "shaders/shader0.frag", nullptr, "spriteShader");
		glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<GLfloat>(windowDimensions.x), static_cast<GLfloat>(windowDimensions.y), 0.0f, -1.0f, 1.0f);
		
		auto shaderWrapperOrError = Engine::Core::ResourceManager::getShader("spriteShader");
		if (shaderWrapperOrError.has_value()) {
			(*shaderWrapperOrError).useShader();
			(*shaderWrapperOrError).setMatrix4("projectionMatrix", projectionMatrix);
		}
		else {
			Engine::Logger::m_ApplicationLogger->error("Program failed due to an shader loading error");
			
			// todo: EXIT AND RELEASING
			return(Engine::Error::ValidationError);
		}
		
		m_SpriteRenderer = std::shared_ptr<Engine::GFX::SpriteRenderer>(new Engine::GFX::SpriteRenderer(*shaderWrapperOrError));

		Engine::Logger::m_ApplicationLogger->info("Application has been initialized");
		return(Engine::Error::Ok);
	}

	Engine::Error One::Application::mainLoop(void) noexcept
	{
		auto& windowInstance = Engine::Window::instance();
		auto  windowPointer  = windowInstance.getWindowPointerKHR();

		Engine::GFX::Sprite cardSprite;
		cardSprite.setSpriteColor({ 0.0f, 1.0f, 0.0f });
		cardSprite.setSpritePosition({ 200.0f, 200.0f });
		cardSprite.setSpriteSize({ 400.0f, 300.0f });
		cardSprite.setSpriteRotation(15.0f);
		cardSprite.setSpriteTextureName("cardSprite");
		cardSprite.setSpriteTexturePath("data/cards.png");

		while (!glfwWindowShouldClose(windowPointer)) {
			processInput(windowPointer);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			cardSprite.render(m_SpriteRenderer);

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