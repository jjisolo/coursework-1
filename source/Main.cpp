#include "engine/Application.hpp"
#include "engine/Logger.hpp"

int main(int argc, char** argv[])
{
	Engine::Logger::initialize();

	auto& applicationInstance = One::Application::instance();
	if (applicationInstance.make() == Engine::Error::Ok) {
		applicationInstance.mainLoop();
		applicationInstance.release();
	} 

	return(0);
}