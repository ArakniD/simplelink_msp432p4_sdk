#
# Set location of various cgtools
#
# These variables can be set here or on the command line.
#
# The CCS_ARMCOMPILER, GCC_ARMCOMPILER, and IAR_ARMCOMPILER variables,
# in addition to pointing to their respective locations, also serve
# as "switches" for disabling a build using those cgtools. To disable a
# build using a specific cgtool, either set the cgtool's variable to
# empty or delete/comment-out its definition:
#     IAR_ARMCOMPILER ?=
# or
#     #IAR_ARMCOMPILER ?= c:/Program Files (x86)/IAR Systems/Embedded Workbench 8.0/arm
#
# If a cgtool's *_ARMCOMPILER variable is set (non-empty), various sub-makes
# in the installation will attempt to build with that cgtool.  This means
# that if multiple *_ARMCOMPILER cgtool variables are set, the sub-makes
# will build using each non-empty *_ARMCOMPILER cgtool.
#

XDC_INSTALL_DIR        ?= c:/ti/xdctools_3_51_01_18_core
SYSCONFIG_TOOL         ?= c:/ti/ccsv8/utils/sysconfig/cli.js
NODE_JS                ?= c:/ti/ccsv8/tools/node/node.exe

FREERTOS_INSTALL_DIR   ?= c:/FreeRTOSv10.0.0

CCS_ARMCOMPILER        ?= c:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS
GCC_ARMCOMPILER        ?= c:/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major-win32
IAR_ARMCOMPILER        ?= c:/Program Files (x86)/IAR Systems/Embedded Workbench 8.0/arm

ifeq ("$(SHELL)","sh.exe")
# for Windows/DOS shell
    RM     = del
    RMDIR  = -rmdir /S /Q
    DEVNULL = NUL
    ECHOBLANKLINE = @cmd /c echo.
else
# for Linux-like shells
    RM     = rm -f
    RMDIR  = rm -rf
    DEVNULL = /dev/null
    ECHOBLANKLINE = echo
endif
