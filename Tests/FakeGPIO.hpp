#ifndef FAKE_GPIO_HPP
#define FAKE_GPIO_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "GPIO.hpp"

struct FakeGpio {
    std::vector<std::string> operations;
    std::vector<uint8_t> bus_values;
    std::vector<uint32_t> delays;

    void set_data_bus(uint8_t val) {
        operations.push_back("data_bus");
        bus_values.push_back(val);
    }
    void set_a0(Port port) {
        operations.push_back(port == Port::ADDRESS ? "a0 ADDRESS" : "a0 DATA");
    }
    void set_a1(Bank bank) {
        operations.push_back(bank == Bank::BANK_0 ? "a1 BANK_0" : "a1 BANK_1");
    }
    void set_cs(State s) {
        operations.push_back(s == State::ACTIVE ? "cs ACTIVE" : "cs INACTIVE");
    }
    void set_wr(State s) {
        operations.push_back(s == State::ACTIVE ? "wr ACTIVE" : "wr INACTIVE");
    }
    void set_ic(State s) {
        operations.push_back(s == State::ACTIVE ? "ic ACTIVE" : "ic INACTIVE");
    }
    void set_rd(State s) {
        operations.push_back(s == State::ACTIVE ? "rd ACTIVE" : "rd INACTIVE");
    }
    void delay_ticks(uint32_t n) {
        operations.push_back("delay_ticks");
        delays.push_back(n);
    }
};

#endif
