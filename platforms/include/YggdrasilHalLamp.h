#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace yggdrasil::platform {

/**
 * @brief Abstract interface for controlling indicator lamps within the HAL.
 *
 * Platforms expose their lamp implementations by inheriting from this class and
 * providing the concrete behaviour for turning the lamp on, off, and changing
 * colours. The interface accommodates both discrete colour sets and full RGB
 * colour output.
 */
class YggdrasilHalLamp {
public:
    struct RgbColor {
        std::uint8_t red = 0;
        std::uint8_t green = 0;
        std::uint8_t blue = 0;

        [[nodiscard]] constexpr bool operator==(const RgbColor &other) const noexcept
        {
            return red == other.red && green == other.green && blue == other.blue;
        }

        [[nodiscard]] constexpr bool operator!=(const RgbColor &other) const noexcept
        {
            return !(*this == other);
        }

        static constexpr RgbColor off() noexcept { return RgbColor{}; }
        static constexpr RgbColor white() noexcept { return {255, 255, 255}; }
        static constexpr RgbColor redColor() noexcept { return {255, 0, 0}; }
        static constexpr RgbColor greenColor() noexcept { return {0, 255, 0}; }
        static constexpr RgbColor blueColor() noexcept { return {0, 0, 255}; }
        static constexpr RgbColor amberColor() noexcept { return {255, 191, 0}; }
    };

    enum class LampState {
        Off,
        On,
        Blinking,
    };

    enum class Capability : std::uint32_t {
        None = 0,
        SupportsStateQuery = 1 << 0,
        SupportsBlink = 1 << 1,
        SupportsRgb = 1 << 2,
        SupportsPresetColors = 1 << 3,
    };

    struct Capabilities {
        std::uint32_t flags = 0;
        std::array<RgbColor, 8> presetColors{};
        std::size_t presetCount = 0;

        [[nodiscard]] bool has(Capability capability) const noexcept
        {
            return (flags & static_cast<std::uint32_t>(capability)) != 0;
        }
    };

    virtual ~YggdrasilHalLamp() = default;

    /**
     * @brief Returns a human-friendly identifier for diagnostics/logging.
     */
    [[nodiscard]] virtual std::string_view name() const noexcept = 0;

    /**
     * @brief Retrieve the capability mask for this lamp implementation.
     */
    [[nodiscard]] virtual Capabilities capabilities() const noexcept = 0;

    /**
     * @brief Turn the lamp completely off.
     */
    virtual void turnOff() noexcept = 0;

    /**
     * @brief Turn the lamp on with its default colour or intensity.
     */
    virtual void turnOn() noexcept = 0;

    /**
     * @brief Switch the lamp to the provided RGB colour.
     *
     * Implementations that only support a discrete set of colours should map
     * the request to the closest preset before applying it.
     *
     * @return true if the colour was applied successfully; false otherwise.
     */
    [[nodiscard]] virtual bool setColor(RgbColor color) noexcept = 0;

    /**
     * @brief Trigger/stop a blinking pattern for the lamp.
     *
     * The @p intervalMs parameter indicates the desired blink period in
     * milliseconds. A value of zero should disable blinking and restore a
     * steady state using the last colour.
     *
     * @return true if the blink request was accepted.
     */
    [[nodiscard]] virtual bool blink(std::uint32_t intervalMs) noexcept = 0;

    /**
     * @brief Optional query of the current lamp state.
     *
     * Implementations that do not support state queries should return
     * LampState::Off and false to indicate the state is unknown.
     *
     * @param outState Receives the current state when available.
     * @return true if @p outState contains valid information.
     */
    [[nodiscard]] virtual bool state(LampState &outState) const noexcept = 0;

    /**
     * @brief Optional query of the active colour.
     *
     * Implementations that cannot determine the active colour should leave
     * @p outColor unchanged and return false.
     *
     * @param outColor Receives the current colour when available.
     * @return true if the colour query succeeded.
     */
    [[nodiscard]] virtual bool activeColor(RgbColor &outColor) const noexcept = 0;

    /**
     * @brief Convenience helpers usable by derived classes.
     */
    [[nodiscard]] static constexpr bool supports(const Capabilities &caps, Capability capability) noexcept
    {
        return caps.has(capability);
    }
};

} // namespace yggdrasil::platform
