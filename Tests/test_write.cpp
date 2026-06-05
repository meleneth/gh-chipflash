#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "YMF262-HAL.hpp"
#include "FakeGPIO.hpp"

namespace {

bool expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << message << '\n';
    }
    return condition;
}

template <typename T>
bool expect_eq(const T& actual, const T& expected, const char* message) {
    if (!(actual == expected)) {
        std::cerr << message << " expected=" << expected << " actual=" << actual << '\n';
        return false;
    }
    return true;
}

bool test_write_sequence() {
    FakeGpio fake;
    YMF262_HAL<FakeGpio> hal{fake, 14318000, 216000000};

    hal.write(Bank::BANK_0, 0x40, 0x20);

    const std::vector<std::string> expected_ops = {
        "a0 ADDRESS", "a1 BANK_0", "delay_ticks", "cs ACTIVE", "wr ACTIVE",
        "delay_ticks", "data_bus", "delay_ticks", "wr INACTIVE", "cs INACTIVE",
        "delay_ticks", "delay_ticks", "delay_ticks",
        "a0 DATA", "a1 BANK_0", "delay_ticks", "cs ACTIVE", "wr ACTIVE",
        "delay_ticks", "data_bus", "delay_ticks", "wr INACTIVE", "cs INACTIVE",
        "delay_ticks", "delay_ticks", "delay_ticks"
    };

    if (!expect_eq(fake.operations.size(), expected_ops.size(), "unexpected operation count")) {
        return false;
    }

    for (std::size_t i = 0; i < expected_ops.size(); ++i) {
        if (!expect_eq(fake.operations[i], expected_ops[i], "unexpected operation order")) {
            return false;
        }
    }

    if (!expect_eq(fake.bus_values.size(), static_cast<size_t>(2), "unexpected bus write count")) {
        return false;
    }
    if (!expect_eq(fake.bus_values[0], static_cast<uint8_t>(0x40), "unexpected register value")) {
        return false;
    }
    if (!expect_eq(fake.bus_values[1], static_cast<uint8_t>(0x20), "unexpected data value")) {
        return false;
    }
    if (!expect_eq(fake.delays.size(), static_cast<size_t>(12), "unexpected delay count")) {
        return false;
    }

    return true;
}

bool test_initial_clear_then_mode() {
    FakeGpio fake;
    YMF262_HAL<FakeGpio> hal{fake, 14318000, 216000000};

    hal.initialize(OPLMode::OPL3);

    if (!expect(fake.operations.size() > 0, "initialize produced no operations")) {
        return false;
    }
    if (!expect_eq(fake.operations[0], std::string("ic ACTIVE"), "reset did not start with IC active")) {
        return false;
    }
    if (!expect_eq(fake.operations[1], std::string("delay_ticks"), "reset did not wait after IC assert")) {
        return false;
    }
    if (!expect_eq(fake.operations[2], std::string("ic INACTIVE"), "reset did not release IC")) {
        return false;
    }
    if (!expect_eq(fake.operations[3], std::string("delay_ticks"), "reset did not wait after IC release")) {
        return false;
    }

    if (!expect_eq(fake.bus_values.size(), static_cast<size_t>(2), "OPL mode write should produce two bus writes")) {
        return false;
    }
    if (!expect_eq(fake.bus_values[0], static_cast<uint8_t>(0x05), "mode write should target register 0x105 low byte")) {
        return false;
    }
    if (!expect_eq(fake.bus_values[1], static_cast<uint8_t>(0x01), "OPL3 mode should enable the chip")) {
        return false;
    }

    return true;
}

} // namespace

int main() {
    if (!test_write_sequence()) {
        return EXIT_FAILURE;
    }
    if (!test_initial_clear_then_mode()) {
        return EXIT_FAILURE;
    }

    std::cout << "all tests passed\n";
    return EXIT_SUCCESS;
}
