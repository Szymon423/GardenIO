#pragma once

#include "spdlog/spdlog.h"

#define LOGGER_DEFAULT_NAME "GadrenIO_Logger"

#define LOG_TRACE(...) if (spdlog::get(LOGGER_DEFAULT_NAME) != nullptr) { spdlog::get(LOGGER_DEFAULT_NAME)->trace(__VA_ARGS__); }
#define LOG_DEBUG(...) if (spdlog::get(LOGGER_DEFAULT_NAME) != nullptr) { spdlog::get(LOGGER_DEFAULT_NAME)->debug(__VA_ARGS__); }
#define LOG_INFO(...) if (spdlog::get(LOGGER_DEFAULT_NAME) != nullptr) { spdlog::get(LOGGER_DEFAULT_NAME)->info(__VA_ARGS__); }
#define LOG_WARN(...) if (spdlog::get(LOGGER_DEFAULT_NAME) != nullptr) { spdlog::get(LOGGER_DEFAULT_NAME)->warn(__VA_ARGS__); }
#define LOG_ERROR(...) if (spdlog::get(LOGGER_DEFAULT_NAME) != nullptr) { spdlog::get(LOGGER_DEFAULT_NAME)->error(__VA_ARGS__); }
#define LOG_CRITICAL(...) if (spdlog::get(LOGGER_DEFAULT_NAME) != nullptr) { spdlog::get(LOGGER_DEFAULT_NAME)->critical(__VA_ARGS__); }