cmake_minimum_required(VERSION 3.20)

project(chipflash LANGUAGES CXX)

option(CHIPFLASH_BUILD_TESTS "Build the ARM/QEMU test executable" ON)
set(CHIPFLASH_QEMU_EXECUTABLE "qemu-arm" CACHE FILEPATH "QEMU user-mode executable used to run ARM tests")
set(CHIPFLASH_QEMU_LD_PREFIX "" CACHE PATH "Optional ARM sysroot passed to qemu-arm via -L")

add_library(ymf262_hal INTERFACE)
target_include_directories(ymf262_hal INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/YMF262-HAL/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Tests
)

if(CHIPFLASH_BUILD_TESTS)
    add_executable(test_write
        Lib/YMF262-HAL/Src/YMF262-HAL.cpp
        Tests/test_write.cpp
    )
    target_link_libraries(test_write PRIVATE ymf262_hal)
    target_compile_features(test_write PRIVATE cxx_std_17)

    include(CTest)
    if(BUILD_TESTING)
        set(_qemu_command ${CHIPFLASH_QEMU_EXECUTABLE})
        if(CHIPFLASH_QEMU_LD_PREFIX)
            list(APPEND _qemu_command -L ${CHIPFLASH_QEMU_LD_PREFIX})
        endif()
        add_test(NAME test_write COMMAND ${_qemu_command} $<TARGET_FILE:test_write>)
    endif()
endif()
