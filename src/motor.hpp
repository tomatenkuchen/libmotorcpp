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

namespace bsp {

inline constexpr struct radspeed final
    : named_unit<"", mag<1> * angular::unit_symbols::rad / si::second> {};

inline constexpr struct radacceleration final
    : named_unit<"", mag<1> * radspeed / si::second> {};

constexpr auto sector_size = std::numbers::pi / 3 * units::rad;

struct Motor {
  enum class Rotation {
    cw,
    ccw,
  };

  struct State {
    quantity<angular::unit_symbols::rad, float> position;
    radspeed speed;
    radacceleration acceleration;
  };

  State state;
  quantity<si::second, float> hall_isr_timestamp;
  int8_t last_sector;

  void interrupt_handler(quantity<si::second, float> system_time,
                         int8_t new_sector);

  State get_motor_state(quantity<si::second, float> system_time);

  Rotation get_rotation_direction(int8_t new_sector);
};

} // namespace bsp
