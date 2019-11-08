SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

CC = "$(CCS_ARMCOMPILER)/bin/armcl"
LNK = "$(CCS_ARMCOMPILER)/bin/armcl"

OBJECTS = spiffsinternal.obj main_nortos.obj ti_drivers_config.obj

NAME = spiffsinternal

CFLAGS = -I.. \
    -I. \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" \
    --advice:power=none \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/nortos" \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/nortos/posix" \
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
    --entry_point=resetISR \
    "-i$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/nortos" \
    -llib/nortos_msp432p401x.aem4f \
    -lti/devices/msp432p4xx/driverlib/ccs/msp432p4xx_driverlib.lib \
    ../ccs/MSP_EXP432P401R_NoRTOS.cmd \
    "-m$(NAME).map" \
    --warn_sections \
    --display_error_number \
    --diag_wrap=off \
    --rom_model \
    "-i$(CCS_ARMCOMPILER)/lib" \
    -llibc.a

all: $(NAME).out

.INTERMEDIATE: syscfg
ti_drivers_config.c ti_drivers_config.h: syscfg
	@ echo generation complete

syscfg: ../spiffsinternal.syscfg
	@ echo Generating configuration files...
	@ $(SYSCONFIG_TOOL) --product $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/.metadata/product.json --output $(@D) $<

spiffsinternal.obj: ../spiffsinternal.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

main_nortos.obj: ../main_nortos.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

ti_drivers_config.obj: ti_drivers_config.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

$(NAME).out: $(OBJECTS)
	@ echo linking $@
	@ $(LNK) -z $(OBJECTS)  $(LFLAGS) -o $(NAME).out


clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
	@ $(RM) ti_drivers_config.c ti_drivers_config.h > $(DEVNULL) 2>&1
