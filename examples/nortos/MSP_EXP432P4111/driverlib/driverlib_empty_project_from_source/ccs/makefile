SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

CC = "$(CCS_ARMCOMPILER)/bin/armcl"
LNK = "$(CCS_ARMCOMPILER)/bin/armcl"

OBJECTS = main.obj adc14.obj aes256.obj comp_e.obj cpu.obj crc32.obj cs.obj dma.obj flash_a.obj fpu.obj gpio.obj i2c.obj interrupt.obj lcd_f.obj mpu.obj pcm.obj pmap.obj pss.obj ref_a.obj reset.obj rtc_c.obj spi.obj sysctl_a.obj systick.obj timer32.obj timer_a.obj uart.obj wdt_a.obj system_msp432p4111.obj ccs_startup_msp432p4111_ccs.obj

NAME = driverlib_empty_project_from_source

CFLAGS = -I.. \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" \
    "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" \
    -D__MSP432P4111__ \
    -DDeviceFamily_MSP432P4x1xI \
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
    -lti/drivers/lib/drivers_msp432p4x1xi.aem4f \
    -lthird_party/fatfs/lib/ccs/m4f/fatfs.a \
    -lti/devices/msp432p4xx/driverlib/ccs/msp432p4xx_driverlib.lib \
    ../ccs/msp432p4111.cmd \
    "-m$(NAME).map" \
    --warn_sections \
    --display_error_number \
    --diag_wrap=off \
    --rom_model \
    "-i$(CCS_ARMCOMPILER)/lib" \
    -llibc.a

all: $(NAME).out

main.obj: ../main.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

adc14.obj: ../adc14.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

aes256.obj: ../aes256.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

comp_e.obj: ../comp_e.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

cpu.obj: ../cpu.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

crc32.obj: ../crc32.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

cs.obj: ../cs.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

dma.obj: ../dma.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

flash_a.obj: ../flash_a.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

fpu.obj: ../fpu.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

gpio.obj: ../gpio.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

i2c.obj: ../i2c.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

interrupt.obj: ../interrupt.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

lcd_f.obj: ../lcd_f.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

mpu.obj: ../mpu.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

pcm.obj: ../pcm.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

pmap.obj: ../pmap.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

pss.obj: ../pss.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

ref_a.obj: ../ref_a.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

reset.obj: ../reset.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

rtc_c.obj: ../rtc_c.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

spi.obj: ../spi.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

sysctl_a.obj: ../sysctl_a.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

systick.obj: ../systick.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

timer32.obj: ../timer32.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

timer_a.obj: ../timer_a.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

uart.obj: ../uart.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

wdt_a.obj: ../wdt_a.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

system_msp432p4111.obj: ../system_msp432p4111.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --output_file=$@

ccs_startup_msp432p4111_ccs.obj: ../ccs/startup_msp432p4111_ccs.c
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
