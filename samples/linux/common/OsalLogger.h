#pragma once

#include <stdio.h>


#include <osal/YggdrasilLoggerHandleBase.h>



/**
 * @brief xxxxxx
 *
 * xxxxxx
 */
class OsalLogger final : public YggdrasilLoggerHandleBase 
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

    
    OsalLogger(std::string_view name, bool supportColor);

    ~OsalLogger() = default;

    YggdrasilTypes::ReturnCode LoggerOutput(LoggerLevel level, std::string_view string) noexcept override;

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