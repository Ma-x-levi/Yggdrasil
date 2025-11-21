#include <stdarg.h>

#include "NanoYggdrasil.h"

YggdrasilTypes::ReturnCode NanoYggdrasil::RegisterHalLampHandle(std::reference_wrapper<YggdrasilHalLampHandleBase> handle) noexcept
{
    _halLampHandleList.push_back(handle);
    return YggdrasilTypes::ReturnCode::SUCCESS;
}