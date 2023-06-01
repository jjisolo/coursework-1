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
	static constexpr const char*    _GLSL_VERSION               = "#version 150";
#elif __linux__
	static constexpr const unsigned _GLFW_CONTEXT_VERSION_MAJOR = 4;
	static constexpr const unsigned _GLFW_CONTEXT_VERSION_MINOR = 3;
	static constexpr const char*    _GLSL_VERSION               = "#version 150";
#elif _WIN32
	static constexpr const unsigned _GLFW_CONTEXT_VERSION_MAJOR = 4;
	static constexpr const unsigned _GLFW_CONTEXT_VERSION_MINOR = 6;
	static constexpr const char*    _GLSL_VERSION               = "#version 130";
#endif

// The default path to the sprites shaders
static constexpr const char* _SPRITESHADER_VERT_RELPATH = "shaders/engine_sprite_shader.vert";
static constexpr const char* _SPRITESHADER_FRAG_RELPATH = "shaders/engine_sprite_shader.frag";

static constexpr const char* _IMGUI_DEFAULT_FONT_RELPATH = "data/fonts/roboto_regular.ttf";

using namespace std;

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
		glfwWindowHint(GLFW_OPENGL_PROFILE,		   GLFW_OPENGL_ANY_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
		glfwWindowHint(GLFW_DEPTH_BITS,   24);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);

		// Set the window non-resizable
		glfwWindowHint(GLFW_RESIZABLE, false);

#ifdef __APPLE__
		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

		// Try to initialize the window abstraction.
		auto& windowInstance = Engine::Window::instance();

		if (FunctionSuccess(windowInstance.make))
		{
			Engine::Logger::m_ApplicationLogger->debug("Binding GLFW context");
			
			// Bind the context of the window, so all OpenGL functions are gonna affect this window instance
			glfwMakeContextCurrent(Engine::Window::instance().getWindowPointerKHR());
			
			// Set the callback on the mouse move action and press action.
			glfwSetInputMode          (Engine::Window::instance().getWindowPointerKHR(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
			glfwSetCursorPosCallback  (Engine::Window::instance().getWindowPointerKHR(), Application::setCursorPosCallback);
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

		// Workaround with HighDPI scaling. 
		m_monitorHighDPIScaleFactor = 1.0f;

#ifdef _WIN32
		// Get the scaling factor of the windows UI.
		GLfloat monitorScaleX, monitorScaleY;

		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		glfwGetMonitorContentScale(primaryMonitor, &monitorScaleX, &monitorScaleY);

		// And store it(used for ImGUI);
		m_monitorHighDPIScaleFactor = monitorScaleX;

		// If there's some scaling factor is set, store it and hint the GLFW window.
		if (monitorScaleX > 1 || monitorScaleY > 1)
			glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
#endif

		// Setup Dear ImGui
		Engine::Logger::m_ApplicationLogger->info("Initializing Dear ImGui");

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Initialize ImGui OpenGL bindings.
		Engine::Logger::m_ApplicationLogger->info("Initializing Dear ImGui OpenGL bindings");

		if (!ImGui_ImplGlfw_InitForOpenGL(Engine::Window::instance().getWindowPointerKHR(), true))
		{
			Engine::Logger::m_ApplicationLogger->error("Initialization failed");

			return(Engine::Error::InitializationError);
		}

		// Initialize ImGui OpenGL 3 bindings.
		if (!ImGui_ImplOpenGL3_Init(_GLSL_VERSION))
		{
			Engine::Logger::m_ApplicationLogger->error("Initialization failed");

			return(Engine::Error::InitializationError);
		}

		// Initialize the ImGui IO and load the fancy Google font.
		ImGuiIO& imguiIO = ImGui::GetIO();
		imguiIO.Fonts->AddFontFromFileTTF(_IMGUI_DEFAULT_FONT_RELPATH, 23.0f * m_monitorHighDPIScaleFactor, NULL, NULL);

		// pass the DPI scaling to the ImGUI renderer.
		applyImGuiStyles();

		// Setup ImGui Style
		ImGui::StyleColorsDark();

		const auto windowDimensions = windowInstance.getWindowDimensionsKHR();
		// Set the render area(viewport) where we want to render the object.
		glViewport(0, 0, windowDimensions.x, windowDimensions.y);
		
		// Enable the blending stage in the OpenGL rendering pipeline, to make objects appear
		// transparent on the screen.
		glEnable   (GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Load the shaders, that are used for rendering Sprites on the screen.
		Engine::Logger::m_ApplicationLogger->info("Loading shaders");
		Engine::ResourceManager::loadShader(_SPRITESHADER_VERT_RELPATH, _SPRITESHADER_FRAG_RELPATH, nullptr, "spriteShader");

		// Try to get the shaders from the global shader/texture storage, if the get request fails
		// exit the program.
		auto shaderWrapperOrError = Engine::ResourceManager::getShader("spriteShader");
		
		// Check the return value of the function.
		if (shaderWrapperOrError.has_value())
		{
			(*shaderWrapperOrError).useShader();

			// Setup the projection matrix for the current shader.
			auto projectionMatrix = glm::ortho(
				0.0f, 
				static_cast<GLfloat>(windowDimensions.x),
				static_cast<GLfloat>(windowDimensions.y),
				0.0f,
				-1.0f,
				1.0f
			);
			
			// Set this matrix as a uniform value for the shader.
			(*shaderWrapperOrError).useShader().setInteger("image", 0);
			(*shaderWrapperOrError).setMatrix4("projectionMatrix", projectionMatrix);
		}
		else 
		{
			Engine::Logger::m_ApplicationLogger->error("Program failed due to an shader loading error");

			// Or safely quit the program.
			auto windowPointerKHR = Engine::Window::instance().getWindowPointerKHR();

			if(windowPointerKHR) 
				glfwDestroyWindow(windowPointerKHR);

			glfwTerminate();
		
			return(Engine::Error::ValidationError);
		}
		
		Engine::Logger::m_ApplicationLogger->info("Creating sprite renderer");

		// Allocate and initialize the class that is used to render Sprites on the screen.
		m_SpriteRenderer = shared_ptr<Engine::GFX::SpriteRenderer>(new Engine::GFX::SpriteRenderer(*shaderWrapperOrError));
 
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

		// Process events and trigger theirs binded callbacks
		glfwPollEvents();

		// Render ImGui elements.
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap the front and back buffers of the current window.
		glfwSwapBuffers(windowPointer);

		return(Engine::Error::Ok);
	}

	void One::Application::processInput(GLFWwindow* windowPointer) noexcept
	{		
		// Handle the exit key binding
		if (glfwGetKey(windowPointer, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			m_escapeButtonPressed = true;
		else
			m_escapeButtonPressed = false;

		if (glfwGetMouseButton(windowPointer, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			m_mouseButtonPressed = true;
		else
			m_mouseButtonPressed = false;
	}

	Engine::Error One::Application::destroyGameEngine(void) noexcept
	{
		// Free all the resources that was allocated during the program execution
		Engine::ResourceManager::release();

		// Exit ImGui Library.
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// Exit GLFW library
		glfwTerminate();

		return(Engine::Error::Ok);
	}

	// Default initialization function.
	Engine::Error Application::onUserInitialize()
	{
		return(Engine::Error::Ok);
	}

	// Default destroy function.
	Engine::Error Application::onUserRelease()
	{
		return(Engine::Error::Ok);
	}

	Engine::Error Application::execute()
	{
		// Try initialize the game engine.
		if (FunctionSuccess(initializeGameEngine))
		{
			Engine::Logger::m_GameLogger->info("Executing onUserInitialize()");

			// Get the user code completion status.
			const Engine::Error userInitializationResult = onUserInitialize();
			
			// If it returned the error, just quit the program and report an error.
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

		// Continue the program execution until the user presses the quit button on the window
		// (in his window manager), or press the quit hotkey.
        double lastTimeStamp    = glfwGetTime();
        double currentTimeStamp = lastTimeStamp;

		while (!glfwWindowShouldClose(windowPointer))
		{
            // Get elapsed time.
            currentTimeStamp = glfwGetTime();
            m_elapsedTime    = lastTimeStamp - currentTimeStamp;
            lastTimeStamp    = currentTimeStamp;

			// Try initialize update.
			const Engine::Error userUpdateResult = onUserUpdate(m_elapsedTime);

			// If the functions encountered an error break the program and report an error.
			if(ValidationError(userUpdateResult) || InitializationError(userUpdateResult))
			{
				Engine::Logger::m_GameLogger->info("onUserUpdate returned error code");

				break;
			}

			// Update the actual engine.
			updateGameEngine();
		}

		Engine::Logger::m_GameLogger->info("Executing onUserDestroy()");

		// Try do userRelease code.
		const Engine::Error userReleaseResult = onUserRelease();

		// If the functions encountered an error.. report it and quit.
		if (ValidationError(userReleaseResult) || InitializationError(userReleaseResult))
		{
			Engine::Logger::m_GameLogger->info("Execution failed");
		
			// Destroy the actual engine.
			destroyGameEngine();

			return(Engine::Error::ValidationError);
		}

		// Destroy the actual engine.
		destroyGameEngine();

		return(Engine::Error::Ok);
	}

	// Default release function.
	Engine::Error Application::onUserUpdate(GLfloat elapsedTime)
	{
		return(Engine::Error::InitializationError);
	}
  
    // Default mouse move callback 
    Engine::Error Application::onMouseMove(double positionX, double positionY)
    {
      m_mousePositionX = positionX;
      m_mousePositionY = positionY;

	  return(Engine::Error::Ok);
    }

	void Application::applyImGuiStyles()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		// https://github.com/ocornut/imgui/issues/707#issuecomment-415097227
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);

		style.WindowPadding = ImVec2(8, 6);
		style.WindowRounding = 0.0f;
		style.FramePadding = ImVec2(5, 7);
		//style.FrameRounding            = 0.0f;
		style.ItemSpacing = ImVec2(5, 5);
		// style.ItemInnerSpacing         = ImVec2(1, 1);
		// style.TouchExtraPadding        = ImVec2(0, 0);
		// style.IndentSpacing            = 6.0f;
		// style.ScrollbarSize            = 12.0f;
		// style.ScrollbarRounding        = 16.0f;
		// style.GrabMinSize              = 20.0f;
		// style.GrabRounding             = 2.0f;
		// style.WindowTitleAlign.x = 0.50f;
		// style.FrameBorderSize = 0.0f;
		// style.WindowBorderSize = 1.0f;

		style.ScaleAllSizes(m_monitorHighDPIScaleFactor);
	}
}
