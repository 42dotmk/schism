#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>

#include "Log.h"

namespace Schism {
std::shared_ptr<spdlog::logger> Log::s_ClientLog;
std::shared_ptr<spdlog::logger> Log::s_CoreLog;

void Log::Init() {
    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        LOG_FILENAME, true));

    logSinks[0]->set_pattern("%^[%T] %n: %v%$");
    logSinks[1]->set_pattern("[%T] [%l] %n: %v");

    s_CoreLog = std::make_shared<spdlog::logger>("SCHISM", begin(logSinks),
                                                 end(logSinks));
    register_logger(s_CoreLog);
    s_CoreLog->set_level(spdlog::level::trace);
    s_CoreLog->flush_on(spdlog::level::trace);

    s_ClientLog =
        std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
    register_logger(s_ClientLog);
    s_ClientLog->set_level(spdlog::level::trace);
    s_ClientLog->flush_on(spdlog::level::trace);
}
}  // namespace Schism