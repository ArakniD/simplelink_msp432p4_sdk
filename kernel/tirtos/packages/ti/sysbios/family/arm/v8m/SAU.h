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

#ifndef ti_sysbios_family_arm_v8m_SAU__include
#define ti_sysbios_family_arm_v8m_SAU__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_arm_v8m_SAU__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_arm_v8m_SAU___VERS 200


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/arm/v8m/package/package.defs.h>

#include <xdc/runtime/IModule.h>
#include <xdc/runtime/Assert.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* DeviceRegs */
struct ti_sysbios_family_arm_v8m_SAU_DeviceRegs {
    xdc_UInt32 CTRL;
    xdc_UInt32 TYPE;
    xdc_UInt32 RNR;
    xdc_UInt32 RBAR;
    xdc_UInt32 RLAR;
    xdc_UInt32 SFSR;
    xdc_UInt32 SFAR;
};

/* deviceRegs */
__extern volatile ti_sysbios_family_arm_v8m_SAU_DeviceRegs ti_sysbios_family_arm_v8m_SAU_deviceRegs;

/* RegionType */
enum ti_sysbios_family_arm_v8m_SAU_RegionType {
    ti_sysbios_family_arm_v8m_SAU_RegionType_NS,
    ti_sysbios_family_arm_v8m_SAU_RegionType_NSC
};
typedef enum ti_sysbios_family_arm_v8m_SAU_RegionType ti_sysbios_family_arm_v8m_SAU_RegionType;


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* RegionEntry */
struct ti_sysbios_family_arm_v8m_SAU_RegionEntry {
    xdc_UInt32 rbar;
    xdc_UInt32 rlar;
};

/* Module_State */
typedef volatile ti_sysbios_family_arm_v8m_SAU_RegionEntry __T1_ti_sysbios_family_arm_v8m_SAU_Module_State__regionEntry;
typedef volatile ti_sysbios_family_arm_v8m_SAU_RegionEntry *ARRAY1_ti_sysbios_family_arm_v8m_SAU_Module_State__regionEntry;
typedef const volatile ti_sysbios_family_arm_v8m_SAU_RegionEntry *CARRAY1_ti_sysbios_family_arm_v8m_SAU_Module_State__regionEntry;
typedef ARRAY1_ti_sysbios_family_arm_v8m_SAU_Module_State__regionEntry __TA_ti_sysbios_family_arm_v8m_SAU_Module_State__regionEntry;


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_arm_v8m_SAU_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Module__diagsEnabled ti_sysbios_family_arm_v8m_SAU_Module__diagsEnabled__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Module__diagsEnabled__CR
#define ti_sysbios_family_arm_v8m_SAU_Module__diagsEnabled__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Module__diagsEnabled*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Module__diagsEnabled__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__diagsEnabled (ti_sysbios_family_arm_v8m_SAU_Module__diagsEnabled__C)
#endif

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_arm_v8m_SAU_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Module__diagsIncluded ti_sysbios_family_arm_v8m_SAU_Module__diagsIncluded__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Module__diagsIncluded__CR
#define ti_sysbios_family_arm_v8m_SAU_Module__diagsIncluded__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Module__diagsIncluded*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Module__diagsIncluded__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__diagsIncluded (ti_sysbios_family_arm_v8m_SAU_Module__diagsIncluded__C)
#endif

/* Module__diagsMask */
typedef xdc_Bits16 *CT__ti_sysbios_family_arm_v8m_SAU_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Module__diagsMask ti_sysbios_family_arm_v8m_SAU_Module__diagsMask__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Module__diagsMask__CR
#define ti_sysbios_family_arm_v8m_SAU_Module__diagsMask__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Module__diagsMask*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Module__diagsMask__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__diagsMask (ti_sysbios_family_arm_v8m_SAU_Module__diagsMask__C)
#endif

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_arm_v8m_SAU_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Module__gateObj ti_sysbios_family_arm_v8m_SAU_Module__gateObj__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Module__gateObj__CR
#define ti_sysbios_family_arm_v8m_SAU_Module__gateObj__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Module__gateObj*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Module__gateObj__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__gateObj (ti_sysbios_family_arm_v8m_SAU_Module__gateObj__C)
#endif

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_arm_v8m_SAU_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Module__gatePrms ti_sysbios_family_arm_v8m_SAU_Module__gatePrms__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Module__gatePrms__CR
#define ti_sysbios_family_arm_v8m_SAU_Module__gatePrms__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Module__gatePrms*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Module__gatePrms__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__gatePrms (ti_sysbios_family_arm_v8m_SAU_Module__gatePrms__C)
#endif

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_arm_v8m_SAU_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Module__id ti_sysbios_family_arm_v8m_SAU_Module__id__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Module__id__CR
#define ti_sysbios_family_arm_v8m_SAU_Module__id__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Module__id*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Module__id__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__id (ti_sysbios_family_arm_v8m_SAU_Module__id__C)
#endif

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerDefined ti_sysbios_family_arm_v8m_SAU_Module__loggerDefined__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Module__loggerDefined__CR
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerDefined__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerDefined*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Module__loggerDefined__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerDefined (ti_sysbios_family_arm_v8m_SAU_Module__loggerDefined__C)
#endif

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerObj ti_sysbios_family_arm_v8m_SAU_Module__loggerObj__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Module__loggerObj__CR
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerObj__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerObj*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Module__loggerObj__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerObj (ti_sysbios_family_arm_v8m_SAU_Module__loggerObj__C)
#endif

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn0 ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn0__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn0__CR
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn0__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn0*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn0__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn0 (ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn0__C)
#endif

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn1 ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn1__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn1__CR
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn1__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn1*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn1__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn1 (ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn1__C)
#endif

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn2 ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn2__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn2__CR
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn2__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn2*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn2__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn2 (ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn2__C)
#endif

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn4 ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn4__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn4__CR
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn4__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn4*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn4__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn4 (ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn4__C)
#endif

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn8 ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn8__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn8__CR
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn8__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn8*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn8__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn8 (ti_sysbios_family_arm_v8m_SAU_Module__loggerFxn8__C)
#endif

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_arm_v8m_SAU_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Object__count ti_sysbios_family_arm_v8m_SAU_Object__count__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Object__count__CR
#define ti_sysbios_family_arm_v8m_SAU_Object__count__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Object__count*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Object__count__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Object__count (ti_sysbios_family_arm_v8m_SAU_Object__count__C)
#endif

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_arm_v8m_SAU_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Object__heap ti_sysbios_family_arm_v8m_SAU_Object__heap__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Object__heap__CR
#define ti_sysbios_family_arm_v8m_SAU_Object__heap__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Object__heap*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Object__heap__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Object__heap (ti_sysbios_family_arm_v8m_SAU_Object__heap__C)
#endif

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_arm_v8m_SAU_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Object__sizeof ti_sysbios_family_arm_v8m_SAU_Object__sizeof__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Object__sizeof__CR
#define ti_sysbios_family_arm_v8m_SAU_Object__sizeof__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Object__sizeof*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Object__sizeof__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Object__sizeof (ti_sysbios_family_arm_v8m_SAU_Object__sizeof__C)
#endif

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_arm_v8m_SAU_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_Object__table ti_sysbios_family_arm_v8m_SAU_Object__table__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_Object__table__CR
#define ti_sysbios_family_arm_v8m_SAU_Object__table__C (*((CT__ti_sysbios_family_arm_v8m_SAU_Object__table*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_Object__table__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_Object__table (ti_sysbios_family_arm_v8m_SAU_Object__table__C)
#endif

/* A_nullPointer */
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_arm_v8m_SAU_A_nullPointer;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_A_nullPointer ti_sysbios_family_arm_v8m_SAU_A_nullPointer__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_A_nullPointer__CR
#define ti_sysbios_family_arm_v8m_SAU_A_nullPointer (*((CT__ti_sysbios_family_arm_v8m_SAU_A_nullPointer*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_A_nullPointer__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_A_nullPointer (ti_sysbios_family_arm_v8m_SAU_A_nullPointer__C)
#endif

/* A_invalidRegionId */
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_arm_v8m_SAU_A_invalidRegionId;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_A_invalidRegionId ti_sysbios_family_arm_v8m_SAU_A_invalidRegionId__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_A_invalidRegionId__CR
#define ti_sysbios_family_arm_v8m_SAU_A_invalidRegionId (*((CT__ti_sysbios_family_arm_v8m_SAU_A_invalidRegionId*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_A_invalidRegionId__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_A_invalidRegionId (ti_sysbios_family_arm_v8m_SAU_A_invalidRegionId__C)
#endif

/* A_unalignedBaseAddr */
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_arm_v8m_SAU_A_unalignedBaseAddr;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_A_unalignedBaseAddr ti_sysbios_family_arm_v8m_SAU_A_unalignedBaseAddr__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_A_unalignedBaseAddr__CR
#define ti_sysbios_family_arm_v8m_SAU_A_unalignedBaseAddr (*((CT__ti_sysbios_family_arm_v8m_SAU_A_unalignedBaseAddr*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_A_unalignedBaseAddr__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_A_unalignedBaseAddr (ti_sysbios_family_arm_v8m_SAU_A_unalignedBaseAddr__C)
#endif

/* A_unalignedEndAddr */
typedef xdc_runtime_Assert_Id CT__ti_sysbios_family_arm_v8m_SAU_A_unalignedEndAddr;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_A_unalignedEndAddr ti_sysbios_family_arm_v8m_SAU_A_unalignedEndAddr__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_A_unalignedEndAddr__CR
#define ti_sysbios_family_arm_v8m_SAU_A_unalignedEndAddr (*((CT__ti_sysbios_family_arm_v8m_SAU_A_unalignedEndAddr*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_A_unalignedEndAddr__C_offset)))
#else
#define ti_sysbios_family_arm_v8m_SAU_A_unalignedEndAddr (ti_sysbios_family_arm_v8m_SAU_A_unalignedEndAddr__C)
#endif

/* enableSAU */
typedef xdc_Bool CT__ti_sysbios_family_arm_v8m_SAU_enableSAU;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_enableSAU ti_sysbios_family_arm_v8m_SAU_enableSAU__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_enableSAU__CR
#define ti_sysbios_family_arm_v8m_SAU_enableSAU (*((CT__ti_sysbios_family_arm_v8m_SAU_enableSAU*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_enableSAU__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8m_SAU_enableSAU__D
#define ti_sysbios_family_arm_v8m_SAU_enableSAU (ti_sysbios_family_arm_v8m_SAU_enableSAU__D)
#else
#define ti_sysbios_family_arm_v8m_SAU_enableSAU (ti_sysbios_family_arm_v8m_SAU_enableSAU__C)
#endif
#endif

/* allSecureWhenSauDisabled */
typedef xdc_Bool CT__ti_sysbios_family_arm_v8m_SAU_allSecureWhenSauDisabled;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_allSecureWhenSauDisabled ti_sysbios_family_arm_v8m_SAU_allSecureWhenSauDisabled__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_allSecureWhenSauDisabled__CR
#define ti_sysbios_family_arm_v8m_SAU_allSecureWhenSauDisabled (*((CT__ti_sysbios_family_arm_v8m_SAU_allSecureWhenSauDisabled*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_allSecureWhenSauDisabled__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8m_SAU_allSecureWhenSauDisabled__D
#define ti_sysbios_family_arm_v8m_SAU_allSecureWhenSauDisabled (ti_sysbios_family_arm_v8m_SAU_allSecureWhenSauDisabled__D)
#else
#define ti_sysbios_family_arm_v8m_SAU_allSecureWhenSauDisabled (ti_sysbios_family_arm_v8m_SAU_allSecureWhenSauDisabled__C)
#endif
#endif

/* numRegions */
typedef xdc_UInt8 CT__ti_sysbios_family_arm_v8m_SAU_numRegions;
__extern __FAR__ const CT__ti_sysbios_family_arm_v8m_SAU_numRegions ti_sysbios_family_arm_v8m_SAU_numRegions__C;
#ifdef ti_sysbios_family_arm_v8m_SAU_numRegions__CR
#define ti_sysbios_family_arm_v8m_SAU_numRegions (*((CT__ti_sysbios_family_arm_v8m_SAU_numRegions*)(xdcRomConstPtr + ti_sysbios_family_arm_v8m_SAU_numRegions__C_offset)))
#else
#ifdef ti_sysbios_family_arm_v8m_SAU_numRegions__D
#define ti_sysbios_family_arm_v8m_SAU_numRegions (ti_sysbios_family_arm_v8m_SAU_numRegions__D)
#else
#define ti_sysbios_family_arm_v8m_SAU_numRegions (ti_sysbios_family_arm_v8m_SAU_numRegions__C)
#endif
#endif


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_arm_v8m_SAU_Module_startup( state ) (-1)

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_arm_v8m_SAU_Module__startupDone__S, "ti_sysbios_family_arm_v8m_SAU_Module__startupDone__S")
__extern xdc_Bool ti_sysbios_family_arm_v8m_SAU_Module__startupDone__S( void );

/* disable__E */
#define ti_sysbios_family_arm_v8m_SAU_disable ti_sysbios_family_arm_v8m_SAU_disable__E
xdc__CODESECT(ti_sysbios_family_arm_v8m_SAU_disable__E, "ti_sysbios_family_arm_v8m_SAU_disable")
__extern xdc_Void ti_sysbios_family_arm_v8m_SAU_disable__E( void );

/* enable__E */
#define ti_sysbios_family_arm_v8m_SAU_enable ti_sysbios_family_arm_v8m_SAU_enable__E
xdc__CODESECT(ti_sysbios_family_arm_v8m_SAU_enable__E, "ti_sysbios_family_arm_v8m_SAU_enable")
__extern xdc_Void ti_sysbios_family_arm_v8m_SAU_enable__E( void );

/* isEnabled__E */
#define ti_sysbios_family_arm_v8m_SAU_isEnabled ti_sysbios_family_arm_v8m_SAU_isEnabled__E
xdc__CODESECT(ti_sysbios_family_arm_v8m_SAU_isEnabled__E, "ti_sysbios_family_arm_v8m_SAU_isEnabled")
__extern xdc_Bool ti_sysbios_family_arm_v8m_SAU_isEnabled__E( void );

/* configureRegion__E */
#define ti_sysbios_family_arm_v8m_SAU_configureRegion ti_sysbios_family_arm_v8m_SAU_configureRegion__E
xdc__CODESECT(ti_sysbios_family_arm_v8m_SAU_configureRegion__E, "ti_sysbios_family_arm_v8m_SAU_configureRegion")
__extern xdc_Void ti_sysbios_family_arm_v8m_SAU_configureRegion__E( xdc_UInt8 regionId, xdc_Ptr regionBase, xdc_SizeT regionSize, ti_sysbios_family_arm_v8m_SAU_RegionType regionTupe );

/* enableRegion__E */
#define ti_sysbios_family_arm_v8m_SAU_enableRegion ti_sysbios_family_arm_v8m_SAU_enableRegion__E
xdc__CODESECT(ti_sysbios_family_arm_v8m_SAU_enableRegion__E, "ti_sysbios_family_arm_v8m_SAU_enableRegion")
__extern xdc_Void ti_sysbios_family_arm_v8m_SAU_enableRegion__E( xdc_UInt8 regionId );

/* disableRegion__E */
#define ti_sysbios_family_arm_v8m_SAU_disableRegion ti_sysbios_family_arm_v8m_SAU_disableRegion__E
xdc__CODESECT(ti_sysbios_family_arm_v8m_SAU_disableRegion__E, "ti_sysbios_family_arm_v8m_SAU_disableRegion")
__extern xdc_Void ti_sysbios_family_arm_v8m_SAU_disableRegion__E( xdc_UInt8 regionId );

/* startup__I */
#define ti_sysbios_family_arm_v8m_SAU_startup ti_sysbios_family_arm_v8m_SAU_startup__I
xdc__CODESECT(ti_sysbios_family_arm_v8m_SAU_startup__I, "ti_sysbios_family_arm_v8m_SAU_startup")
__extern xdc_Void ti_sysbios_family_arm_v8m_SAU_startup__I( void );


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_arm_v8m_SAU_Module_startupDone() ti_sysbios_family_arm_v8m_SAU_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_arm_v8m_SAU_Object_heap() ti_sysbios_family_arm_v8m_SAU_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_arm_v8m_SAU_Module_heap() ti_sysbios_family_arm_v8m_SAU_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_arm_v8m_SAU_Module__id ti_sysbios_family_arm_v8m_SAU_Module_id(void);
static inline CT__ti_sysbios_family_arm_v8m_SAU_Module__id ti_sysbios_family_arm_v8m_SAU_Module_id( void ) 
{
    return ti_sysbios_family_arm_v8m_SAU_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_arm_v8m_SAU_Module_hasMask(void);
static inline xdc_Bool ti_sysbios_family_arm_v8m_SAU_Module_hasMask(void)
{
    return (xdc_Bool)(ti_sysbios_family_arm_v8m_SAU_Module__diagsMask__C != (CT__ti_sysbios_family_arm_v8m_SAU_Module__diagsMask)NULL);
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_arm_v8m_SAU_Module_getMask(void);
static inline xdc_Bits16 ti_sysbios_family_arm_v8m_SAU_Module_getMask(void)
{
    return (ti_sysbios_family_arm_v8m_SAU_Module__diagsMask__C != (CT__ti_sysbios_family_arm_v8m_SAU_Module__diagsMask)NULL) ? *ti_sysbios_family_arm_v8m_SAU_Module__diagsMask__C : (xdc_Bits16)0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_arm_v8m_SAU_Module_setMask(xdc_Bits16 mask);
static inline xdc_Void ti_sysbios_family_arm_v8m_SAU_Module_setMask(xdc_Bits16 mask)
{
    if (ti_sysbios_family_arm_v8m_SAU_Module__diagsMask__C != (CT__ti_sysbios_family_arm_v8m_SAU_Module__diagsMask)NULL) {
        *ti_sysbios_family_arm_v8m_SAU_Module__diagsMask__C = mask;
    }
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_family_arm_v8m_SAU__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_arm_v8m_SAU__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_arm_v8m_SAU__internalaccess))

#ifndef ti_sysbios_family_arm_v8m_SAU__include_state
#define ti_sysbios_family_arm_v8m_SAU__include_state

/* Module_State */
struct ti_sysbios_family_arm_v8m_SAU_Module_State {
    __TA_ti_sysbios_family_arm_v8m_SAU_Module_State__regionEntry regionEntry;
};

/* Module__state__V */
#ifndef ti_sysbios_family_arm_v8m_SAU_Module__state__VR
extern struct ti_sysbios_family_arm_v8m_SAU_Module_State__ ti_sysbios_family_arm_v8m_SAU_Module__state__V;
#else
#define ti_sysbios_family_arm_v8m_SAU_Module__state__V (*((struct ti_sysbios_family_arm_v8m_SAU_Module_State__*)(xdcRomStatePtr + ti_sysbios_family_arm_v8m_SAU_Module__state__V_offset)))
#endif

#endif /* ti_sysbios_family_arm_v8m_SAU__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_arm_v8m_SAU__nolocalnames)

#ifndef ti_sysbios_family_arm_v8m_SAU__localnames__done
#define ti_sysbios_family_arm_v8m_SAU__localnames__done

/* module prefix */
#define SAU_DeviceRegs ti_sysbios_family_arm_v8m_SAU_DeviceRegs
#define SAU_deviceRegs ti_sysbios_family_arm_v8m_SAU_deviceRegs
#define SAU_RegionType ti_sysbios_family_arm_v8m_SAU_RegionType
#define SAU_RegionEntry ti_sysbios_family_arm_v8m_SAU_RegionEntry
#define SAU_Module_State ti_sysbios_family_arm_v8m_SAU_Module_State
#define SAU_RegionType_NS ti_sysbios_family_arm_v8m_SAU_RegionType_NS
#define SAU_RegionType_NSC ti_sysbios_family_arm_v8m_SAU_RegionType_NSC
#define SAU_A_nullPointer ti_sysbios_family_arm_v8m_SAU_A_nullPointer
#define SAU_A_invalidRegionId ti_sysbios_family_arm_v8m_SAU_A_invalidRegionId
#define SAU_A_unalignedBaseAddr ti_sysbios_family_arm_v8m_SAU_A_unalignedBaseAddr
#define SAU_A_unalignedEndAddr ti_sysbios_family_arm_v8m_SAU_A_unalignedEndAddr
#define SAU_enableSAU ti_sysbios_family_arm_v8m_SAU_enableSAU
#define SAU_allSecureWhenSauDisabled ti_sysbios_family_arm_v8m_SAU_allSecureWhenSauDisabled
#define SAU_numRegions ti_sysbios_family_arm_v8m_SAU_numRegions
#define SAU_disable ti_sysbios_family_arm_v8m_SAU_disable
#define SAU_enable ti_sysbios_family_arm_v8m_SAU_enable
#define SAU_isEnabled ti_sysbios_family_arm_v8m_SAU_isEnabled
#define SAU_configureRegion ti_sysbios_family_arm_v8m_SAU_configureRegion
#define SAU_enableRegion ti_sysbios_family_arm_v8m_SAU_enableRegion
#define SAU_disableRegion ti_sysbios_family_arm_v8m_SAU_disableRegion
#define SAU_Module_name ti_sysbios_family_arm_v8m_SAU_Module_name
#define SAU_Module_id ti_sysbios_family_arm_v8m_SAU_Module_id
#define SAU_Module_startup ti_sysbios_family_arm_v8m_SAU_Module_startup
#define SAU_Module_startupDone ti_sysbios_family_arm_v8m_SAU_Module_startupDone
#define SAU_Module_hasMask ti_sysbios_family_arm_v8m_SAU_Module_hasMask
#define SAU_Module_getMask ti_sysbios_family_arm_v8m_SAU_Module_getMask
#define SAU_Module_setMask ti_sysbios_family_arm_v8m_SAU_Module_setMask
#define SAU_Object_heap ti_sysbios_family_arm_v8m_SAU_Object_heap
#define SAU_Module_heap ti_sysbios_family_arm_v8m_SAU_Module_heap

#endif /* ti_sysbios_family_arm_v8m_SAU__localnames__done */
#endif
