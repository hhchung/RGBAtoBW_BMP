# RGBAtoBW_BMP
This prject is based on brchiu/RGBAtoBW and with some modifiation.
I optimize the kernel algorithm with NEON asm on aarch64

# Build & Run
- C version:  `make run`
- NEON(aarch64) version: `meke ARCH=aarch64 run`
- QEMU (aarch64): `make ARCH=qemu_aarch64 run`


