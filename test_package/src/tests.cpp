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

    auto state = motor.get_motor_state(0.1f * si::second);

    REQUIRE_THAT(state.position.numerical_value_in(angular::radian),
                 Catch::Matchers::WithinRel((motor::sector_size * 0.5).numerical_value_in(angular::radian), 0.01f));
    REQUIRE_THAT(state.speed.numerical_value_in(motor::Motor::radspeed),
                 Catch::Matchers::WithinRel(
                     (motor::sector_size / (0.1f * si::second)).numerical_value_in(motor::Motor::radspeed), 0.01f));
    REQUIRE_THAT(state.acceleration.numerical_value_in(motor::Motor::radacceleration),
                 Catch::Matchers::WithinRel((motor::sector_size / (0.1f * si::second * 0.1f * si::second))
                                                .numerical_value_in(motor::Motor::radacceleration),
                                            0.01f));
}
