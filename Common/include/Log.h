#ifndef LOG_COMMON_H
#define LOG_COMMON_H
#include <qapplication.h>
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/spdlog.h>

namespace Log
{
void initLog(const QApplication& app, const spdlog::level::level_enum& level)
{
  spdlog::init_thread_pool(8192, 1); // 异步队列大小，线程数

  std::vector<spdlog::sink_ptr> sinks;
  sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  sinks.push_back(
    std::make_shared<spdlog::sinks::rotating_file_sink_mt>("app.log", 1024 * 1024 * 5, 3));

  auto logger = std::make_shared<spdlog::async_logger>(
    "app", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

  logger->set_level(level);
  logger->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%# %!] %v%$");
  spdlog::set_default_logger(logger);

  SPDLOG_INFO("Application started");

  app.connect(&app, &QCoreApplication::aboutToQuit, []() { spdlog::shutdown(); });
}

void destroyLog()
{
  spdlog::shutdown();
}
}
#endif
