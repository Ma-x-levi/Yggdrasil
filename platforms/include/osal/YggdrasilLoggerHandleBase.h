#pragma once

#include <common/YggdrasilTypes.h>

/**
 * @brief xxxxxx
 *
 * xxxxxx
 */
class YggdrasilLoggerHandleBase
{

//define space
public:

    enum class LoggerLevel : uint8_t
    {
        DEFAULT  = 0,
        ERROR    = 1,
        WARN     = 2,
        INFO     = 3,
        DEBUG    = 4,
    };

    using HandlePtr = std::unique_ptr<YggdrasilLoggerHandleBase>;
    

//define space
protected:  

//define space
private:

    const YggdrasilTypes::HandleName     _handleName;
    const bool                           _supportColor;

//variable space
public:

//variable space
protected:  

//variable space
private:


//method space
public:

    
    YggdrasilLoggerHandleBase(std::string_view name, bool supportColor) :
    _handleName{0},
    _supportColor(supportColor)
    {
        YggdrasilTypes::HandleName* namePtr = const_cast<YggdrasilTypes::HandleName*>(&_handleName);
        std::fill(namePtr->begin(), namePtr->end(), 0);
        std::copy(name.begin(), name.begin() + std::min(name.size(), namePtr->size()), namePtr->begin());
    }

    virtual ~YggdrasilLoggerHandleBase() = default;

    virtual YggdrasilTypes::ReturnCode LoggerOutput(LoggerLevel level, std::string_view string) noexcept = 0;



//method space
protected:

//method space
private:


//interface space
public:

    std::string_view getName() const noexcept
    {
        return std::string_view(_handleName.data(), _handleName.size());
    }

//interface space
protected:

//interface space
private:

};


