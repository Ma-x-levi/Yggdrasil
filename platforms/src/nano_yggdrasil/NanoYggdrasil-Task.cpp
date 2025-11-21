#include "NanoYggdrasil.h"

YggdrasilTypes::ReturnCode NanoYggdrasil::RegisterOsalTaskManager(std::reference_wrapper<YggdrasilOsalTaskManagerBase> manager) noexcept
{
    _osalTaskManager = manager;
    return YggdrasilTypes::ReturnCode::SUCCESS;
}


