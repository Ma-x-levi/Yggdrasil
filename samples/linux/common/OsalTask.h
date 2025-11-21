#pragma once

#include <stdio.h>


#include <osal/YggdrasilOsalTaskManagerBase.h>



/**
 * @brief xxxxxx
 *
 * xxxxxx
 */
class OsalTaskManager final : public YggdrasilOsalTaskManagerBase 
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

    
    OsalTaskManager();

    ~OsalTaskManager();


    YggdrasilTypes::ReturnCode TaskCreate(std::string_view name, uint32_t stackSize) noexcept override;

    YggdrasilTypes::ReturnCode SleepMs(size_t ms) noexcept override;

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