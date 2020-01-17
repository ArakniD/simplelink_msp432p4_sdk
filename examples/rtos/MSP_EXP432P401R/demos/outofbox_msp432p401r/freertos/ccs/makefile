SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../../../../..)

include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

KERNEL_BUILD := $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/freertos/builds/MSP_EXP432P401R/release

CC = "$(CCS_ARMCOMPILER)/bin/armcl"
LNK = "$(CCS_ARMCOMPILER)/bin/armcl"

OBJECTS = callbacks.obj led_thread.obj uart_thread.obj ti_drivers_config.obj freertos_main_freertos.obj

NAME = outofbox_msp432p401r

CFLAGS = -I../.. \
    -I. \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" \
    -D__MSP432P401R__ \
    -DDeviceFamily_MSP432P401x \
    --advice:power=none \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/posix/ccs" \
    "-I$(FREERTOS_INSTALL_DIR)/FreeRTOS/Source/include" \
    "-I$(FREERTOS_INSTALL_DIR)/FreeRTOS/Source/portable/CCS/ARM_CM4F" \
    "-I$(KERNEL_BUILD)" \
    --silicon_version=7M4 \
    --code_state=16 \
    --little_endian \
    --display_error_number \
    --diag_warning=255 \
    --diag_wrap=off \
    --gen_func_subsections=on \
    --float_support=FPv4SPD16 \
    --symdebug:dwarf \
    "-I$(CCS_ARMCOMPILER)/include"

LFLAGS = "-i$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    -lti/display/lib/display.aem4f \
    -lti/grlib/lib/ccs/m4f/grlib.a \
    -lthird_party/spiffs/lib/ccs/m4f/spiffs.a \
    -lti/drivers/lib/drivers_msp432p401x.aem4f \
    -lthird_party/fatfs/lib/ccs/m4f/fatfs.a \
    --diag_suppress=10063 \
    "-l$(KERNEL_BUILD)/ccs/freertos.lib" \
    -lti/devices/msp432p4xx/driverlib/ccs/msp432p4xx_driverlib.lib \
    ../../freertos/ccs/MSP_EXP432P401R_FREERTOS.cmd \
    "-m$(NAME).map" \
    --warn_sections \
    --display_error_number \
    --diag_wrap=off \
    --rom_model \
    "-i$(CCS_ARMCOMPILER)/lib" \
    -llibc.a

all: $(NAME).out

$(KERNEL_BUILD)/ccs/freertos.lib:
	@ $(ECHOBLANKLINE)
	@ echo $@ is not built.
	@ echo You can build it by issuing $(MAKE) in $(dir $@).
	@ $(ECHOBLANKLINE)

.INTERMEDIATE: syscfg
ti_drivers_config.c ti_drivers_config.h: syscfg
	@ echo generation complete

syscfg: ../../outofbox_msp432p401r.syscfg
	@ echo Generating configuration files...
	@ $(SYSCONFIG_TOOL) --compiler ccs --product $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/.metadata/product.json --output $(@D) $<

callbacks.obj: ../../callbacks.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

led_thread.obj: ../../led_thread.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

uart_thread.obj: ../../uart_thread.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

freertos_main_freertos.obj: ../../freertos/main_freertos.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

ti_drivers_config.obj: ti_drivers_config.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

$(NAME).out: $(OBJECTS) $(KERNEL_BUILD)/ccs/freertos.lib
	@ echo linking $@
	@ $(LNK) -z $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
	@ $(RM) ti_drivers_config.c ti_drivers_config.h > $(DEVNULL) 2>&1
