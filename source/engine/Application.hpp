// This file declares the `Application` class.
// 
// The `Application` class is the core class of the engine API, and the only
// data structure that can be seen and used by the endpoint graphics API user.
#pragma once 

#include "_EngineIncludes.hpp"

#include "Window.hpp"
#include "ResourseManager.hpp"

#include "rendering/SpriteRenderer.hpp"
 
// This namespace holds the data, that should be accessed by the endpoint API user.
namespace One
{

	// This class is the core data structure of the graphics API.
	// 
	// It is responsible actions like: initializing internal API's, such as GLFW, GLAD,
	// GLM, SPDLOG; handle the user keyboard input, perform the main game endless loop.  
	class Application
	{
	private:
		Application()  {};
		~Application() {};

	public:
		Application(Application const&)            = delete;
		Application& operator=(Application const&) = delete;

		// This function is the way to realize the Singleton OOP programming pattern,
		// so that this class can only be instantiated only once.
		static Application& instance()
		{
			static Application _instance;
			return(_instance);
		}

	public:
		// Initialize the engine and its subsystems.
		Engine::Error make(void) noexcept;

		// Safely deallocate memory that was used during the game loop, quit the subsystems.
		Engine::Error release(void) noexcept;

		// Engage the game main loop, in which happens rendering, updating, input handling.
		Engine::Error mainLoop(void) noexcept;

	private:
		// Gather the input from the keyboard.
		void processInput(GLFWwindow* windowPointer) noexcept;

	private:
		std::shared_ptr<Engine::GFX::SpriteRenderer> m_SpriteRenderer;
	};
}