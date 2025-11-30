#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include "motor.hpp"
#include <mp-units/systems/angular/units.h>
#include <mp-units/systems/si.h>

using namespace mp_units;

TEST_CASE("Motor state update on interrupt", "[motor]")
{
    motor::Motor motor;

    // Simulate an interrupt at t = 0.1s with new sector 2
    motor.hall_interrupt_handler(0.1f * si::second, 2);

    // get state on same time. no change in time is expected
    auto state = motor.get_motor_state(0.1f * si::second);

    /// check result
    // acceleration is position independend
    REQUIRE_THAT(state.acceleration.numerical_value_in(motor::Motor::radacceleration),
                 Catch::Matchers::WithinRel((motor::sector_size / (0.1f * si::second * 0.1f * si::second))
                                                .numerical_value_in(motor::Motor::radacceleration),
                                            0.01f));

    // speed is calculated by sector size (60°) by the time it took to pass the sector
    REQUIRE_THAT(state.speed.numerical_value_in(motor::Motor::radspeed),
                 Catch::Matchers::WithinRel(
                     (motor::sector_size / (0.1f * si::second)).numerical_value_in(motor::Motor::radspeed), 0.01f));

    // position is dependent on rotation direction
    REQUIRE_THAT(state.position.numerical_value_in(angular::radian),
                 Catch::Matchers::WithinRel((motor::sector_size * 0.5).numerical_value_in(angular::radian), 0.01f));

    // test extrapolation for a second
    auto state_after_1_sec = motor.get_motor_state(1.1f * si::second);

    /// check result
    // acceleration should not have changed
    REQUIRE_THAT(state_after_1_sec.acceleration.numerical_value_in(motor::Motor::radacceleration),
                 Catch::Matchers::WithinRel((motor::sector_size / (0.1f * si::second * 0.1f * si::second))
                                                .numerical_value_in(motor::Motor::radacceleration),
                                            0.01f));

    // speed is integrated over a second of acceleration
    auto const expected_speed = state.acceleration * 1.f * si::second;
    REQUIRE_THAT(state_after_1_sec.speed.numerical_value_in(motor::Motor::radspeed),
                 Catch::Matchers::WithinRel(expected_speed.numerical_value_in(motor::Motor::radspeed), 0.01f));

    // position is integrated over speed and acceleration
    auto const expected_position = expected_speed * 1.f * si::second;
    REQUIRE_THAT(state_after_1_sec.position.numerical_value_in(angular::radian),
                 Catch::Matchers::WithinRel(expected_position.numerical_value_in(angular::radian), 0.01f));
}
