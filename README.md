# workspace

for regulus

## GPIO_Toggle firmware (MM32F0144C6P)

This repository contains MindMotion **MM32F0144C6P** (Arm Cortex-M0) firmware that
blinks LED1–LED4 and exercises the UART, I2C, SPI and EXTI peripherals. See
[`readme.md`](readme.md) for the application description.

### Toolchain

Builds use the GNU Arm Embedded toolchain (`arm-none-eabi-gcc`). On Debian/Ubuntu:

```bash
sudo apt-get install -y gcc-arm-none-eabi binutils-arm-none-eabi libnewlib-arm-none-eabi make
```

Cloud Agents install this automatically via [`.cursor/install.sh`](.cursor/install.sh).

### Build

```bash
make          # -> build/GPIO_Toggle.elf, .bin, .hex (+ size report)
make clean    # remove build artifacts
```

The MindMotion MM32F0140 device library (CMSIS core, peripheral HAL, `system_*`)
is vendored under [`Libraries/`](Libraries/). A GCC startup file and linker script
for the 64 KB Flash / 8 KB SRAM part live in
`Libraries/Device/MM32F0140/Source/GCC/`.

### Flashing

Flash `build/GPIO_Toggle.bin` to `0x08000000` (or `build/GPIO_Toggle.hex`) with an
MM32-LINK / CMSIS-DAP probe, for example:

```bash
# OpenOCD example (adjust interface/target as needed)
openocd -f interface/cmsis-dap.cfg -c "transport select swd" \
        -f target/... -c "program build/GPIO_Toggle.hex verify reset exit"
```
