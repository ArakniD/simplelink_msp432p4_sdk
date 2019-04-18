
SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../..)
include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

# Find all subdirectories
NORTOS-CCS-SUBDIRS   = $(wildcard */*/ccs/.)
NORTOS-GCC-SUBDIRS   = $(wildcard */*/gcc/.)
NORTOS-IAR-SUBDIRS   = $(wildcard */*/iar/.)

NORTOS-SUBDIRS =
ifneq ($(CCS_ARMCOMPILER),)
	NORTOS-SUBDIRS += $(NORTOS-CCS-SUBDIRS)
endif
ifneq ($(GCC_ARMCOMPILER),)
	NORTOS-SUBDIRS += $(NORTOS-GCC-SUBDIRS)
endif
ifneq ($(IAR_ARMCOMPILER),)
	NORTOS-SUBDIRS += $(NORTOS-IAR-SUBDIRS)
endif

.PHONY: nortos $(NORTOS-SUBDIRS)

all: nortos

nortos: $(NORTOS-SUBDIRS)

$(NORTOS-SUBDIRS):
	@ $(MAKE) -C $@

CLEAN-NORTOS-SUBDIRS = $(addprefix clean-,$(NORTOS-SUBDIRS))

.PHONY: clean-nortos $(CLEAN-NORTOS-SUBDIRS)

clean: clean-nortos

clean-nortos: $(CLEAN-NORTOS-SUBDIRS)

$(CLEAN-NORTOS-SUBDIRS):
	@ $(MAKE) -C $(subst clean-,,$@) clean
