#include "engine/Application.hpp"

int main(int argc, char** argv[])
{
	spdlog::info("Program start");

	auto& applicationInstance = One::Application::instance();
	if (applicationInstance.make() == Engine::Error::Ok) {
		applicationInstance.mainLoop();
		applicationInstance.release();
	} 

	spdlog::info("Program exit");
	return(0);
}