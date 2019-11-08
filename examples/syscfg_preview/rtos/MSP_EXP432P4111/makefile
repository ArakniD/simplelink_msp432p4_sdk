SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../..)
include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

# Find all subdirectories
TIRTOS-CCS-SUBDIRS = $(wildcard */*/tirtos/ccs/.)
TIRTOS-GCC-SUBDIRS = $(wildcard */*/tirtos/gcc/.)
TIRTOS-IAR-SUBDIRS = $(wildcard */*/tirtos/iar/.)

TIRTOS-SUBDIRS =
ifneq ($(CCS_ARMCOMPILER),)
	TIRTOS-SUBDIRS += $(TIRTOS-CCS-SUBDIRS)
endif
ifneq ($(GCC_ARMCOMPILER),)
	TIRTOS-SUBDIRS += $(TIRTOS-GCC-SUBDIRS)
endif
ifneq ($(IAR_ARMCOMPILER),)
	TIRTOS-SUBDIRS += $(TIRTOS-IAR-SUBDIRS)
endif

.PHONY: tirtos $(TIRTOS-SUBDIRS)

all: tirtos

tirtos: $(TIRTOS-SUBDIRS)

$(TIRTOS-SUBDIRS):
	@ $(MAKE) -C $@

CLEAN-TIRTOS-SUBDIRS = $(addprefix clean-,$(TIRTOS-SUBDIRS))

.PHONY: clean-tirtos $(CLEAN-TIRTOS-SUBDIRS)

clean: clean-tirtos

clean-tirtos: $(CLEAN-TIRTOS-SUBDIRS)

$(CLEAN-TIRTOS-SUBDIRS):
	@ $(MAKE) -C $(subst clean-,,$@) clean
