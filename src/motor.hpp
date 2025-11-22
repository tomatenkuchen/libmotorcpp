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

    using radspeed = decltype(units::rad / si::second);
    using radacceleration = decltype(units::rad / (si::second * si::second));

    struct State
    {
        quantity<units::rad, float> position;
        // quantity<radspeed, float> speed;
        // quantity<radacceleration, float> acceleration;
    };

    State state;
    quantity<si::second, float> hall_isr_timestamp;
    int8_t last_sector;

    void interrupt_handler(quantity<si::second, float> system_time, int8_t new_sector);

    State get_motor_state(quantity<si::second, float> system_time);

    Rotation get_rotation_direction(int8_t new_sector);
};

} // namespace bsp
