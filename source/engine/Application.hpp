#pragma once 

#include "expected"
#include "memory"

#include "utility/Error.hpp"
#include "Window.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
	
namespace One
{
	class Application
	{
	private:
		Application()  {};
		~Application() {};

	public:
		Application(Application const&)            = delete;
		Application& operator=(Application const&) = delete;

		static Application& instance()
		{
			static Application _instance;
			return(_instance);
		}

	public:
		Engine::Error make(void) noexcept;

		Engine::Error release(void) noexcept;

		Engine::Error mainLoop(void) noexcept;

	private:
		void setupLoggingSubsystem(void) noexcept;

		void processInput(GLFWwindow* windowPointer) noexcept;

	private:
		mutable std::shared_ptr<spdlog::logger> m_ApplicationLogger;
	};
}