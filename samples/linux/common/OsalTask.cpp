#include <thread>
#include <chrono>

#include "OsalTask.h"


OsalTaskManager::OsalTaskManager() :
YggdrasilOsalTaskManagerBase()
{
    
}

OsalTaskManager::~OsalTaskManager()
{

}


YggdrasilTypes::ReturnCode OsalTaskManager::TaskCreate(std::string_view name, uint32_t stackSize) noexcept
{
    return YggdrasilTypes::ReturnCode::SUCCESS;
}

YggdrasilTypes::ReturnCode OsalTaskManager::SleepMs(size_t ms) noexcept
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    return YggdrasilTypes::ReturnCode::SUCCESS;
}


