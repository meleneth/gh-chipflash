# chipflash

This workspace mirrors the YMF262 HAL headers and includes a small ARM/QEMU test
build around them.

## Configure

Use an ARM Linux cross compiler and point CMake at the provided toolchain file:

```bash
cmake -S . -B build-arm -DCMAKE_TOOLCHAIN_FILE=cmake/arm-linux-toolchain.cmake
```

## Build

```bash
cmake --build build-arm
```

## Run the test under QEMU

If your ARM binary needs a sysroot, pass it through `CHIPFLASH_QEMU_LD_PREFIX`:

```bash
ctest --test-dir build-arm --output-on-failure
```

You can override the QEMU binary with `-DCHIPFLASH_QEMU_EXECUTABLE=/path/to/qemu-arm`.
