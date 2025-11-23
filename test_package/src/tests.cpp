#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include "motor.hpp"

using namespace mp_units;

TEST_CASE("Motor state update on interrupt", "[motor]")
{
    motor::Motor motor = {
        .state =
            {
                .position = 0.0f * mp_units::angular::unit_symbols::rad,
                .speed = 0.0f * motor::Motor::radspeed,
                .acceleration = 0.0f * motor::Motor::radacceleration,
            },
        .hall_isr_timestamp = 0.0f * mp_units::si::second,
        .last_sector = 1,
    };

    // Simulate an interrupt at t = 0.1s with new sector 2
    motor.hall_interrupt_handler(0.1f * mp_units::si::second, 2);

    auto state = motor.get_motor_state(0.1f * mp_units::si::second);

    REQUIRE_THAT(state.position, Catch::Matchers::WithinRel(motor::sector_size, 0.01));
    REQUIRE_THAT(state.speed, Catch::Matchers::WithinRel((motor::sector_size / (0.1f * si::second)), 0.01));
    REQUIRE_THAT(state.acceleration,
                 Catch::Matchers::WithinRel((motor::sector_size / (0.1f * si::second * 0.1f * si::second)), 0.01));
}
