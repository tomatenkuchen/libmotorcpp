/**
 * @file motor.hpp
 * @author tomatenkuchen
 * @brief motor state definitions
 * @date 2025-10-30
 *
 * @copyright Copyright (c) 2025
 */

#pragma once

#include <cstdint>
#include <mp-units/systems/angular.h>
#include <mp-units/systems/angular/units.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>
#include <mp-units/systems/si/units.h>
#include <numbers>

namespace units = mp_units::angular::unit_symbols;
using namespace mp_units;

namespace bsp
{

constexpr auto sector_size = std::numbers::pi / 3 * units::rad;

struct Motor
{
    enum class Rotation
    {
        cw,
        ccw,
    };

    // define radial speed quantity type
    inline static constexpr auto radspeed = units::rad / si::second;
    // define radial acceleration quantity type
    inline static constexpr auto radacceleration = units::rad / (si::second * si::second);

    struct State
    {
        quantity<units::rad, float> position;
        quantity<radspeed, float> speed;
        quantity<radacceleration, float> acceleration;
    };

    State state;
    quantity<si::second, float> hall_isr_timestamp;
    int8_t last_sector;

    /**
     * @brief interrupt handler to be called when hall sensor generate new input
     * 
     * @param system_time system time of hall sensor interrupt
     * @param new_sector  new sector hall sensor determined rotor to be in
     */
    void hall_interrupt_handler(quantity<si::second, float> system_time, int8_t new_sector);

    /**
     * @brief get the current motor state
     * 
     * @param system_time motor rotation angle is approximated based on last known speed and acceleration. system time is needed for extrapolation
     * @return State current motor state
     */
    State get_motor_state(quantity<si::second, float> system_time);

    private:

    /**
     * @brief Get the rotation direction object 
     * 
     * @param new_sector  new sector hall sensor determined rotor to be in
     * @return Rotation   current rotation direction
     */
    Rotation get_rotation_direction(int8_t new_sector);
};

} // namespace bsp
