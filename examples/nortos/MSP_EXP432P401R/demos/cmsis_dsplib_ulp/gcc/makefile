SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

CC = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"
LNK = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"

OBJECTS = dmaTasks.obj main.obj system_msp432p401r.obj gcc_startup_msp432p401r_gcc.obj

NAME = cmsis_dsplib_ulp

CFLAGS = -I.. \
    -DARM_MATH_CM4 \
    -D__FPU_PRESENT=1 \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    -D__MSP432P401R__ \
    -DDeviceFamily_MSP432P401x \
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

LFLAGS = "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/DSP_Lib/lib/gcc/m4f" \
    -l:arm_cortexM4lf_math.a \
    -Wl,-T,../gcc/msp432p401r_LPMRAM.lds \
    "-Wl,-Map,$(NAME).map" \
    "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    -l:ti/display/lib/display.am4fg \
    -l:ti/grlib/lib/gcc/m4f/grlib.a \
    -l:third_party/spiffs/lib/gcc/m4f/spiffs.a \
    -l:ti/drivers/lib/drivers_msp432p401x.am4fg \
    -l:third_party/fatfs/lib/gcc/m4f/fatfs.a \
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

dmaTasks.obj: ../dmaTasks.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

main.obj: ../main.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

system_msp432p401r.obj: ../system_msp432p401r.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

gcc_startup_msp432p401r_gcc.obj: ../gcc/startup_msp432p401r_gcc.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

$(NAME).out: $(OBJECTS)
	@ echo linking $@
	@ $(LNK)  $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
