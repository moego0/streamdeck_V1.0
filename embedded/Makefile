/**
 * STM32F103 Linker Script and Startup Configuration
 * 
 * This is a template for building the embedded project.
 * Adjust memory regions and optimization flags as needed.
 */

# Compiler
ARM_GCC_TOOLCHAIN = arm-none-eabi

# MCU Specific Flags
MCU_FLAGS = -mcpu=cortex-m3 -mthumb

# Optimization & Debug
CFLAGS += -O2 -g3
CFLAGS += -Wall -Wextra

# Include directories
INCLUDES = -I./embedded
INCLUDES += -I./lvgl
INCLUDES += -I./lvgl/src
INCLUDES += -I./HAL/Inc

# Source files
SOURCES = embedded/main.c
SOURCES += lvgl/src/lv_core/lv_obj.c
SOURCES += lvgl/src/lv_core/lv_disp.c
# ... add more LVGL sources

# Build output
TARGET = streamdeck_firmware
OBJECTS = $(SOURCES:.c=.o)

# Rules
%.o: %.c
	$(ARM_GCC_TOOLCHAIN)-gcc $(MCU_FLAGS) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET).elf: $(OBJECTS)
	$(ARM_GCC_TOOLCHAIN)-gcc $(MCU_FLAGS) -Tlinker.ld -o $@ $^

$(TARGET).bin: $(TARGET).elf
	$(ARM_GCC_TOOLCHAIN)-objcopy -O binary $< $@

clean:
	rm -f $(OBJECTS) $(TARGET).elf $(TARGET).bin

.PHONY: clean
