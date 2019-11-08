SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

CC = "$(IAR_ARMCOMPILER)/bin/iccarm"
LNK = "$(IAR_ARMCOMPILER)/bin/ilinkarm"

OBJECTS = main.obj adc14.obj aes256.obj comp_e.obj cpu.obj crc32.obj cs.obj dma.obj flash_a.obj fpu.obj gpio.obj i2c.obj interrupt.obj lcd_f.obj mpu.obj pcm.obj pmap.obj pss.obj ref_a.obj reset.obj rtc_c.obj spi.obj sysctl_a.obj systick.obj timer32.obj timer_a.obj uart.obj wdt_a.obj system_msp432p4111.obj iar_startup_msp432p4111_ewarm.obj

NAME = driverlib_empty_project_from_source

CFLAGS = -I.. \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" \
    -D__MSP432P4111__ \
    -DDeviceFamily_MSP432P4x1xI \
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
    "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/devices/msp432p4xx/driverlib/iar/msp432p4xx_driverlib.a" \
    --config ../iar/msp432p4111.icf \
    --map "$(NAME).map" \
    --silent \
    --cpu=Cortex-M4F \
    --semihosting=iar_breakpoint

all: $(NAME).out

main.obj: ../main.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

adc14.obj: ../adc14.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

aes256.obj: ../aes256.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

comp_e.obj: ../comp_e.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

cpu.obj: ../cpu.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

crc32.obj: ../crc32.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

cs.obj: ../cs.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

dma.obj: ../dma.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

flash_a.obj: ../flash_a.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

fpu.obj: ../fpu.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

gpio.obj: ../gpio.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

i2c.obj: ../i2c.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

interrupt.obj: ../interrupt.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

lcd_f.obj: ../lcd_f.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

mpu.obj: ../mpu.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

pcm.obj: ../pcm.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

pmap.obj: ../pmap.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

pss.obj: ../pss.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

ref_a.obj: ../ref_a.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

reset.obj: ../reset.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

rtc_c.obj: ../rtc_c.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

spi.obj: ../spi.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

sysctl_a.obj: ../sysctl_a.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

systick.obj: ../systick.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

timer32.obj: ../timer32.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

timer_a.obj: ../timer_a.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

uart.obj: ../uart.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

wdt_a.obj: ../wdt_a.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

system_msp432p4111.obj: ../system_msp432p4111.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

iar_startup_msp432p4111_ewarm.obj: ../iar/startup_msp432p4111_ewarm.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

$(NAME).out: $(OBJECTS)
	@ echo linking $@
	@ $(LNK)  $(OBJECTS)  $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
