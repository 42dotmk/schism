#pragma once

#include <spdlog/spdlog.h>

namespace Schism
{
	constexpr char LOG_FILENAME[] = "schism.log";

	class Log
	{
	public:
		// Init must be called at the start of the app.
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLog() { return s_CoreLog; }
		static std::shared_ptr<spdlog::logger>& GetClientLog() { return s_ClientLog; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLog;
		static std::shared_ptr<spdlog::logger> s_ClientLog;
	};
}

#define SC_CORE_TRACE(...)		::Schism::Log::GetCoreLog()->trace(__VA_ARGS__)
#define SC_CORE_INFO(...)		::Schism::Log::GetCoreLog()->info(__VA_ARGS__)
#define SC_CORE_WARN(...)		::Schism::Log::GetCoreLog()->warn(__VA_ARGS__)
#define SC_CORE_ERROR(...)		::Schism::Log::GetCoreLog()->error(__VA_ARGS__)
#define SC_CORE_CRITICAL(...)	::Schism::Log::GetCoreLog()->critical(__VA_ARGS__)
