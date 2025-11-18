#pragma once

#include <array>
#include <memory>
#include <functional>
#include <string_view>


#include <common/YggdrasilConfig.h>

namespace YggdrasilTypes
{

    enum class ReturnCode : int64_t
    {
        SUCCESS         =   0,
        TIMEOUT         =   1,
    };


    constexpr std::string_view LOGGER_LEVEL_ERROR_COLOR    = "\033[0;31m";
    constexpr std::string_view LOGGER_LEVEL_WARN_COLOR     = "\033[0;33m";
    constexpr std::string_view LOGGER_LEVEL_INFO_COLOR     = "\033[0;32m";
    constexpr std::string_view LOGGER_LEVEL_DEBUG_COLOR    = "\033[0;37m";
    constexpr std::string_view LOGGER_LEVEL_DEFAULT_COLOR  = "\033[0m";

} // namespace YggdrasilTypes

