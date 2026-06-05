/**
 * @file GPIO.hpp
 * @brief Logical types for the YMF262 pin-control interface (used by GPIO policies).
 */
#ifndef GPIO_HPP
#define GPIO_HPP

/** @brief Logical activation state of a control line (CS, WR, IC...). */
enum class State{
    ACTIVE,
    INACTIVE
};

/** @brief Selects what the data bus carries (drives the A0 line). */
enum class Port{
    DATA,
    ADDRESS
};

/** @brief Selects the register bank / array (drives A1). */
enum class Bank{
    BANK_0,
    BANK_1,
};

#endif
