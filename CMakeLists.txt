cmake_minimum_required(VERSION 3.20)

project(chipflash LANGUAGES CXX)

option(CHIPFLASH_BUILD_TESTS "Build the ARM/QEMU test executable" ON)
set(CHIPFLASH_QEMU_EXECUTABLE "qemu-arm" CACHE FILEPATH "QEMU user-mode executable used to run ARM tests")
set(CHIPFLASH_QEMU_LD_PREFIX "" CACHE PATH "Optional ARM sysroot passed to qemu-arm via -L")

set(_chipflash_qemu_ld_prefix "${CHIPFLASH_QEMU_LD_PREFIX}")
if(NOT _chipflash_qemu_ld_prefix)
    set(_chipflash_qemu_ld_prefix_candidates)
    if(CMAKE_SYSROOT)
        list(APPEND _chipflash_qemu_ld_prefix_candidates "${CMAKE_SYSROOT}")
    endif()
    if(CMAKE_CXX_COMPILER_TARGET)
        list(APPEND _chipflash_qemu_ld_prefix_candidates "/usr/${CMAKE_CXX_COMPILER_TARGET}")
    endif()
    list(APPEND _chipflash_qemu_ld_prefix_candidates
        "/usr/arm-linux-gnueabihf"
        "/usr/arm-linux-gnueabi"
    )

    foreach(_candidate IN LISTS _chipflash_qemu_ld_prefix_candidates)
        if(EXISTS "${_candidate}/lib/ld-linux-armhf.so.3" OR EXISTS "${_candidate}/lib/ld-linux.so.3")
            set(_chipflash_qemu_ld_prefix "${_candidate}")
            break()
        endif()
    endforeach()
endif()

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
        if(_chipflash_qemu_ld_prefix)
            list(APPEND _qemu_command -L ${_chipflash_qemu_ld_prefix})
        endif()
        add_test(NAME test_write COMMAND ${_qemu_command} $<TARGET_FILE:test_write>)
    endif()
endif()
