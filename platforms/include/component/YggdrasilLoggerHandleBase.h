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
    using HandleName = std::array<char, YGGDRASIL_DEFAULT_NAME_SIZE>;

//define space
protected:  

//define space
private:

    const HandleName     handleName;
    const bool           isSupportColor;

//variable space
public:

//variable space
protected:  

//variable space
private:


//method space
public:

    
    YggdrasilLoggerHandleBase(std::string_view name, bool supportColor) :
    handleName{0},
    isSupportColor(supportColor)
    {
        HandleName* namePtr = const_cast<HandleName*>(&handleName);
        std::fill(namePtr->begin(), namePtr->end(), 0);
        std::copy(name.begin(), name.begin() + std::min(name.size(), namePtr->size()), namePtr->begin());
    }

    virtual ~YggdrasilLoggerHandleBase() = default;

    virtual YggdrasilTypes::ReturnCode LoggerOutput(LoggerLevel level, std::string_view string) = 0;



//method space
protected:

//method space
private:


//interface space
public:

    std::string_view getName() const
    {
        return std::string_view(handleName.data(), handleName.size());
    }

//interface space
protected:

//interface space
private:

};


