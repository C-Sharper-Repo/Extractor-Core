#pragma once

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace ExCore {
    class Log {
    public:
        static void init();
        static std::shared_ptr<spdlog::logger>& getLogger() { return _logger; };
    private:
        static std::shared_ptr<spdlog::logger> _logger;
    };
}

//Core Log Macros
#define EX_CORE_FATAL(...) ::ExCore::Log::getLogger()->fatal(__VA_ARGS__)
#define EX_CORE_ERROR(...) ::ExCore::Log::getLogger()->error(__VA_ARGS__)
#define EX_CORE_WARN(...)  ::ExCore::Log::getLogger()->warn(__VA_ARGS__)
#define EX_CORE_INFO(...)  ::ExCore::Log::getLogger()->info(__VA_ARGS__)
#define EX_CORE_TRACE(...) ::ExCore::Log::getLogger()->trace(__VA_ARGS__)