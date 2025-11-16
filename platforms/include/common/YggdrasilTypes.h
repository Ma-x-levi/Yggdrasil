#pragma once

#include <array>
#include <memory>
#include <string_view>

#include <common/YggdrasilConfig.h>

namespace YggdrasilTypes
{
    enum class ReturnCode : int64_t
    {
        SUCCESS         =   0,
        TIMEOUT         =   1,
    };
} // namespace YggdrasilTypes

