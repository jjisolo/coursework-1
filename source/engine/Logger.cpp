// This file implements the `Logger` class.
#include "Logger.hpp"

// Initialize the static sinks that was declared in the `Logger` class.
std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Engine::Logger::m_ApplicationSink;
std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Engine::Logger::m_GraphicsSink;
std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Engine::Logger::m_GeneralSink;
std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Engine::Logger::m_GameSink;

// Initialize the static loggers that was declared in the `Logger` class.
std::shared_ptr<spdlog::logger> Engine::Logger::m_ApplicationLogger;
std::shared_ptr<spdlog::logger> Engine::Logger::m_GraphicsLogger;
std::shared_ptr<spdlog::logger> Engine::Logger::m_ResourceLogger;
std::shared_ptr<spdlog::logger> Engine::Logger::m_GameLogger;

static constexpr const char* LOGGER_APP_LOGFILE = "logs/application.log";
static constexpr const char* LOGGER_GFX_LOGFILE = "logs/graphics.log";
static constexpr const char* LOGGER_GEN_LOGFILE = "logs/general.log";
static constexpr const char* LOGGER_GAM_LOGFILE = "logs/game.log";


namespace Engine
{
	void Logger::initialize()
	{
		// Bind the sinks to the corresponding files.
		m_ApplicationSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOGGER_APP_LOGFILE);
		m_GraphicsSink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOGGER_GFX_LOGFILE);
		m_GeneralSink     = std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOGGER_GEN_LOGFILE);
		m_GameSink        = std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOGGER_GAM_LOGFILE);

		// Groups the sinks, so that each logger writes into the logger-specific and
		// the `general` sinks.
		std::vector<spdlog::sink_ptr> applicationSinks;
		applicationSinks.push_back(m_ApplicationSink);
		applicationSinks.push_back(m_GeneralSink);

		std::vector<spdlog::sink_ptr> graphicsSink;
		graphicsSink.push_back(m_GraphicsSink);
		graphicsSink.push_back(m_GeneralSink);

		std::vector<spdlog::sink_ptr> gameSink;
		gameSink.push_back(m_GeneralSink);
		gameSink.push_back(m_GameSink);

		// Create the loggers and assign theirs names.
		m_ApplicationLogger = std::make_unique<spdlog::logger>("Application General",  applicationSinks.begin(), applicationSinks.end());
		m_GraphicsLogger    = std::make_unique<spdlog::logger>("Graphics General",     graphicsSink.begin(),     graphicsSink.end());
		m_ResourceLogger    = std::make_unique<spdlog::logger>("Resource Manager",     graphicsSink.begin(),     graphicsSink.end());
		m_GameLogger        = std::make_unique<spdlog::logger>("Game General",         gameSink.begin(),         gameSink.end());

		// Set the logging level for each logger(the trace level means that
		// we are printing all the messages into the sink).
		m_ApplicationLogger -> set_level(spdlog::level::trace);
		m_GraphicsLogger    -> set_level(spdlog::level::trace);
		m_ResourceLogger    -> set_level(spdlog::level::trace);
		m_GameLogger        -> set_level(spdlog::level::trace);
	}
}