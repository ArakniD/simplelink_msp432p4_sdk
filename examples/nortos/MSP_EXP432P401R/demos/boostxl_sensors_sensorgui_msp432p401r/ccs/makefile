SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

CC = "$(CCS_ARMCOMPILER)/bin/armcl"
LNK = "$(CCS_ARMCOMPILER)/bin/armcl"

OBJECTS = bme280.obj bme280_support.obj bmi160.obj bmi160_support.obj demo_sysctl.obj i2c_driver.obj main.obj opt3001.obj tmp007.obj uart_driver.obj system_msp432p401r.obj ccs_startup_msp432p401r_ccs.obj

NAME = boostxl_sensors_sensorgui_msp432p401r

CFLAGS = -I.. \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" \
    -D__MSP432P401R__ \
    -DDeviceFamily_MSP432P401x \
    --advice:power=none \
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

LFLAGS = --stack_size=512 \
    "-i$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    -lti/display/lib/display.aem4f \
    -lti/grlib/lib/ccs/m4f/grlib.a \
    -lthird_party/spiffs/lib/ccs/m4f/spiffs.a \
    -lti/drivers/lib/drivers_msp432p401x.aem4f \
    -lthird_party/fatfs/lib/ccs/m4f/fatfs.a \
    -lti/devices/msp432p4xx/driverlib/ccs/msp432p4xx_driverlib.lib \
    ../ccs/msp432p401r.cmd \
    "-m$(NAME).map" \
    --warn_sections \
    --display_error_number \
    --diag_wrap=off \
    --rom_model \
    "-i$(CCS_ARMCOMPILER)/lib" \
    -llibc.a

all: $(NAME).out

bme280.obj: ../bme280.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

bme280_support.obj: ../bme280_support.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

bmi160.obj: ../bmi160.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

bmi160_support.obj: ../bmi160_support.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

demo_sysctl.obj: ../demo_sysctl.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

i2c_driver.obj: ../i2c_driver.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

main.obj: ../main.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

opt3001.obj: ../opt3001.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

tmp007.obj: ../tmp007.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

uart_driver.obj: ../uart_driver.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

system_msp432p401r.obj: ../system_msp432p401r.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

ccs_startup_msp432p401r_ccs.obj: ../ccs/startup_msp432p401r_ccs.c
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
