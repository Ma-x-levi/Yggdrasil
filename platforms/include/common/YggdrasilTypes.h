#pragma once

#include <array>
#include <memory>
#include <functional>
#include <string_view>


#include <common/YggdrasilConfig.h>

namespace YggdrasilTypes
{

    enum class ReturnCode : uint64_t
    {
        
        SUCCESS              =   0,
        NONSUPPORT           =   1,
        UNKNOWN_FAILURE      =   2,
        TIMEOUT              =   3,
        INVALID_ARGUMENT     =   4,
    };


    constexpr std::string_view LOGGER_LEVEL_ERROR_COLOR    = "\033[0;31m";
    constexpr std::string_view LOGGER_LEVEL_WARN_COLOR     = "\033[0;33m";
    constexpr std::string_view LOGGER_LEVEL_INFO_COLOR     = "\033[0;32m";
    constexpr std::string_view LOGGER_LEVEL_DEBUG_COLOR    = "\033[0;37m";
    constexpr std::string_view LOGGER_LEVEL_DEFAULT_COLOR  = "\033[0m";

    using LOGGER_DEFAULT_FORMAT_SIZE    = std::integral_constant<int, 128>;
    using LOGGER_DEFAULT_PARAMETER_SIZE = std::integral_constant<int, 512>;


    using HandleName = std::array<char, YGGDRASIL_DEFAULT_NAME_SIZE>;

} // namespace YggdrasilTypes

