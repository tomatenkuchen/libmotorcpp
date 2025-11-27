#include "motor.hpp"
#include <cstdint>
#include <mp-units/systems/angular.h>
#include <mp-units/systems/angular/units.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>
#include <mp-units/systems/si/units.h>

using namespace mp_units;

namespace motor
{

Motor::Motor(State initial_state, quantity<si::second, float> isr_timestamp, int8_t initial_sector)
    : state(initial_state),
      hall_isr_timestamp(isr_timestamp),
      last_sector(initial_sector)
{
}

void Motor::hall_interrupt_handler(quantity<si::second, float> system_time, int8_t new_sector)
{
    // calc delta t
    auto const delta_t = system_time - hall_isr_timestamp;

    // figure out rotation direction
    auto const dir = get_rotation_direction(new_sector);

    auto const new_speed = (sector_size / delta_t) * (dir == Rotation::ccw ? -1.f : 1.f);

    state.acceleration = (new_speed - state.speed) / delta_t;
    state.speed = new_speed;
    hall_isr_timestamp = system_time;

    if (dir == Rotation::cw)
    {
        switch (new_sector)
        {
        case 1:
            state.position = sector_size * 5.5;
            break;
        case 2:
            state.position = sector_size * 0.5;
            break;
        case 3:
            state.position = sector_size * 1.5;
            break;
        case 4:
            state.position = sector_size * 2.5;
            break;
        case 5:
            state.position = sector_size * 3.5;
            break;
        case 6:
            state.position = sector_size * 4.5;
            break;
        }
    }
    else
    {
        switch (new_sector)
        {
        case 1:
            state.position = sector_size * 0.5;
            break;
        case 2:
            state.position = sector_size * 1.5;
            break;
        case 3:
            state.position = sector_size * 2.5;
            break;
        case 4:
            state.position = sector_size * 3.5;
            break;
        case 5:
            state.position = sector_size * 4.5;
            break;
        case 6:
            state.position = sector_size * 5.5;
            break;
        }
    }

    last_sector = new_sector;
};

Motor::State Motor::get_motor_state(quantity<si::second, float> system_time)
{
    auto const delta_t = system_time - hall_isr_timestamp;

    State current_state = {
        .position = state.position + state.speed * delta_t,
        .speed = state.speed + state.acceleration * delta_t,
        .acceleration = state.acceleration,
    };

    return current_state;
}

Motor::Rotation Motor::get_rotation_direction(int8_t new_sector)
{
    if (new_sector == 1 && last_sector == 6)
    {
        return Rotation::cw;
    }

    if (new_sector == 6 && last_sector == 1)
    {
        return Rotation::ccw;
    }

    return new_sector - last_sector > 0 ? Rotation::cw : Rotation::ccw;
}

} // namespace motor
