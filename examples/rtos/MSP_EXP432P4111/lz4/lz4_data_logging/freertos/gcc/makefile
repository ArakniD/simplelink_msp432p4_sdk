SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../../../../..)

include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

KERNEL_BUILD := $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/freertos/builds/MSP_EXP432P4111/release

CC = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"
LNK = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"

OBJECTS = compress_thread.obj sensor_thread.obj MSP_EXP432P4111.obj freertos_main_freertos.obj

NAME = lz4_data_logging

CFLAGS = -I../.. \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" \
    -D__MSP432P4111__ \
    -DDeviceFamily_MSP432P4x1xI \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/posix/gcc" \
    "-I$(FREERTOS_INSTALL_DIR)/FreeRTOS/Source/include" \
    "-I$(FREERTOS_INSTALL_DIR)/FreeRTOS/Source/portable/GCC/ARM_CM4F" \
    "-I$(KERNEL_BUILD)" \
    -mcpu=cortex-m4 \
    -march=armv7e-m \
    -mthumb \
    -std=c99 \
    -mfloat-abi=hard \
    -mfpu=fpv4-sp-d16 \
    -ffunction-sections \
    -fdata-sections \
    -g \
    -gstrict-dwarf \
    -Wall \
    "-I$(GCC_ARMCOMPILER)/arm-none-eabi/include/newlib-nano" \
    "-I$(GCC_ARMCOMPILER)/arm-none-eabi/include"

LFLAGS = "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/compression/lz4/lib/gcc/m4f" \
    -l:lz4.a \
    -Wl,-T,../../freertos/gcc/MSP_EXP432P4111_FREERTOS.lds \
    "-Wl,-Map,$(NAME).map" \
    "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    -l:ti/display/lib/display.am4fg \
    -l:ti/grlib/lib/gcc/m4f/grlib.a \
    -l:third_party/spiffs/lib/gcc/m4f/spiffs.a \
    -l:ti/drivers/lib/drivers_msp432p4x1xi.am4fg \
    -l:third_party/fatfs/lib/gcc/m4f/fatfs.a \
    -nostartfiles \
    "-L$(KERNEL_BUILD)/gcc" \
    -l:freertos.lib \
    -l:ti/devices/msp432p4xx/driverlib/gcc/msp432p4xx_driverlib.a \
    -march=armv7e-m \
    -mthumb \
    -mfloat-abi=hard \
    -mfpu=fpv4-sp-d16 \
    -static \
    -Wl,--gc-sections \
    "-L$(GCC_ARMCOMPILER)/arm-none-eabi/lib/thumb/v7e-m/fpv4-sp/hard" \
    -lgcc \
    -lc \
    -lm \
    -lnosys \
    --specs=nano.specs

all: $(NAME).out

$(KERNEL_BUILD)/gcc/freertos.lib:
	@ $(ECHOBLANKLINE)
	@ echo $@ is not built.
	@ echo You can build it by issuing $(MAKE) in $(dir $@).
	@ $(ECHOBLANKLINE)

compress_thread.obj: ../../compress_thread.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

sensor_thread.obj: ../../sensor_thread.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

MSP_EXP432P4111.obj: ../../MSP_EXP432P4111.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

freertos_main_freertos.obj: ../../freertos/main_freertos.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

$(NAME).out: $(OBJECTS) $(KERNEL_BUILD)/gcc/freertos.lib
	@ echo linking $@
	@ $(LNK)  $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
