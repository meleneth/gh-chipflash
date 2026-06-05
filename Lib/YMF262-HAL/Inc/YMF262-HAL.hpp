/**
 * @file YMF262-HAL.hpp
 * @brief Hardware Abstraction Layer for the Yamaha YMF262 (OPL3).
 */

#ifndef YMF262_HAL_HPP
#define YMF262_HAL_HPP

#include <cstdint>
#include "GPIO.hpp"
#include "YMF262-Types.hpp"

/**
 * @brief Hardware Abstraction Layer for the Yamaha YMF262 (OPL3) integrated circuit.
 *
 * Choreographs the control protocol between the synthesizer IC and the
 * microcontroller. The electrical handling of the pins is delegated to the
 * GPIOPolicy (template parameter), allowing the logic to be tested with a
 * mock without real hardware.
 *
 * @tparam GPIOPolicy Class type implementing the verbs for electrical pin control
 *                    (set_a0, set_a1, set_cs, set_wr, set_ic, set_data_bus, delay_ticks).
 */
template <class GPIOPolicy>
class YMF262_HAL{
     private:
    GPIOPolicy& _gpio;
    uint32_t _t_icw_ticks;
    uint32_t _t_as_ticks;
    uint32_t _t_ah_ticks;
    uint32_t _t_csw_ticks;
    uint32_t _t_csr_ticks;
    uint32_t _t_ww_ticks;
    uint32_t _t_wds_ticks;
    uint32_t _t_wdh_ticks;
    uint32_t _t_rw_ticks;
    uint32_t _t_acc_ticks;
    uint32_t _t_rdh_ticks;
    uint32_t _t_recovery_ticks;

    /**
     * @brief Performs a single bus write cycle (one of the two in a register write).
     */
    void write_bus(Bank bank, Port port, uint8_t data){
        _gpio.set_a0(port);
        _gpio.set_a1(bank);
        _gpio.delay_ticks(_t_as_ticks);
        _gpio.set_cs(State::ACTIVE);
        _gpio.set_wr(State::ACTIVE);
        _gpio.delay_ticks(_t_ww_ticks - _t_wds_ticks);
        _gpio.set_data_bus(data);
        _gpio.delay_ticks(_t_wds_ticks);
        _gpio.set_wr(State::INACTIVE);
        _gpio.set_cs(State::INACTIVE);
        _gpio.delay_ticks(_t_ah_ticks);
        _gpio.delay_ticks(_t_wdh_ticks - _t_ah_ticks);
        _gpio.delay_ticks(_t_recovery_ticks);
    };

    public:
    YMF262_HAL(GPIOPolicy& policy, uint32_t opl_clock, uint32_t cpu_clock) : _gpio(policy){
        _t_icw_ticks = 400 * (cpu_clock / opl_clock);
        _t_recovery_ticks = 32 * (cpu_clock / opl_clock);
        double ticks_per_ns = static_cast<double>(cpu_clock) / 1.0e9;
        _t_as_ticks   = static_cast<uint32_t>(10  * ticks_per_ns);
        _t_ah_ticks   = static_cast<uint32_t>(10  * ticks_per_ns);
        _t_csw_ticks  = static_cast<uint32_t>(100 * ticks_per_ns);
        _t_ww_ticks   = static_cast<uint32_t>(100 * ticks_per_ns);
        _t_wds_ticks  = static_cast<uint32_t>(10  * ticks_per_ns);
        _t_wdh_ticks  = static_cast<uint32_t>(20  * ticks_per_ns);
        _t_csr_ticks  = static_cast<uint32_t>(150 * ticks_per_ns);
        _t_rw_ticks   = static_cast<uint32_t>(150 * ticks_per_ns);
        _t_acc_ticks  = static_cast<uint32_t>(150 * ticks_per_ns);
        _t_rdh_ticks  = static_cast<uint32_t>(10  * ticks_per_ns);
    };

    void initialize(OPLMode mode){
        initial_clear();
        set_OPL_Mode(mode);
    }

    void write(Bank bank, uint8_t reg, uint8_t data){
        write_bus(bank, Port::ADDRESS, reg);
        write_bus(bank, Port::DATA, data);
    };

    void set_OPL_Mode(OPLMode mode){
        write(Bank::BANK_1, 0x105, (mode == OPLMode::OPL3) ? 0x01 : 0x00);
    };

    void initial_clear(){
        _gpio.set_ic(State::ACTIVE);
        _gpio.delay_ticks(_t_icw_ticks);
        _gpio.set_ic(State::INACTIVE);
        _gpio.delay_ticks(_t_recovery_ticks);
    };
};

#endif
