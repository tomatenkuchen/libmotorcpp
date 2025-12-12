#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include "motor.hpp"
#include "transformations.hpp"
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
    auto const expected_speed = state.speed + state.acceleration * 1.f * si::second;
    REQUIRE_THAT(state_after_1_sec.speed.numerical_value_in(motor::Motor::radspeed),
                 Catch::Matchers::WithinRel(expected_speed.numerical_value_in(motor::Motor::radspeed), 0.01f));

    // position is integrated over speed and acceleration
    auto const expected_position = state.position + expected_speed * 1.f * si::second;
    REQUIRE_THAT(state_after_1_sec.position.numerical_value_in(angular::radian),
                 Catch::Matchers::WithinRel(expected_position.numerical_value_in(angular::radian), 0.01f));
}

TEST_CASE("transformation tests", "[trafo]")
{
    // to test the transformations, a dq to abc and backwards analysis should suffice, since all steps on the way are
    // included
    motor::trans::DQ<quantity<si::volt, float>> dq = {
        .d = 1.f * si::volt,
        .q = 0.f * si::volt,
    };

    // the 3 phase equivalent of the dq input are 3 sinus signals with 120° phase shift
    quantity<angular::radian, float> const full_circle = 2.f * std::numbers::pi * angular::radian;
    quantity<angular::radian, float> const step = 0.1f * std::numbers::pi * angular::radian;
    quantity<angular::radian, float> const _120deg = full_circle / 3;
    for (quantity<angular::radian, float> angle = 0.f * angular::radian; angle < full_circle; angle += step)
    {
        // transform
        auto const abc = dq.to_abc(angle);

        // calculate reference voltages
        auto const v_phase_u = std::cos(angle.numerical_value_in(angular::radian));
        auto const v_phase_v = std::cos((angle + _120deg).numerical_value_in(angular::radian));
        auto const v_phase_w = std::cos((angle + _120deg * 2).numerical_value_in(angular::radian));

        // check
        REQUIRE_THAT(abc.a.numerical_value_in(si::volt), Catch::Matchers::WithinRel(v_phase_u, 0.01f));
        REQUIRE_THAT(abc.b.numerical_value_in(si::volt), Catch::Matchers::WithinRel(v_phase_v, 0.01f));
        REQUIRE_THAT(abc.c.numerical_value_in(si::volt), Catch::Matchers::WithinRel(v_phase_w, 0.01f));
    }
}
