#pragma once

//定义一个在日志后添加 文件名 函数名 行号 的宏定义
#ifndef suffix
#define suffix(msg)  std::string(msg).append("  <")\
        .append(__FILENAME__).append("> <").append(__func__)\
        .append("> <").append(std::to_string(__LINE__))\
        .append(">").c_str()
//#define suffix(msg)  std::string().append(" File:")\
//        .append(__FILENAME__).append("\", Func:\"").append(__func__)\
//        .append("()\", Line:\"").append(std::to_string(__LINE__)).append("\"").append(msg).c_str()
#endif
 
//在  spdlog.h   之前定义，才有效
#define SPDLOG_HEADER_ONLY
#ifndef SPDLOG_TRACE_ON
#define SPDLOG_TRACE_ON
#endif
 
#ifndef SPDLOG_DEBUG_ON
#define SPDLOG_DEBUG_ON
#endif

#include "spdlog/spdlog.h"
#include "spdlog/pattern_formatter.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
 
class Logger
{
 
public:
	static Logger& GetInstance() {
		static Logger m_instance;
		return m_instance;
	}
 
	std::shared_ptr<spdlog::logger>& GetLogger() { return global_logger; }
 
private:
	Logger() {
		std::vector<spdlog::sink_ptr> sinkList;
// #ifdef _DEBUG
		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		consoleSink->set_level(spdlog::level::debug);
		sinkList.push_back(consoleSink);
// #endif
		auto basicSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log/InferSDK.log");
		basicSink->set_level(spdlog::level::debug);
		sinkList.push_back(basicSink);

		global_logger = std::make_shared<spdlog::logger>("GlobalLog", begin(sinkList), end(sinkList));
 
		// 设置日志记录级别
#ifdef _DEBUG
		global_logger->set_level(spdlog::level::trace);
#else
		global_logger->set_level(spdlog::level::err);
#endif
		//设置 logger 格式[%^%L%$]
		spdlog::set_pattern(*global_logger.get(), "[%Y-%m-%d %H:%M:%S.%e] [%^%5l%$]  %v");
		//设置当出发 err 或更严重的错误时立刻刷新日志
		global_logger->flush_on(spdlog::level::err);
	}
 
	~Logger() {}
 
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
 
private:
	std::shared_ptr<spdlog::logger> global_logger;
};
 
//Logger& operator<<(Logger& log, const char* s);
//Logger& operator<<(Logger& log, const std::string& s);
 
#define LoggerPtr 			Logger::GetInstance().GetLogger() 
#define LTrace(msg,...) 	LoggerPtr->trace(suffix(msg),__VA_ARGS__);
//#define LDebug(msg,...) 	LoggerPtr->debug(suffix(msg),__VA_ARGS__);
#define LDebug(...) 		LoggerPtr->debug(__VA_ARGS__);
#define LInfo(...) 			LoggerPtr->info(__VA_ARGS__);
#define LWarn(...) 			LoggerPtr->warn(__VA_ARGS__);
#define LError(...) 		LoggerPtr->error(__VA_ARGS__);
#define LCritical(...) 		LoggerPtr->critical(__VA_ARGS__);
 
#define criticalif(b, ...)                        \
    do {                                       \
        if ((b)) {                             \
           LoggerPtr->critical(__VA_ARGS__); \
        }                                      \
    } while (0);
 