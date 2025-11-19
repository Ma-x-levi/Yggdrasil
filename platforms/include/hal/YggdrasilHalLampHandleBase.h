#pragma once

#include <common/YggdrasilTypes.h>

/**
 * @brief xxxxxx
 *
 * xxxxxx
 */
class YggdrasilHalLampHandleBase
{

//define space
public:

    enum class LampFuntion : uint8_t
    {
        UNKNOWN             = 0,
        SYSTEM_STATUS       = 1,
        USER                = 2,
    };

    struct RgbColor
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

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
    const YggdrasilTypes::HandleName    _handleName;
    const LampFuntion                   _lampFunction;
    RgbColor                            _presentColor;


//method space
public:

    YggdrasilHalLampHandleBase(std::string_view name, LampFuntion function) :
    _handleName{0},
    _lampFunction(function)
    {
        YggdrasilTypes::HandleName* namePtr = const_cast<YggdrasilTypes::HandleName*>(&_handleName);
        std::fill(namePtr->begin(), namePtr->end(), 0);
        std::copy(name.begin(), name.begin() + std::min(name.size(), namePtr->size()), namePtr->begin());
    }

    virtual ~YggdrasilHalLampHandleBase() = default;

    /**
     * @brief Turn the lamp completely off.
     */
    [[nodiscard]] virtual YggdrasilTypes::ReturnCode TurnOff() noexcept = 0;

    /**
     * @brief Turn the lamp on with its default colour or intensity.
     */
    [[nodiscard]] virtual YggdrasilTypes::ReturnCode TurnOn() noexcept = 0;

    /**
     * @brief Toggle the lamp .
     */
    [[nodiscard]] virtual YggdrasilTypes::ReturnCode Toggle() noexcept = 0;

    /**
     * @brief Switch the lamp to the provided RGB colour.
     *
     * Implementations that only support a discrete set of colours should map
     * the request to the closest preset before applying it.
     *
     * @return true if the colour was applied successfully; false otherwise.
     */
    [[nodiscard]] virtual YggdrasilTypes::ReturnCode SetColor(RgbColor color) noexcept = 0;

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


