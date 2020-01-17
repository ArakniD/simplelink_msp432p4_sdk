SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

CC = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"
LNK = "$(GCC_ARMCOMPILER)/bin/arm-none-eabi-gcc"

OBJECTS = main.obj adc14.obj aes256.obj comp_e.obj cpu.obj crc32.obj cs.obj dma.obj flash.obj fpu.obj gpio.obj i2c.obj interrupt.obj mpu.obj pcm.obj pmap.obj pss.obj ref_a.obj reset.obj rtc_c.obj spi.obj sysctl.obj systick.obj timer32.obj timer_a.obj uart.obj wdt_a.obj system_msp432p401r.obj gcc_startup_msp432p401r_gcc.obj

NAME = driverlib_empty_project_from_source

CFLAGS = -I.. \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" \
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

LFLAGS = -Wl,-T,../gcc/msp432p401r.lds \
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

main.obj: ../main.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

adc14.obj: ../adc14.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

aes256.obj: ../aes256.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

comp_e.obj: ../comp_e.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

cpu.obj: ../cpu.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

crc32.obj: ../crc32.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

cs.obj: ../cs.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

dma.obj: ../dma.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

flash.obj: ../flash.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

fpu.obj: ../fpu.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

gpio.obj: ../gpio.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

i2c.obj: ../i2c.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

interrupt.obj: ../interrupt.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

mpu.obj: ../mpu.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

pcm.obj: ../pcm.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

pmap.obj: ../pmap.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

pss.obj: ../pss.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

ref_a.obj: ../ref_a.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

reset.obj: ../reset.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

rtc_c.obj: ../rtc_c.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

spi.obj: ../spi.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

sysctl.obj: ../sysctl.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

systick.obj: ../systick.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

timer32.obj: ../timer32.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

timer_a.obj: ../timer_a.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

uart.obj: ../uart.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -c -o $@

wdt_a.obj: ../wdt_a.c
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
