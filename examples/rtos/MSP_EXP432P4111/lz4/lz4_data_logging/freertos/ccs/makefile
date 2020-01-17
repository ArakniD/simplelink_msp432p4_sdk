SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../../../../..)

include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

KERNEL_BUILD := $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/freertos/builds/MSP_EXP432P4111/release

CC = "$(CCS_ARMCOMPILER)/bin/armcl"
LNK = "$(CCS_ARMCOMPILER)/bin/armcl"

OBJECTS = compress_thread.obj sensor_thread.obj MSP_EXP432P4111.obj freertos_main_freertos.obj

NAME = lz4_data_logging

CFLAGS = -I../.. \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" \
    -D__MSP432P4111__ \
    -DDeviceFamily_MSP432P4x1xI \
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

LFLAGS = "-l$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/compression/lz4/lib/ccs/m4f/lz4.a" \
    "-i$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    -lti/display/lib/display.aem4f \
    -lti/grlib/lib/ccs/m4f/grlib.a \
    -lthird_party/spiffs/lib/ccs/m4f/spiffs.a \
    -lti/drivers/lib/drivers_msp432p4x1xi.aem4f \
    -lthird_party/fatfs/lib/ccs/m4f/fatfs.a \
    --diag_suppress=10063 \
    "-l$(KERNEL_BUILD)/ccs/freertos.lib" \
    -lti/devices/msp432p4xx/driverlib/ccs/msp432p4xx_driverlib.lib \
    ../../freertos/ccs/MSP_EXP432P4111_FREERTOS.cmd \
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

compress_thread.obj: ../../compress_thread.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

sensor_thread.obj: ../../sensor_thread.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

MSP_EXP432P4111.obj: ../../MSP_EXP432P4111.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

freertos_main_freertos.obj: ../../freertos/main_freertos.c
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
