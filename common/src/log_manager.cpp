#include "log_manager.hpp"
#include "log.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <memory>


void logger::manager::Initialize(std::string log_file_path)
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] %v%$");

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file_path);
    file_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] %v%$");

    std::vector<spdlog::sink_ptr> sinks { console_sink, file_sink };
    auto logger = std::make_shared<spdlog::logger>(LOGGER_DEFAULT_NAME, sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
    spdlog::register_logger(logger);


}

void logger::manager::Shutdown()
{
    spdlog::shutdown();
}