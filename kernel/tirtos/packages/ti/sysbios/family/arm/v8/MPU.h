/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-F14
 */

/*
 * ======== GENERATED SECTIONS ========
 *
 *     PROLOGUE
 *     INCLUDES
 *
 *     INTERNAL DEFINITIONS
 *     MODULE-WIDE CONFIGS
 *     FUNCTION DECLARATIONS
 *     SYSTEM FUNCTIONS
 *
 *     EPILOGUE
 *     STATE STRUCTURES
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef ti_sysbios_family_arm_v8_MPU__include
#define ti_sysbios_family_arm_v8_MPU__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_arm_v8_MPU__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_arm_v8_MPU___VERS 200


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/arm/v8/package/package.defs.h>

#include <xdc/runtime/IModule.h>
#include <xdc/runtime/Assert.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* DeviceRegs */
struct ti_sysbios_family_arm_v8_MPU_DeviceRegs {
    xdc_UInt32 TYPE;
    xdc_UInt32 CTRL;
    xdc_UInt32 RNR;
    xdc_UInt32 RBAR;
    xdc_UInt32 RLAR;
    xdc_UInt32 RBAR_A1;
    xdc_UInt32 RLAR_A1;
    xdc_UInt32 RBAR_A2;
    xdc_UInt32 RLAR_A2;
    xdc_UInt32 RBAR_A3;
    xdc_UInt32 RLAR_A3;
    xdc_UInt32 res0;
    xdc_UInt32 MAIR0;
    xdc_UInt32 MAIR1;
};

/* deviceRegs */
__extern volatile ti_sysbios_family_arm_v8_MPU_DeviceRegs ti_sysbios_family_arm_v8_MPU_deviceRegs;

/* AccessPerm */
enum ti_sysbios_family_arm_v8_MPU_AccessPerm {
    ti_sysbios_family_arm_v8_MPU_AccessPerm_RW_PRIV = 0x0,
    ti_sysbios_family_arm_v8_MPU_AccessPerm_RW_ANY = 0x1,
    ti_sysbios_family_arm_v8_MPU_AccessPerm_RO_PRIV = 0x2,
    ti_sysbios_family_arm_v8_MPU_AccessPerm_RO_ANY = 0x3
};
typedef enum ti_sysbios_family_arm_v8_MPU_AccessPerm ti_sysbios_family_arm_v8_MPU_AccessPerm;

/* Shareable */
enum ti_sysbios_family_arm_v8_MPU_Shareable {
    ti_sysbios_family_arm_v8_MPU_Shareable_NONE = 0x0,
    ti_sysbios_family_arm_v8_MPU_Shareable_OUTER = 0x2,
    ti_sysbios_family_arm_v8_MPU_Shareable_INNER = 0x3
};
typedef enum ti_sysbios_family_arm_v8_MPU_Shareable ti_sysbios_family_arm_v8_MPU_Shareable;

/* RegionAttrs */
struct ti_sysbios_family_arm_v8_MPU_RegionAttrs {
    xdc_Bool enable;
    ti_sysbios_family_arm_v8_MPU_Shareable shareable;
    xdc_Bool executable;
    ti_sysbios_family_arm_v8_MPU_AccessPerm accessPerm;
    xdc_UInt8 attrIndx;
};


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* RegionEntry */
struct ti_sysbios_family_arm_v8_MPU_RegionEntry {
    xdc_UInt32 rbar;
    xdc_UInt32 rlar;
};

/* Module_State */
typedef ti_sysbios_family_arm_v8_MPU_RegionEntry __T1_ti_sysbios_family_arm_v8_MPU_Module_State__regionEntry;
typedef ti_sysbios_family_arm_v8_MPU_RegionEntry *ARRAY1_ti_sysbios_family_arm_v8_MPU_Module_State__regionEntry;
typedef const ti_sysbios_family_arm_v8_MPU_RegionEntry *CARRAY1_ti_sysbios_family_arm_v8_MPU_Module_State__regionEntry;
typedef ARRAY1_ti_sysbios_family_arm_v8_MPU_Module_State__regionEntry __TA_ti_sysbios_family_arm_v8_MPU_Module_State__regionEntry;


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_arm_v8_MPU_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Module__diagsEnabled ti_sysbios_family_arm_v8_MPU_Module__diagsEnabled__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Module__diagsEnabled__CR
#define ti_sysbios_family_arm_v8_MPU_Module__diagsEnabled__C (*((CT__ti_sysbios_family_arm_v8_MPU_Module__diagsEnabled*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Module__diagsEnabled__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Module__diagsEnabled (ti_sysbios_family_arm_v8_MPU_Module__diagsEnabled__C)
#endif

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_arm_v8_MPU_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Module__diagsIncluded ti_sysbios_family_arm_v8_MPU_Module__diagsIncluded__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Module__diagsIncluded__CR
#define ti_sysbios_family_arm_v8_MPU_Module__diagsIncluded__C (*((CT__ti_sysbios_family_arm_v8_MPU_Module__diagsIncluded*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Module__diagsIncluded__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Module__diagsIncluded (ti_sysbios_family_arm_v8_MPU_Module__diagsIncluded__C)
#endif

/* Module__diagsMask */
typedef xdc_Bits16 *CT__ti_sysbios_family_arm_v8_MPU_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Module__diagsMask ti_sysbios_family_arm_v8_MPU_Module__diagsMask__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Module__diagsMask__CR
#define ti_sysbios_family_arm_v8_MPU_Module__diagsMask__C (*((CT__ti_sysbios_family_arm_v8_MPU_Module__diagsMask*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Module__diagsMask__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Module__diagsMask (ti_sysbios_family_arm_v8_MPU_Module__diagsMask__C)
#endif

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_arm_v8_MPU_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Module__gateObj ti_sysbios_family_arm_v8_MPU_Module__gateObj__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Module__gateObj__CR
#define ti_sysbios_family_arm_v8_MPU_Module__gateObj__C (*((CT__ti_sysbios_family_arm_v8_MPU_Module__gateObj*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Module__gateObj__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Module__gateObj (ti_sysbios_family_arm_v8_MPU_Module__gateObj__C)
#endif

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_arm_v8_MPU_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Module__gatePrms ti_sysbios_family_arm_v8_MPU_Module__gatePrms__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Module__gatePrms__CR
#define ti_sysbios_family_arm_v8_MPU_Module__gatePrms__C (*((CT__ti_sysbios_family_arm_v8_MPU_Module__gatePrms*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Module__gatePrms__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Module__gatePrms (ti_sysbios_family_arm_v8_MPU_Module__gatePrms__C)
#endif

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_arm_v8_MPU_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Module__id ti_sysbios_family_arm_v8_MPU_Module__id__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Module__id__CR
#define ti_sysbios_family_arm_v8_MPU_Module__id__C (*((CT__ti_sysbios_family_arm_v8_MPU_Module__id*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Module__id__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Module__id (ti_sysbios_family_arm_v8_MPU_Module__id__C)
#endif

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_arm_v8_MPU_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Module__loggerDefined ti_sysbios_family_arm_v8_MPU_Module__loggerDefined__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Module__loggerDefined__CR
#define ti_sysbios_family_arm_v8_MPU_Module__loggerDefined__C (*((CT__ti_sysbios_family_arm_v8_MPU_Module__loggerDefined*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Module__loggerDefined__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Module__loggerDefined (ti_sysbios_family_arm_v8_MPU_Module__loggerDefined__C)
#endif

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_arm_v8_MPU_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Module__loggerObj ti_sysbios_family_arm_v8_MPU_Module__loggerObj__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Module__loggerObj__CR
#define ti_sysbios_family_arm_v8_MPU_Module__loggerObj__C (*((CT__ti_sysbios_family_arm_v8_MPU_Module__loggerObj*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Module__loggerObj__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Module__loggerObj (ti_sysbios_family_arm_v8_MPU_Module__loggerObj__C)
#endif

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn0 ti_sysbios_family_arm_v8_MPU_Module__loggerFxn0__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Module__loggerFxn0__CR
#define ti_sysbios_family_arm_v8_MPU_Module__loggerFxn0__C (*((CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn0*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Module__loggerFxn0__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Module__loggerFxn0 (ti_sysbios_family_arm_v8_MPU_Module__loggerFxn0__C)
#endif

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn1 ti_sysbios_family_arm_v8_MPU_Module__loggerFxn1__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Module__loggerFxn1__CR
#define ti_sysbios_family_arm_v8_MPU_Module__loggerFxn1__C (*((CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn1*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Module__loggerFxn1__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Module__loggerFxn1 (ti_sysbios_family_arm_v8_MPU_Module__loggerFxn1__C)
#endif

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn2 ti_sysbios_family_arm_v8_MPU_Module__loggerFxn2__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Module__loggerFxn2__CR
#define ti_sysbios_family_arm_v8_MPU_Module__loggerFxn2__C (*((CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn2*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Module__loggerFxn2__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Module__loggerFxn2 (ti_sysbios_family_arm_v8_MPU_Module__loggerFxn2__C)
#endif

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn4 ti_sysbios_family_arm_v8_MPU_Module__loggerFxn4__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Module__loggerFxn4__CR
#define ti_sysbios_family_arm_v8_MPU_Module__loggerFxn4__C (*((CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn4*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Module__loggerFxn4__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Module__loggerFxn4 (ti_sysbios_family_arm_v8_MPU_Module__loggerFxn4__C)
#endif

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn8 ti_sysbios_family_arm_v8_MPU_Module__loggerFxn8__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Module__loggerFxn8__CR
#define ti_sysbios_family_arm_v8_MPU_Module__loggerFxn8__C (*((CT__ti_sysbios_family_arm_v8_MPU_Module__loggerFxn8*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Module__loggerFxn8__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Module__loggerFxn8 (ti_sysbios_family_arm_v8_MPU_Module__loggerFxn8__C)
#endif

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_arm_v8_MPU_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Object__count ti_sysbios_family_arm_v8_MPU_Object__count__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Object__count__CR
#define ti_sysbios_family_arm_v8_MPU_Object__count__C (*((CT__ti_sysbios_family_arm_v8_MPU_Object__count*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Object__count__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Object__count (ti_sysbios_family_arm_v8_MPU_Object__count__C)
#endif

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_arm_v8_MPU_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Object__heap ti_sysbios_family_arm_v8_MPU_Object__heap__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Object__heap__CR
#define ti_sysbios_family_arm_v8_MPU_Object__heap__C (*((CT__ti_sysbios_family_arm_v8_MPU_Object__heap*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Object__heap__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Object__heap (ti_sysbios_family_arm_v8_MPU_Object__heap__C)
#endif

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_arm_v8_MPU_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Object__sizeof ti_sysbios_family_arm_v8_MPU_Object__sizeof__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Object__sizeof__CR
#define ti_sysbios_family_arm_v8_MPU_Object__sizeof__C (*((CT__ti_sysbios_family_arm_v8_MPU_Object__sizeof*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Object__sizeof__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Object__sizeof (ti_sysbios_family_arm_v8_MPU_Object__sizeof__C)
#endif

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_arm_v8_MPU_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_Object__table ti_sysbios_family_arm_v8_MPU_Object__table__C;
#ifdef ti_sysbios_family_arm_v8_MPU_Object__table__CR
#define ti_sysbios_family_arm_v8_MPU_Object__table__C (*((CT__ti_sysbios_family_arm_v8_MPU_Object__table*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_Object__table__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_Object__table (ti_sysbios_family_arm_v8_MPU_Object__table__C)
#endif

/* defaultAttrs */
typedef ti_sysbios_family_arm_v8_MPU_RegionAttrs CT__ti_sysbios_family_arm_v8_MPU_defaultAttrs;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_defaultAttrs ti_sysbios_family_arm_v8_MPU_defaultAttrs__C;
#ifdef ti_sysbios_family_arm_v8_MPU_defaultAttrs__CR
#define ti_sysbios_family_arm_v8_MPU_defaultAttrs (*((CT__ti_sysbios_family_arm_v8_MPU_defaultAttrs*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_defaultAttrs__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_defaultAttrs (ti_sysbios_family_arm_v8_MPU_defaultAttrs__C)
#endif

/* A_nullPointer */
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_arm_v8_MPU_A_nullPointer;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_A_nullPointer ti_sysbios_family_arm_v8_MPU_A_nullPointer__C;
#ifdef ti_sysbios_family_arm_v8_MPU_A_nullPointer__CR
#define ti_sysbios_family_arm_v8_MPU_A_nullPointer (*((CT__ti_sysbios_family_arm_v8_MPU_A_nullPointer*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_A_nullPointer__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_A_nullPointer (ti_sysbios_family_arm_v8_MPU_A_nullPointer__C)
#endif

/* A_invalidRegionId */
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_arm_v8_MPU_A_invalidRegionId;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_A_invalidRegionId ti_sysbios_family_arm_v8_MPU_A_invalidRegionId__C;
#ifdef ti_sysbios_family_arm_v8_MPU_A_invalidRegionId__CR
#define ti_sysbios_family_arm_v8_MPU_A_invalidRegionId (*((CT__ti_sysbios_family_arm_v8_MPU_A_invalidRegionId*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_A_invalidRegionId__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_A_invalidRegionId (ti_sysbios_family_arm_v8_MPU_A_invalidRegionId__C)
#endif

/* A_unalignedBaseAddr */
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_arm_v8_MPU_A_unalignedBaseAddr;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_A_unalignedBaseAddr ti_sysbios_family_arm_v8_MPU_A_unalignedBaseAddr__C;
#ifdef ti_sysbios_family_arm_v8_MPU_A_unalignedBaseAddr__CR
#define ti_sysbios_family_arm_v8_MPU_A_unalignedBaseAddr (*((CT__ti_sysbios_family_arm_v8_MPU_A_unalignedBaseAddr*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_A_unalignedBaseAddr__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_A_unalignedBaseAddr (ti_sysbios_family_arm_v8_MPU_A_unalignedBaseAddr__C)
#endif

/* A_unalignedEndAddr */
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_arm_v8_MPU_A_unalignedEndAddr;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_A_unalignedEndAddr ti_sysbios_family_arm_v8_MPU_A_unalignedEndAddr__C;
#ifdef ti_sysbios_family_arm_v8_MPU_A_unalignedEndAddr__CR
#define ti_sysbios_family_arm_v8_MPU_A_unalignedEndAddr (*((CT__ti_sysbios_family_arm_v8_MPU_A_unalignedEndAddr*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_A_unalignedEndAddr__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_A_unalignedEndAddr (ti_sysbios_family_arm_v8_MPU_A_unalignedEndAddr__C)
#endif

/* enableMPU */
typedef xdc_Bool CT__ti_sysbios_family_arm_v8_MPU_enableMPU;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_enableMPU ti_sysbios_family_arm_v8_MPU_enableMPU__C;
#ifdef ti_sysbios_family_arm_v8_MPU_enableMPU__CR
#define ti_sysbios_family_arm_v8_MPU_enableMPU (*((CT__ti_sysbios_family_arm_v8_MPU_enableMPU*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_enableMPU__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8_MPU_enableMPU__D
#define ti_sysbios_family_arm_v8_MPU_enableMPU (ti_sysbios_family_arm_v8_MPU_enableMPU__D)
#else
#define ti_sysbios_family_arm_v8_MPU_enableMPU (ti_sysbios_family_arm_v8_MPU_enableMPU__C)
#endif
#endif

/* enableBackgroundRegion */
typedef xdc_Bool CT__ti_sysbios_family_arm_v8_MPU_enableBackgroundRegion;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_enableBackgroundRegion ti_sysbios_family_arm_v8_MPU_enableBackgroundRegion__C;
#ifdef ti_sysbios_family_arm_v8_MPU_enableBackgroundRegion__CR
#define ti_sysbios_family_arm_v8_MPU_enableBackgroundRegion (*((CT__ti_sysbios_family_arm_v8_MPU_enableBackgroundRegion*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_enableBackgroundRegion__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8_MPU_enableBackgroundRegion__D
#define ti_sysbios_family_arm_v8_MPU_enableBackgroundRegion (ti_sysbios_family_arm_v8_MPU_enableBackgroundRegion__D)
#else
#define ti_sysbios_family_arm_v8_MPU_enableBackgroundRegion (ti_sysbios_family_arm_v8_MPU_enableBackgroundRegion__C)
#endif
#endif

/* MAIR0 */
typedef xdc_UInt8 CT__ti_sysbios_family_arm_v8_MPU_MAIR0;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_MAIR0 ti_sysbios_family_arm_v8_MPU_MAIR0__C;
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR0__CR
#define ti_sysbios_family_arm_v8_MPU_MAIR0 (*((CT__ti_sysbios_family_arm_v8_MPU_MAIR0*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_MAIR0__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR0__D
#define ti_sysbios_family_arm_v8_MPU_MAIR0 (ti_sysbios_family_arm_v8_MPU_MAIR0__D)
#else
#define ti_sysbios_family_arm_v8_MPU_MAIR0 (ti_sysbios_family_arm_v8_MPU_MAIR0__C)
#endif
#endif

/* MAIR1 */
typedef xdc_UInt8 CT__ti_sysbios_family_arm_v8_MPU_MAIR1;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_MAIR1 ti_sysbios_family_arm_v8_MPU_MAIR1__C;
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR1__CR
#define ti_sysbios_family_arm_v8_MPU_MAIR1 (*((CT__ti_sysbios_family_arm_v8_MPU_MAIR1*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_MAIR1__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR1__D
#define ti_sysbios_family_arm_v8_MPU_MAIR1 (ti_sysbios_family_arm_v8_MPU_MAIR1__D)
#else
#define ti_sysbios_family_arm_v8_MPU_MAIR1 (ti_sysbios_family_arm_v8_MPU_MAIR1__C)
#endif
#endif

/* MAIR2 */
typedef xdc_UInt8 CT__ti_sysbios_family_arm_v8_MPU_MAIR2;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_MAIR2 ti_sysbios_family_arm_v8_MPU_MAIR2__C;
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR2__CR
#define ti_sysbios_family_arm_v8_MPU_MAIR2 (*((CT__ti_sysbios_family_arm_v8_MPU_MAIR2*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_MAIR2__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR2__D
#define ti_sysbios_family_arm_v8_MPU_MAIR2 (ti_sysbios_family_arm_v8_MPU_MAIR2__D)
#else
#define ti_sysbios_family_arm_v8_MPU_MAIR2 (ti_sysbios_family_arm_v8_MPU_MAIR2__C)
#endif
#endif

/* MAIR3 */
typedef xdc_UInt8 CT__ti_sysbios_family_arm_v8_MPU_MAIR3;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_MAIR3 ti_sysbios_family_arm_v8_MPU_MAIR3__C;
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR3__CR
#define ti_sysbios_family_arm_v8_MPU_MAIR3 (*((CT__ti_sysbios_family_arm_v8_MPU_MAIR3*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_MAIR3__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR3__D
#define ti_sysbios_family_arm_v8_MPU_MAIR3 (ti_sysbios_family_arm_v8_MPU_MAIR3__D)
#else
#define ti_sysbios_family_arm_v8_MPU_MAIR3 (ti_sysbios_family_arm_v8_MPU_MAIR3__C)
#endif
#endif

/* MAIR4 */
typedef xdc_UInt8 CT__ti_sysbios_family_arm_v8_MPU_MAIR4;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_MAIR4 ti_sysbios_family_arm_v8_MPU_MAIR4__C;
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR4__CR
#define ti_sysbios_family_arm_v8_MPU_MAIR4 (*((CT__ti_sysbios_family_arm_v8_MPU_MAIR4*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_MAIR4__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR4__D
#define ti_sysbios_family_arm_v8_MPU_MAIR4 (ti_sysbios_family_arm_v8_MPU_MAIR4__D)
#else
#define ti_sysbios_family_arm_v8_MPU_MAIR4 (ti_sysbios_family_arm_v8_MPU_MAIR4__C)
#endif
#endif

/* MAIR5 */
typedef xdc_UInt8 CT__ti_sysbios_family_arm_v8_MPU_MAIR5;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_MAIR5 ti_sysbios_family_arm_v8_MPU_MAIR5__C;
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR5__CR
#define ti_sysbios_family_arm_v8_MPU_MAIR5 (*((CT__ti_sysbios_family_arm_v8_MPU_MAIR5*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_MAIR5__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR5__D
#define ti_sysbios_family_arm_v8_MPU_MAIR5 (ti_sysbios_family_arm_v8_MPU_MAIR5__D)
#else
#define ti_sysbios_family_arm_v8_MPU_MAIR5 (ti_sysbios_family_arm_v8_MPU_MAIR5__C)
#endif
#endif

/* MAIR6 */
typedef xdc_UInt8 CT__ti_sysbios_family_arm_v8_MPU_MAIR6;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_MAIR6 ti_sysbios_family_arm_v8_MPU_MAIR6__C;
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR6__CR
#define ti_sysbios_family_arm_v8_MPU_MAIR6 (*((CT__ti_sysbios_family_arm_v8_MPU_MAIR6*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_MAIR6__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR6__D
#define ti_sysbios_family_arm_v8_MPU_MAIR6 (ti_sysbios_family_arm_v8_MPU_MAIR6__D)
#else
#define ti_sysbios_family_arm_v8_MPU_MAIR6 (ti_sysbios_family_arm_v8_MPU_MAIR6__C)
#endif
#endif

/* MAIR7 */
typedef xdc_UInt8 CT__ti_sysbios_family_arm_v8_MPU_MAIR7;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_MAIR7 ti_sysbios_family_arm_v8_MPU_MAIR7__C;
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR7__CR
#define ti_sysbios_family_arm_v8_MPU_MAIR7 (*((CT__ti_sysbios_family_arm_v8_MPU_MAIR7*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_MAIR7__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8_MPU_MAIR7__D
#define ti_sysbios_family_arm_v8_MPU_MAIR7 (ti_sysbios_family_arm_v8_MPU_MAIR7__D)
#else
#define ti_sysbios_family_arm_v8_MPU_MAIR7 (ti_sysbios_family_arm_v8_MPU_MAIR7__C)
#endif
#endif

/* numRegions */
typedef xdc_UInt8 CT__ti_sysbios_family_arm_v8_MPU_numRegions;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_numRegions ti_sysbios_family_arm_v8_MPU_numRegions__C;
#ifdef ti_sysbios_family_arm_v8_MPU_numRegions__CR
#define ti_sysbios_family_arm_v8_MPU_numRegions (*((CT__ti_sysbios_family_arm_v8_MPU_numRegions*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_numRegions__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8_MPU_numRegions__D
#define ti_sysbios_family_arm_v8_MPU_numRegions (ti_sysbios_family_arm_v8_MPU_numRegions__D)
#else
#define ti_sysbios_family_arm_v8_MPU_numRegions (ti_sysbios_family_arm_v8_MPU_numRegions__C)
#endif
#endif

/* regionEntry */
typedef ti_sysbios_family_arm_v8_MPU_RegionEntry __T1_ti_sysbios_family_arm_v8_MPU_regionEntry;
typedef ti_sysbios_family_arm_v8_MPU_RegionEntry *ARRAY1_ti_sysbios_family_arm_v8_MPU_regionEntry;
typedef const ti_sysbios_family_arm_v8_MPU_RegionEntry *CARRAY1_ti_sysbios_family_arm_v8_MPU_regionEntry;
typedef CARRAY1_ti_sysbios_family_arm_v8_MPU_regionEntry __TA_ti_sysbios_family_arm_v8_MPU_regionEntry;
typedef CARRAY1_ti_sysbios_family_arm_v8_MPU_regionEntry CT__ti_sysbios_family_arm_v8_MPU_regionEntry;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8_MPU_regionEntry ti_sysbios_family_arm_v8_MPU_regionEntry__C;
#ifdef ti_sysbios_family_arm_v8_MPU_regionEntry__CR
#define ti_sysbios_family_arm_v8_MPU_regionEntry (*((CT__ti_sysbios_family_arm_v8_MPU_regionEntry*)(xdcRomConstPtr + ti_sysbios_family_arm_v8_MPU_regionEntry__C_offset)))
#else
#define ti_sysbios_family_arm_v8_MPU_regionEntry (ti_sysbios_family_arm_v8_MPU_regionEntry__C)
#endif


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_arm_v8_MPU_Module_startup( state ) (-1)

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_arm_v8_MPU_Module__startupDone__S, "ti_sysbios_family_arm_v8_MPU_Module__startupDone__S")
__extern xdc_Bool ti_sysbios_family_arm_v8_MPU_Module__startupDone__S( void );

/* disable__E */
#define ti_sysbios_family_arm_v8_MPU_disable ti_sysbios_family_arm_v8_MPU_disable__E
xdc__CODESECT(ti_sysbios_family_arm_v8_MPU_disable__E, "ti_sysbios_family_arm_v8_MPU_disable")
__extern xdc_Void ti_sysbios_family_arm_v8_MPU_disable__E( void );

/* enable__E */
#define ti_sysbios_family_arm_v8_MPU_enable ti_sysbios_family_arm_v8_MPU_enable__E
xdc__CODESECT(ti_sysbios_family_arm_v8_MPU_enable__E, "ti_sysbios_family_arm_v8_MPU_enable")
__extern xdc_Void ti_sysbios_family_arm_v8_MPU_enable__E( void );

/* initRegionAttrs__E */
#define ti_sysbios_family_arm_v8_MPU_initRegionAttrs ti_sysbios_family_arm_v8_MPU_initRegionAttrs__E
xdc__CODESECT(ti_sysbios_family_arm_v8_MPU_initRegionAttrs__E, "ti_sysbios_family_arm_v8_MPU_initRegionAttrs")
__extern xdc_Void ti_sysbios_family_arm_v8_MPU_initRegionAttrs__E( ti_sysbios_family_arm_v8_MPU_RegionAttrs *regionAttrs );

/* isEnabled__E */
#define ti_sysbios_family_arm_v8_MPU_isEnabled ti_sysbios_family_arm_v8_MPU_isEnabled__E
xdc__CODESECT(ti_sysbios_family_arm_v8_MPU_isEnabled__E, "ti_sysbios_family_arm_v8_MPU_isEnabled")
__extern xdc_Bool ti_sysbios_family_arm_v8_MPU_isEnabled__E( void );

/* setMAIR__E */
#define ti_sysbios_family_arm_v8_MPU_setMAIR ti_sysbios_family_arm_v8_MPU_setMAIR__E
xdc__CODESECT(ti_sysbios_family_arm_v8_MPU_setMAIR__E, "ti_sysbios_family_arm_v8_MPU_setMAIR")
__extern xdc_Void ti_sysbios_family_arm_v8_MPU_setMAIR__E( xdc_UInt8 attrIndx, xdc_UInt8 attr );

/* setRegion__E */
#define ti_sysbios_family_arm_v8_MPU_setRegion ti_sysbios_family_arm_v8_MPU_setRegion__E
xdc__CODESECT(ti_sysbios_family_arm_v8_MPU_setRegion__E, "ti_sysbios_family_arm_v8_MPU_setRegion")
__extern xdc_Void ti_sysbios_family_arm_v8_MPU_setRegion__E( xdc_UInt8 regionId, xdc_Ptr regionBaseAddr, xdc_Ptr regionEndAddr, ti_sysbios_family_arm_v8_MPU_RegionAttrs *attrs );

/* startup__I */
#define ti_sysbios_family_arm_v8_MPU_startup ti_sysbios_family_arm_v8_MPU_startup__I
xdc__CODESECT(ti_sysbios_family_arm_v8_MPU_startup__I, "ti_sysbios_family_arm_v8_MPU_startup")
__extern xdc_Void ti_sysbios_family_arm_v8_MPU_startup__I( void );


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_arm_v8_MPU_Module_startupDone() ti_sysbios_family_arm_v8_MPU_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_arm_v8_MPU_Object_heap() ti_sysbios_family_arm_v8_MPU_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_arm_v8_MPU_Module_heap() ti_sysbios_family_arm_v8_MPU_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_arm_v8_MPU_Module__id ti_sysbios_family_arm_v8_MPU_Module_id(void);
static inline CT__ti_sysbios_family_arm_v8_MPU_Module__id ti_sysbios_family_arm_v8_MPU_Module_id( void ) 
{
    return ti_sysbios_family_arm_v8_MPU_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_arm_v8_MPU_Module_hasMask(void);
static inline xdc_Bool ti_sysbios_family_arm_v8_MPU_Module_hasMask(void)
{
    return (xdc_Bool)(ti_sysbios_family_arm_v8_MPU_Module__diagsMask__C != (CT__ti_sysbios_family_arm_v8_MPU_Module__diagsMask)NULL);
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_arm_v8_MPU_Module_getMask(void);
static inline xdc_Bits16 ti_sysbios_family_arm_v8_MPU_Module_getMask(void)
{
    return (ti_sysbios_family_arm_v8_MPU_Module__diagsMask__C != (CT__ti_sysbios_family_arm_v8_MPU_Module__diagsMask)NULL) ? *ti_sysbios_family_arm_v8_MPU_Module__diagsMask__C : (xdc_Bits16)0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_arm_v8_MPU_Module_setMask(xdc_Bits16 mask);
static inline xdc_Void ti_sysbios_family_arm_v8_MPU_Module_setMask(xdc_Bits16 mask)
{
    if (ti_sysbios_family_arm_v8_MPU_Module__diagsMask__C != (CT__ti_sysbios_family_arm_v8_MPU_Module__diagsMask)NULL) {
        *ti_sysbios_family_arm_v8_MPU_Module__diagsMask__C = mask;
    }
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_family_arm_v8_MPU__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_arm_v8_MPU__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_arm_v8_MPU__internalaccess))

#ifndef ti_sysbios_family_arm_v8_MPU__include_state
#define ti_sysbios_family_arm_v8_MPU__include_state

/* Module_State */
struct ti_sysbios_family_arm_v8_MPU_Module_State {
    __TA_ti_sysbios_family_arm_v8_MPU_Module_State__regionEntry regionEntry;
};

/* Module__state__V */
#ifndef ti_sysbios_family_arm_v8_MPU_Module__state__VR
extern struct ti_sysbios_family_arm_v8_MPU_Module_State__ ti_sysbios_family_arm_v8_MPU_Module__state__V;
#else
#define ti_sysbios_family_arm_v8_MPU_Module__state__V (*((struct ti_sysbios_family_arm_v8_MPU_Module_State__*)(xdcRomStatePtr + ti_sysbios_family_arm_v8_MPU_Module__state__V_offset)))
#endif

#endif /* ti_sysbios_family_arm_v8_MPU__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_arm_v8_MPU__nolocalnames)

#ifndef ti_sysbios_family_arm_v8_MPU__localnames__done
#define ti_sysbios_family_arm_v8_MPU__localnames__done

/* module prefix */
#define MPU_DeviceRegs ti_sysbios_family_arm_v8_MPU_DeviceRegs
#define MPU_deviceRegs ti_sysbios_family_arm_v8_MPU_deviceRegs
#define MPU_AccessPerm ti_sysbios_family_arm_v8_MPU_AccessPerm
#define MPU_Shareable ti_sysbios_family_arm_v8_MPU_Shareable
#define MPU_RegionAttrs ti_sysbios_family_arm_v8_MPU_RegionAttrs
#define MPU_RegionEntry ti_sysbios_family_arm_v8_MPU_RegionEntry
#define MPU_Module_State ti_sysbios_family_arm_v8_MPU_Module_State
#define MPU_AccessPerm_RW_PRIV ti_sysbios_family_arm_v8_MPU_AccessPerm_RW_PRIV
#define MPU_AccessPerm_RW_ANY ti_sysbios_family_arm_v8_MPU_AccessPerm_RW_ANY
#define MPU_AccessPerm_RO_PRIV ti_sysbios_family_arm_v8_MPU_AccessPerm_RO_PRIV
#define MPU_AccessPerm_RO_ANY ti_sysbios_family_arm_v8_MPU_AccessPerm_RO_ANY
#define MPU_Shareable_NONE ti_sysbios_family_arm_v8_MPU_Shareable_NONE
#define MPU_Shareable_OUTER ti_sysbios_family_arm_v8_MPU_Shareable_OUTER
#define MPU_Shareable_INNER ti_sysbios_family_arm_v8_MPU_Shareable_INNER
#define MPU_defaultAttrs ti_sysbios_family_arm_v8_MPU_defaultAttrs
#define MPU_A_nullPointer ti_sysbios_family_arm_v8_MPU_A_nullPointer
#define MPU_A_invalidRegionId ti_sysbios_family_arm_v8_MPU_A_invalidRegionId
#define MPU_A_unalignedBaseAddr ti_sysbios_family_arm_v8_MPU_A_unalignedBaseAddr
#define MPU_A_unalignedEndAddr ti_sysbios_family_arm_v8_MPU_A_unalignedEndAddr
#define MPU_enableMPU ti_sysbios_family_arm_v8_MPU_enableMPU
#define MPU_enableBackgroundRegion ti_sysbios_family_arm_v8_MPU_enableBackgroundRegion
#define MPU_MAIR0 ti_sysbios_family_arm_v8_MPU_MAIR0
#define MPU_MAIR1 ti_sysbios_family_arm_v8_MPU_MAIR1
#define MPU_MAIR2 ti_sysbios_family_arm_v8_MPU_MAIR2
#define MPU_MAIR3 ti_sysbios_family_arm_v8_MPU_MAIR3
#define MPU_MAIR4 ti_sysbios_family_arm_v8_MPU_MAIR4
#define MPU_MAIR5 ti_sysbios_family_arm_v8_MPU_MAIR5
#define MPU_MAIR6 ti_sysbios_family_arm_v8_MPU_MAIR6
#define MPU_MAIR7 ti_sysbios_family_arm_v8_MPU_MAIR7
#define MPU_numRegions ti_sysbios_family_arm_v8_MPU_numRegions
#define MPU_regionEntry ti_sysbios_family_arm_v8_MPU_regionEntry
#define MPU_disable ti_sysbios_family_arm_v8_MPU_disable
#define MPU_enable ti_sysbios_family_arm_v8_MPU_enable
#define MPU_initRegionAttrs ti_sysbios_family_arm_v8_MPU_initRegionAttrs
#define MPU_isEnabled ti_sysbios_family_arm_v8_MPU_isEnabled
#define MPU_setMAIR ti_sysbios_family_arm_v8_MPU_setMAIR
#define MPU_setRegion ti_sysbios_family_arm_v8_MPU_setRegion
#define MPU_Module_name ti_sysbios_family_arm_v8_MPU_Module_name
#define MPU_Module_id ti_sysbios_family_arm_v8_MPU_Module_id
#define MPU_Module_startup ti_sysbios_family_arm_v8_MPU_Module_startup
#define MPU_Module_startupDone ti_sysbios_family_arm_v8_MPU_Module_startupDone
#define MPU_Module_hasMask ti_sysbios_family_arm_v8_MPU_Module_hasMask
#define MPU_Module_getMask ti_sysbios_family_arm_v8_MPU_Module_getMask
#define MPU_Module_setMask ti_sysbios_family_arm_v8_MPU_Module_setMask
#define MPU_Object_heap ti_sysbios_family_arm_v8_MPU_Object_heap
#define MPU_Module_heap ti_sysbios_family_arm_v8_MPU_Module_heap

#endif /* ti_sysbios_family_arm_v8_MPU__localnames__done */
#endif
