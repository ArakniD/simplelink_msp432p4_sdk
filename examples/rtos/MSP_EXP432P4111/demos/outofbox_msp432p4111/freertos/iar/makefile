SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../../../../..)

include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

KERNEL_BUILD := $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/kernel/freertos/builds/MSP_EXP432P4111/release

CC = "$(IAR_ARMCOMPILER)/bin/iccarm"
LNK = "$(IAR_ARMCOMPILER)/bin/ilinkarm"

OBJECTS = callbacks.obj lcd_thread.obj led_thread.obj temperature_thread.obj uart_thread.obj ti_drivers_config.obj cJSON_cJSON.obj LCD_HAL_lcd_hal.obj freertos_main_freertos.obj

NAME = outofbox_msp432p4111

CFLAGS = -I../.. \
    -I. \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" \
    -D__MSP432P4111__ \
    -DDeviceFamily_MSP432P4x1xI \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/posix/iar" \
    "-I$(FREERTOS_INSTALL_DIR)/FreeRTOS/Source/include" \
    "-I$(FREERTOS_INSTALL_DIR)/FreeRTOS/Source/portable/IAR/ARM_CM4F" \
    "-I$(KERNEL_BUILD)" \
    --cpu=Cortex-M4F \
    --debug \
    --silent \
    -e \
    --aeabi \
    --thumb \
    --diag_suppress=Pa050

LFLAGS = "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/display/lib/display.arm4f" \
    "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/grlib/lib/iar/m4f/grlib.a" \
    "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/spiffs/lib/iar/m4f/spiffs.a" \
    "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/drivers/lib/drivers_msp432p4x1xi.arm4f" \
    "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/fatfs/lib/iar/m4f/fatfs.a" \
    --threaded_lib \
    "$(KERNEL_BUILD)/iar/freertos.lib" \
    "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/devices/msp432p4xx/driverlib/iar/msp432p4xx_driverlib.a" \
    --config ../../freertos/iar/MSP_EXP432P4111_FREERTOS.icf \
    --map "$(NAME).map" \
    --silent \
    --cpu=Cortex-M4F \
    --semihosting=iar_breakpoint

all: $(NAME).out

$(KERNEL_BUILD)/iar/freertos.lib:
	@ $(ECHOBLANKLINE)
	@ echo $@ is not built.
	@ echo You can build it by issuing $(MAKE) in $(dir $@).
	@ $(ECHOBLANKLINE)

.INTERMEDIATE: syscfg
ti_drivers_config.c ti_drivers_config.h: syscfg
	@ echo generation complete

syscfg: ../../outOfBox_msp432p4111.syscfg
	@ echo Generating configuration files...
	@ $(SYSCONFIG_TOOL) --compiler iar --product $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/.metadata/product.json --output $(@D) $<

callbacks.obj: ../../callbacks.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

lcd_thread.obj: ../../lcd_thread.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

led_thread.obj: ../../led_thread.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

temperature_thread.obj: ../../temperature_thread.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

uart_thread.obj: ../../uart_thread.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

cJSON_cJSON.obj: ../../cJSON/cJSON.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

LCD_HAL_lcd_hal.obj: ../../LCD_HAL/lcd_hal.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

freertos_main_freertos.obj: ../../freertos/main_freertos.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ti_drivers_config.obj: ti_drivers_config.c ti_drivers_config.h
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

$(NAME).out: $(OBJECTS) $(KERNEL_BUILD)/iar/freertos.lib
	@ echo linking $@
	@ $(LNK)  $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
	@ $(RM) ti_drivers_config.c ti_drivers_config.h > $(DEVNULL) 2>&1
