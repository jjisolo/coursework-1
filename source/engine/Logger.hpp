#pragma once

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog/common.h"
#include "spdlog/logger.h"

namespace Engine
{
	static class Logger
	{
	public:
		static void initialize();

		static std::shared_ptr<spdlog::logger> m_ApplicationLogger;
		static std::shared_ptr<spdlog::logger> m_GraphicsLogger;
		static std::shared_ptr<spdlog::logger> m_GameLogger;

	private:
		static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> m_AppllicationSink;
		static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> m_GraphicsSink;
		static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> m_GeneralSink;
		static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> m_GameSink;
	};
}