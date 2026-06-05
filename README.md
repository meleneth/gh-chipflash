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

The test runner uses `qemu-arm` and will try to auto-detect a loader prefix
from common ARM sysroots (for example `/usr/arm-linux-gnueabihf`).

If auto-detection does not match your environment, set `CHIPFLASH_QEMU_LD_PREFIX`
explicitly when configuring:

```bash
cmake -S . -B build-arm \
	-DCMAKE_TOOLCHAIN_FILE=cmake/arm-linux-toolchain.cmake \
	-DCHIPFLASH_QEMU_LD_PREFIX=/path/to/arm-sysroot
```

You can override the QEMU binary with `-DCHIPFLASH_QEMU_EXECUTABLE=/path/to/qemu-arm`.

Then run:

```bash
ctest --test-dir build-arm --output-on-failure
```
