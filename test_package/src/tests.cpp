#include "catch2/catch_test_macros.hpp"
#include "motor.hpp"

TEST_CASE("Motor state update on interrupt", "[motor]")
{
    bsp::Motor motor;
    motor.state.position = 0.0f * mp_units::angular::unit_symbols::rad;
    motor.state.speed = 0.0f * (mp_units::angular::unit_symbols::rad / mp_units::si::second);
    motor.state.acceleration = 0.0f * (mp_units::angular::unit_symbols::rad / (mp_units::si::second * mp_units::si::second));
    motor.hall_isr_timestamp = 0.0f * mp_units::si::second;
    motor.last_sector = 1;

    // Simulate an interrupt at t = 0.1s with new sector 2
    motor.interrupt_handler(0.1f * mp_units::si::second, 2);

    auto state = motor.get_motor_state(0.1f * mp_units::si::second);

    REQUIRE(state.position == Approx(bsp::sector_size).epsilon(0.01));
    REQUIRE(state.speed == Approx((bsp::sector_size / (0.1f * mp_units::si::second))).epsilon(0.01));
}
