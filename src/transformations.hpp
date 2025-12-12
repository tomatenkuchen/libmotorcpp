/**
 * @file transformations.hpp
 * @brief transformations necessary to get from dq to abc and backwards
 * @author tomatenkuchen
 */

#pragma once

#include <complex>
#include <cstdint>
#include <mp-units/systems/angular.h>
#include <mp-units/systems/angular/units.h>

using namespace mp_units;

namespace motor::trans
{

// prototypes

/** @brief motor value coordinates aligned with rotor */
template <typename T> struct DQ;
/** @brief motor value coordinates aligned with stator */
template <typename T> struct AB;
/** @brief motor value coordinates aligned with stator in 3 phase system */
template <typename T> struct ABC;

/**
 * @brief compute coordinate rotation from stator to rotor alignment
 * @tparam value to compute
 * @param ab motor values in stator orientation
 * @param angle angle between stator and rotor
 * @return rotor aligned values
 */
template <typename T> DQ<T> ab_to_dq(AB<T> ab, quantity<angular::radian, float> angle);

/**
 * @brief compute coordinate rotation from rotor to stator alignment
 * @tparam value to compute
 * @param dq motor values in rotor orientation
 * @param angle angle between stator and rotor
 * @return rotor aligned values
 */
template <typename T> AB<T> dq_to_ab(DQ<T> dq, quantity<angular::radian, float> angle);

/**
 * @brief compute 3 phase system from 2d values
 * @tparam value to compute
 * @param ab 2d values
 * @return 3 phase values
 */
template <typename T> ABC<T> ab_to_abc(AB<T> ab);

/**
 * @brief compute 2d values from 3 phase system
 * @tparam value to compute
 * @param abc 3 phase values
 * @return 2d stator aligned values
 */
template <typename T> AB<T> abc_to_ab(ABC<T> abc);

/** @brief motor values aligned with rotor */
template <typename T> struct DQ
{
    T d;
    T q;

    /**
     * @brief convert to stator aligned coordinates
     * @param angle angle between rotor and stator
     * @return stator aligned values */
    AB<T> to_ab(quantity<angular::radian, float> angle);

    /**
     * @brief convert to stator aligned coordinates in 3 phase system
     * @param angle angle between rotor and stator
     * @return stator aligned values */
    ABC<T> to_abc(quantity<angular::radian, float> angle);
};

template <typename T> struct AB
{
    T a;
    T b;

    /**
     * @brief convert to rotor aligned coordinates
     * @param angle angle between rotor and stator
     * @return rotor aligned values */
    DQ<T> to_dq(quantity<angular::radian, float> angle);

    /**
     * @brief convert to stator aligned coordinates in 3 phase system
     * @return stator aligned values */
    ABC<T> to_abc();
};

template <typename T> struct ABC
{
    T a;
    T b;
    T c;

    /**
     * @brief convert to rotor aligned coordinates
     * @param angle angle between rotor and stator
     * @return rotor aligned values */
    DQ<T> to_dq(quantity<angular::radian, float> angle);

    /**
     * @brief convert to stator aligned coordinates
     * @return stator aligned values */
    AB<T> to_ab();
};

template <typename T> AB<T> DQ<T>::to_ab(quantity<angular::radian, float> angle)
{
    return dq_to_ab(*this, angle);
}

template <typename T> ABC<T> DQ<T>::to_abc(quantity<angular::radian, float> angle)
{
    return ab_to_abc(dq_to_ab(*this, angle));
}

template <typename T> DQ<T> AB<T>::to_dq(quantity<angular::radian, float> angle)
{
    return ab_to_dq(*this, angle);
}

template <typename T> ABC<T> AB<T>::to_abc()
{
    return ab_to_abc(*this);
}

template <typename T> DQ<T> ABC<T>::to_dq(quantity<angular::radian, float> angle)
{
    return dq_to_ab(abc_to_ab(*this), angle);
}

template <typename T> AB<T> ABC<T>::to_ab()
{
    return abc_to_ab(*this);
}

template <typename T> DQ<T> ab_to_dq(AB<T> ab, quantity<angular::radian, float> angle)
{
    float cosine_angle = std::cos(angle.numerical_value_in(angular::radian));
    float sinus_angle = std::sin(angle.numerical_value_in(angular::radian));

    DQ<T> dq = {
        .d = cosine_angle * ab.a + sinus_angle * ab.b,
        .q = -sinus_angle * ab.a + cosine_angle * ab.b,
    };

    return dq;
}

template <typename T> AB<T> dq_to_ab(DQ<T> dq, quantity<angular::radian, float> angle)
{
    // reuse rotation from ab_to_dq in different direction
    AB<T> ab = {
        .a = dq.d,
        .b = dq.q,
    };

    // rotate in inverse direction
    auto const dq2 = ab_to_dq(ab, -angle);
    ab.a = dq2.d;
    ab.b = dq2.q;
    return ab;
}

template <typename T> ABC<T> ab_to_abc(AB<T> ab)
{
    constexpr float sqrt3 = 1.7320508075688772f;

    ABC<T> abc = {
        .a = ab.a,
        .b = ab.a * -0.5f + sqrt3 * ab.b * 0.5f,
        .c = ab.a * -0.5f + sqrt3 * ab.b * -0.5f,
    };

    return abc;
}

template <typename T> AB<T> abc_to_ab(ABC<T> abc)
{
    constexpr float inverse_sqrt3 = 0.5773502691896258f;

    AB<T> ab = {
        .a = abc.a,
        .b = abc.a * inverse_sqrt3 + 2.f * abc.b * inverse_sqrt3,
    };

    return ab;
}

} // namespace motor::trans
