# Makefile for the MM32F0144C6P "GPIO_Toggle" firmware (GNU Arm toolchain).
#
# Builds the application together with the vendored MindMotion MM32F0140
# device library into a flashable ELF/BIN/HEX image.
#
#   make            # build build/GPIO_Toggle.{elf,bin,hex} and print size
#   make clean      # remove the build directory
#   make size       # print section sizes of the built ELF

TARGET      := GPIO_Toggle
BUILD_DIR   := build

# ----------------------------------------------------------------------------
# Toolchain
# ----------------------------------------------------------------------------
PREFIX      := arm-none-eabi-
CC          := $(PREFIX)gcc
OBJCOPY     := $(PREFIX)objcopy
SIZE        := $(PREFIX)size

# ----------------------------------------------------------------------------
# Device library layout
# ----------------------------------------------------------------------------
DEVICE_DIR  := Libraries/Device/MM32F0140
CMSIS_DIR   := Libraries/Device/CMSIS
LDSCRIPT    := $(DEVICE_DIR)/Source/GCC/mm32f0140.ld

# ----------------------------------------------------------------------------
# Sources
# ----------------------------------------------------------------------------
# Application sources. SYSTEM/UART/uart.c is intentionally excluded: it is dead
# code (its CONSOLE_Init is unused and references UART_InitTypeDef fields that do
# not exist in the current MM32F0140 library); the active console path is
# HARDWARE/UART/uart_print_loop.c.
APP_SRC := \
    USER/main.c \
    HARDWARE/LED/led.c \
    HARDWARE/EXTI/exti.c \
    HARDWARE/I2C/i2c.c \
    HARDWARE/SPI/spi.c \
    HARDWARE/RAY/ray.c \
    HARDWARE/UART/uart_print_loop.c \
    SYSTEM/DELAY/delay.c \
    SYSTEM/SYS/sys.c

# Device library sources.
LIB_SRC := \
    $(wildcard $(DEVICE_DIR)/HAL_Lib/Src/*.c) \
    $(DEVICE_DIR)/Source/system_mm32f0140.c

ASM_SRC := $(DEVICE_DIR)/Source/GCC/startup_mm32f0140_gcc.S

C_SRC   := $(APP_SRC) $(LIB_SRC)

# ----------------------------------------------------------------------------
# Include paths
# ----------------------------------------------------------------------------
INCLUDES := \
    -I USER \
    -I HARDWARE/EXTI -I HARDWARE/I2C -I HARDWARE/LED -I HARDWARE/RAY \
    -I HARDWARE/SPI -I HARDWARE/UART \
    -I SYSTEM/DELAY -I SYSTEM/SYS -I SYSTEM/UART \
    -I $(DEVICE_DIR)/Include \
    -I $(DEVICE_DIR)/HAL_Lib/Inc \
    -I $(CMSIS_DIR)

# ----------------------------------------------------------------------------
# Flags
# ----------------------------------------------------------------------------
CPU     := -mcpu=cortex-m0 -mthumb -mfloat-abi=soft

CFLAGS  := $(CPU) $(INCLUDES) -DMM32F0140 \
    -Os -g3 -Wall -ffunction-sections -fdata-sections -fms-extensions \
    -fno-common -std=gnu11

ASFLAGS := $(CPU) -g3

LDFLAGS := $(CPU) -T$(LDSCRIPT) \
    --specs=nano.specs --specs=nosys.specs \
    -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref \
    -lc -lm -lnosys

# ----------------------------------------------------------------------------
# Objects
# ----------------------------------------------------------------------------
OBJECTS := $(addprefix $(BUILD_DIR)/,$(C_SRC:.c=.o))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(ASM_SRC:.S=.o))

# ----------------------------------------------------------------------------
# Rules
# ----------------------------------------------------------------------------
.PHONY: all clean size

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex size

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O ihex $< $@

size: $(BUILD_DIR)/$(TARGET).elf
	$(SIZE) $<

clean:
	rm -rf $(BUILD_DIR)
