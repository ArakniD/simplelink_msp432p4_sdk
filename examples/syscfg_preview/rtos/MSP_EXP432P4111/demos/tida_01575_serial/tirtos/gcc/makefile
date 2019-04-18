SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../../../../../..)

include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

KERNEL_BUILD := $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/tirtos/builds/MSP_EXP432P4111/release
SYSCONFIG_PRODUCT := $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/.metadata/product.json

CC = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"
LNK = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"

OBJECTS = Board.obj main_tirtos.obj commonComms.obj vibrationCapture.obj

CONFIGPKG = $(KERNEL_BUILD)/gcc

NAME = tida_01575_serial

CFLAGS = -I../.. \
    -I. \
    -DARM_MATH_CM4 \
    -D__FPU_PRESENT=1 \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" \
    -DRTC_WAKEUP \
    -DAVG_LOOP_COUNT=1 \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    -D__MSP432P4111__ \
    -DDeviceFamily_MSP432P4x1xI \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/posix/gcc" \
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
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/tirtos/packages/gnu/targets/arm/libs/install-native/arm-none-eabi/include/newlib-nano" \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/tirtos/packages/gnu/targets/arm/libs/install-native/arm-none-eabi/include" \
    "-I$(GCC_ARMCOMPILER)/arm-none-eabi/include"

LFLAGS = "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/DSP_Lib/lib/gcc/m4f" \
    -l:arm_cortexM4lf_math.a \
    -Wl,-T,../../tirtos/gcc/MSP_EXP432P4111_TIRTOS.lds \
    "-Wl,-Map,$(NAME).map" \
    "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    -l:ti/display/lib/display.am4fg \
    -l:ti/grlib/lib/gcc/m4f/grlib.a \
    -l:third_party/spiffs/lib/gcc/m4f/spiffs.a \
    -l:ti/drivers/lib/drivers_msp432p4x1xi.am4fg \
    -l:third_party/fatfs/lib/gcc/m4f/fatfs.a \
    -nostartfiles \
    "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/tirtos/packages" \
    -l:ti/dpl/lib/dpl_msp432p4x1xi.am4fg \
    "-Wl,-T,$(KERNEL_BUILD)/gcc/linker.cmd" \
    -l:ti/devices/msp432p4xx/driverlib/gcc/msp432p4xx_driverlib.a \
    -march=armv7e-m \
    -mthumb \
    -mfloat-abi=hard \
    -mfpu=fpv4-sp-d16 \
    -static \
    -Wl,--gc-sections \
    "-L$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/tirtos/packages/gnu/targets/arm/libs/install-native/arm-none-eabi/lib/thumb/v7e-m/fpv4-sp/hard" \
    "-L$(GCC_ARMCOMPILER)/arm-none-eabi/lib" \
    -lgcc \
    -lc \
    -lm \
    -lnosys \
    --specs=nano.specs

all: $(NAME).out

$(CONFIGPKG)/linker.cmd $(CONFIGPKG)/compiler.opt:
	@ $(ECHOBLANKLINE)
	@ echo $(CONFIGPKG) is not built.
	@ echo You can build it by issuing $(MAKE) in $(CONFIGPKG).
	@ $(ECHOBLANKLINE)

.INTERMEDIATE: syscfg
Board.c Board.h : syscfg
	@ echo generation complete

syscfg: ../../tida_01575_serial.syscfg
	@ echo Generating configuration files...
	@ $(NODE_JS) $(SYSCONFIG_TOOL) --product $(SYSCONFIG_PRODUCT) --output $(@D) $<

Board.obj: Board.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c @$(CONFIGPKG)/compiler.opt -o $@

main_tirtos.obj: ../../tirtos/main_tirtos.c Board.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c @$(CONFIGPKG)/compiler.opt -o $@

commonComms.obj: ../../commonComms.c Board.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c @$(CONFIGPKG)/compiler.opt -o $@

vibrationCapture.obj: ../../vibrationCapture.c Board.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c @$(CONFIGPKG)/compiler.opt -o $@

$(NAME).out: $(OBJECTS) $(CONFIGPKG)/linker.cmd
	@ echo linking $@
	@ $(LNK)  $(OBJECTS) $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
	@ $(RM) Board.c Board.h  > $(DEVNULL) 2>&1
