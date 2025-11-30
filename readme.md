# libmotorcpp

c++ motor control library tailored for embedded devices with a floating point engine.
function comprises of
- 3 phase transformations
- hall sensor input dependent motor state analysis
- future: electrical value dependent motor state analyzer


## setup

this project is best handled and compiled using conan package manager. this is how

```bash
# initial setup if you haven't got conan already
pip install conan
conan profile detect

# build library for your machine
conan build . -b=missing

# build for embedded machine
conan build . -b=missing -pr=my-microcontroler-profile
```

## units

to help with robust implementations, safe types and a healily paraniod compiler I use mp-units as a
units library for all physical units. It's my first attempt to use them. If you find a better way
to build this, please notify me.

most notably, you will see functions with arguments that hold
```c++
quantity<si::ampere, float> some_electrical_current
```

instead of a pure float. this helps avoid errors with values.

## usage

motor control algorithms usually work in dq transformed space to ease yourself from producing
sinuses in your controller code all the time. and for various simplifications on the way.

to get to and from 3 phase motor values, transformations make this relatively easy. here's an exaple

```c++
#include "transformations.hpp"

void my_µC_adc_interrupt()
{
	// get measurements from your adc
	quantity<si::ampere, float> i_u = get_phase_u_current();
	quantity<si::ampere, float> i_v = get_phase_v_current();
	quantity<si::ampere, float> i_w = get_phase_w_current();

	motor::trans::ABC<quantity<si::ampere, float>> const abc = {
		.a = i_u,
		.b = i_v,
		.c = i_w,
	};

	// calc dq currents from member function
	auto const dq_by_member dq = abc.to_dq(motor_state.position);  

	// calc dq currents via standalone function
	auto const dq_by_func dqfunc = motor::trans::abc_to_dq(abc);
}
```
