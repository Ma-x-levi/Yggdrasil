#pragma once

#include <common/YggdrasilTypes.h>

class YggdrasilOsalTaskManagerBase
{

//define space
public:

//define space
protected:

//define space
private:


//variable space
public:

//variable space
protected:

//variable space
private:


//method space
public:

    YggdrasilOsalTaskManagerBase(){};

    ~YggdrasilOsalTaskManagerBase(){};

    [[nodiscard]] virtual YggdrasilTypes::ReturnCode TaskCreate(std::string_view name, uint32_t stackSize) noexcept = 0;

    [[nodiscard]] virtual YggdrasilTypes::ReturnCode SleepMs(size_t ms) noexcept = 0;

//method space
protected:

//method space
private:


//interface space
public:

//interface space
protected:

//interface space
private:

};


class DefaultYggdrasilOsalTaskManager : public YggdrasilOsalTaskManagerBase
{
public:
    DefaultYggdrasilOsalTaskManager(){};

    ~DefaultYggdrasilOsalTaskManager(){};

    [[nodiscard]] virtual YggdrasilTypes::ReturnCode TaskCreate(std::string_view name, uint32_t stackSize) noexcept override { return YggdrasilTypes::ReturnCode::NONSUPPORT; }

    [[nodiscard]] virtual YggdrasilTypes::ReturnCode SleepMs(size_t ms) noexcept override { return YggdrasilTypes::ReturnCode::NONSUPPORT; }

};
