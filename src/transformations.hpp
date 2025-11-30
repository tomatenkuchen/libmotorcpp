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

// templates

/** @brief motor value coordinates aligned with rotor */
template <typename T> struct DQ;
/** @brief motor value coordinates aligned with stator */
template <typename T> struct AB;
/** @brief motor value coordinates aligned with stator in 3 phase system */
template <typename T> struct ABC;

template <typename T> DQ<T> ab_to_dq(AB<T> ab, angular::radian angle);

template <typename T> AB<T> dq_to_ab(DQ<T> dq, angular::radian angle);

template <typename T> ABC<T> ab_to_abc(AB<T> ab);

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
    AB<T> to_ab(angular::radian angle);

    /**
     * @brief convert to stator aligned coordinates in 3 phase system
     * @param angle angle between rotor and stator
     * @return stator aligned values */
    ABC<T> to_abc(angular::radian angle);
};

template <typename T> struct AB
{
    T a;
    T b;

    /**
     * @brief convert to rotor aligned coordinates
     * @param angle angle between rotor and stator
     * @return rotor aligned values */
    DQ<T> to_dq(angular::radian angle);

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
    DQ<T> to_dq(angular::radian angle);

    /**
     * @brief convert to stator aligned coordinates
     * @return stator aligned values */
    AB<T> to_ab();
};

template <typename T> AB<T> DQ<T>::to_ab(angular::radian angle)
{
    return dq_to_ab(*this, angle);
}

template <typename T> ABC<T> DQ<T>::to_abc(angular::radian angle)
{
    return ab_to_abc(dq_to_ab(*this, angle));
}

template <typename T> DQ<T> AB<T>::to_dq(angular::radian angle)
{
    return ab_to_dq(*this, angle);
}

template <typename T> ABC<T> AB<T>::to_abc()
{
    return ab_to_abc(*this);
}

template <typename T> DQ<T> ABC<T>::to_dq(angular::radian angle)
{
    return dq_to_ab(abc_to_ab(*this), angle);
}

template <typename T> AB<T> ABC<T>::to_ab()
{
    return abc_to_ab(*this);
}

} // namespace motor::trans
