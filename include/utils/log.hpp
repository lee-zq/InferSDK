#ifndef  __LOGGER_H__
#define  __LOGGER_H__

#include <iostream>
#include <string>
#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

class Logger
{
public:
    Logger() {}
    ~Logger() {}
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    int init(const std::string& file_log_path="log/infer_sdk.log", int file_log_level=spdlog::level::debug){
        spdlog::set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");
        auto console_logger = spdlog::stdout_color_mt("console_logger");
        console_logger->set_level(spdlog::level::warn);
        auto rotating_logger = spdlog::rotating_logger_mt("rotating_logger", file_log_path.c_str(), 1024 * 1024 * 5, 1);
        rotating_logger->set_level(spdlog::level::debug);

        logger_ = std::make_shared<spdlog::logger>(std::string("multi_sink"), {console_logger, rotating_logger});
        logger_->set_level(spdlog::level::debug);

        return 0;
    }
    spdlog::logger* get(){
        return logger_.get();
    }
public:
    std::shared_ptr<spdlog::logger> logger_ = nullptr;
};

static Logger logger;

#define LOG_DEBUG(...) logger.get()->debug(__VA_ARGS__)
#define LOG_INFO(...) logger.get()->info(__VA_ARGS__)
#define LOG_WARN(...) logger.get()->warn(__VA_ARGS__)
#define LOG_ERROR(...) logger.get()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) logger.get()->critical(__VA_ARGS__)

#endif // __LOGGER_H__
