#include "Logger.hpp"

std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Engine::Logger::m_ApplicationSink;
std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Engine::Logger::m_GraphicsSink;
std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Engine::Logger::m_GeneralSink;
std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Engine::Logger::m_GameSink;

std::shared_ptr<spdlog::logger> Engine::Logger::m_ApplicationLogger;

std::shared_ptr<spdlog::logger> Engine::Logger::m_GraphicsLogger;
std::shared_ptr<spdlog::logger> Engine::Logger::m_ResourceLogger;

std::shared_ptr<spdlog::logger> Engine::Logger::m_GameLogger;

namespace Engine
{
	void Logger::initialize()
	{
		m_ApplicationSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/application.log");
		m_GraphicsSink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/graphics.log"   );
		m_GeneralSink     = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/general.log"    );
	
		std::vector<spdlog::sink_ptr> applicationSinks;
		applicationSinks.push_back(m_ApplicationSink);
		applicationSinks.push_back(m_GeneralSink);

		std::vector<spdlog::sink_ptr> graphicsSink;
		graphicsSink.push_back(m_GraphicsSink);
		graphicsSink.push_back(m_GeneralSink);

		std::vector<spdlog::sink_ptr> gameSink;
		gameSink.push_back(m_GeneralSink);
		gameSink.push_back(m_GameSink);

		m_ApplicationLogger = std::make_unique<spdlog::logger>("Application General",  applicationSinks.begin(), applicationSinks.end());
		m_GraphicsLogger    = std::make_unique<spdlog::logger>("Graphics General",     graphicsSink.begin(),     graphicsSink.end());
		m_ResourceLogger    = std::make_unique<spdlog::logger>("Resource Manager",     graphicsSink.begin(), graphicsSink.end());
		m_GameLogger        = std::make_unique<spdlog::logger>("Game General",         gameSink.begin(),         gameSink.end());

		m_ApplicationLogger -> set_level(spdlog::level::trace);
		m_GraphicsLogger    -> set_level(spdlog::level::trace);
		m_ResourceLogger    -> set_level(spdlog::level::trace);
		m_GameLogger        -> set_level(spdlog::level::trace);
	}
}