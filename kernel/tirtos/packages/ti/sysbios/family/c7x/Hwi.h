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
 *     CREATE ARGS
 *     INTERNAL DEFINITIONS
 *     MODULE-WIDE CONFIGS
 *     PER-INSTANCE TYPES
 *     VIRTUAL FUNCTIONS
 *     FUNCTION DECLARATIONS
 *     CONVERTORS
 *     SYSTEM FUNCTIONS
 *
 *     EPILOGUE
 *     STATE STRUCTURES
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef ti_sysbios_family_c7x_Hwi__include
#define ti_sysbios_family_c7x_Hwi__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_c7x_Hwi__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_c7x_Hwi___VERS 200


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/IInstance.h>
#include <ti/sysbios/family/c7x/Hwi__prologue.h>
#include <ti/sysbios/family/c7x/package/package.defs.h>

#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <ti/sysbios/interfaces/IHwi.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* FuncPtr */
typedef ti_sysbios_interfaces_IHwi_FuncPtr ti_sysbios_family_c7x_Hwi_FuncPtr;

/* Irp */
typedef ti_sysbios_interfaces_IHwi_Irp ti_sysbios_family_c7x_Hwi_Irp;

/* HookSet */
typedef ti_sysbios_interfaces_IHwi_HookSet ti_sysbios_family_c7x_Hwi_HookSet;

/* MaskingOption */
typedef ti_sysbios_interfaces_IHwi_MaskingOption ti_sysbios_family_c7x_Hwi_MaskingOption;

/* StackInfo */
typedef ti_sysbios_interfaces_IHwi_StackInfo ti_sysbios_family_c7x_Hwi_StackInfo;

/* NUM_INTERRUPTS */
#define ti_sysbios_family_c7x_Hwi_NUM_INTERRUPTS (64)

/* PlugFuncPtr */
typedef xdc_Void (*ti_sysbios_family_c7x_Hwi_PlugFuncPtr)(xdc_Void );

/* MaskingOption_NONE */
#define ti_sysbios_family_c7x_Hwi_MaskingOption_NONE ti_sysbios_interfaces_IHwi_MaskingOption_NONE

/* MaskingOption_ALL */
#define ti_sysbios_family_c7x_Hwi_MaskingOption_ALL ti_sysbios_interfaces_IHwi_MaskingOption_ALL

/* MaskingOption_SELF */
#define ti_sysbios_family_c7x_Hwi_MaskingOption_SELF ti_sysbios_interfaces_IHwi_MaskingOption_SELF

/* MaskingOption_BITMASK */
#define ti_sysbios_family_c7x_Hwi_MaskingOption_BITMASK ti_sysbios_interfaces_IHwi_MaskingOption_BITMASK

/* MaskingOption_LOWER */
#define ti_sysbios_family_c7x_Hwi_MaskingOption_LOWER ti_sysbios_interfaces_IHwi_MaskingOption_LOWER


/*
 * ======== CREATE ARGS ========
 */

/* Args__create */
typedef struct ti_sysbios_family_c7x_Hwi_Args__create {
    xdc_Int intNum;
    ti_sysbios_interfaces_IHwi_FuncPtr hwiFxn;
} ti_sysbios_family_c7x_Hwi_Args__create;


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* Instance_State */
typedef xdc_Ptr __T1_ti_sysbios_family_c7x_Hwi_Instance_State__hookEnv;
typedef xdc_Ptr *ARRAY1_ti_sysbios_family_c7x_Hwi_Instance_State__hookEnv;
typedef const xdc_Ptr *CARRAY1_ti_sysbios_family_c7x_Hwi_Instance_State__hookEnv;
typedef ARRAY1_ti_sysbios_family_c7x_Hwi_Instance_State__hookEnv __TA_ti_sysbios_family_c7x_Hwi_Instance_State__hookEnv;

/* Module_State */
typedef xdc_Char __T1_ti_sysbios_family_c7x_Hwi_Module_State__intEvents;
typedef xdc_Char ARRAY1_ti_sysbios_family_c7x_Hwi_Module_State__intEvents[64];
typedef xdc_Char CARRAY1_ti_sysbios_family_c7x_Hwi_Module_State__intEvents[64];
typedef ARRAY1_ti_sysbios_family_c7x_Hwi_Module_State__intEvents __TA_ti_sysbios_family_c7x_Hwi_Module_State__intEvents;
typedef ti_sysbios_family_c7x_Hwi_Handle __T1_ti_sysbios_family_c7x_Hwi_Module_State__dispatchTable;
typedef ti_sysbios_family_c7x_Hwi_Handle ARRAY1_ti_sysbios_family_c7x_Hwi_Module_State__dispatchTable[64];
typedef ti_sysbios_family_c7x_Hwi_Handle CARRAY1_ti_sysbios_family_c7x_Hwi_Module_State__dispatchTable[64];
typedef ARRAY1_ti_sysbios_family_c7x_Hwi_Module_State__dispatchTable __TA_ti_sysbios_family_c7x_Hwi_Module_State__dispatchTable;


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_c7x_Hwi_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Module__diagsEnabled ti_sysbios_family_c7x_Hwi_Module__diagsEnabled__C;
#ifdef ti_sysbios_family_c7x_Hwi_Module__diagsEnabled__CR
#define ti_sysbios_family_c7x_Hwi_Module__diagsEnabled__C (*((CT__ti_sysbios_family_c7x_Hwi_Module__diagsEnabled*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__diagsEnabled__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Module__diagsEnabled (ti_sysbios_family_c7x_Hwi_Module__diagsEnabled__C)
#endif

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_c7x_Hwi_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Module__diagsIncluded ti_sysbios_family_c7x_Hwi_Module__diagsIncluded__C;
#ifdef ti_sysbios_family_c7x_Hwi_Module__diagsIncluded__CR
#define ti_sysbios_family_c7x_Hwi_Module__diagsIncluded__C (*((CT__ti_sysbios_family_c7x_Hwi_Module__diagsIncluded*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__diagsIncluded__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Module__diagsIncluded (ti_sysbios_family_c7x_Hwi_Module__diagsIncluded__C)
#endif

/* Module__diagsMask */
typedef xdc_Bits16 *CT__ti_sysbios_family_c7x_Hwi_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Module__diagsMask ti_sysbios_family_c7x_Hwi_Module__diagsMask__C;
#ifdef ti_sysbios_family_c7x_Hwi_Module__diagsMask__CR
#define ti_sysbios_family_c7x_Hwi_Module__diagsMask__C (*((CT__ti_sysbios_family_c7x_Hwi_Module__diagsMask*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__diagsMask__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Module__diagsMask (ti_sysbios_family_c7x_Hwi_Module__diagsMask__C)
#endif

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_c7x_Hwi_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Module__gateObj ti_sysbios_family_c7x_Hwi_Module__gateObj__C;
#ifdef ti_sysbios_family_c7x_Hwi_Module__gateObj__CR
#define ti_sysbios_family_c7x_Hwi_Module__gateObj__C (*((CT__ti_sysbios_family_c7x_Hwi_Module__gateObj*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__gateObj__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Module__gateObj (ti_sysbios_family_c7x_Hwi_Module__gateObj__C)
#endif

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_c7x_Hwi_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Module__gatePrms ti_sysbios_family_c7x_Hwi_Module__gatePrms__C;
#ifdef ti_sysbios_family_c7x_Hwi_Module__gatePrms__CR
#define ti_sysbios_family_c7x_Hwi_Module__gatePrms__C (*((CT__ti_sysbios_family_c7x_Hwi_Module__gatePrms*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__gatePrms__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Module__gatePrms (ti_sysbios_family_c7x_Hwi_Module__gatePrms__C)
#endif

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_c7x_Hwi_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Module__id ti_sysbios_family_c7x_Hwi_Module__id__C;
#ifdef ti_sysbios_family_c7x_Hwi_Module__id__CR
#define ti_sysbios_family_c7x_Hwi_Module__id__C (*((CT__ti_sysbios_family_c7x_Hwi_Module__id*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__id__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Module__id (ti_sysbios_family_c7x_Hwi_Module__id__C)
#endif

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_c7x_Hwi_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Module__loggerDefined ti_sysbios_family_c7x_Hwi_Module__loggerDefined__C;
#ifdef ti_sysbios_family_c7x_Hwi_Module__loggerDefined__CR
#define ti_sysbios_family_c7x_Hwi_Module__loggerDefined__C (*((CT__ti_sysbios_family_c7x_Hwi_Module__loggerDefined*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__loggerDefined__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Module__loggerDefined (ti_sysbios_family_c7x_Hwi_Module__loggerDefined__C)
#endif

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_c7x_Hwi_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Module__loggerObj ti_sysbios_family_c7x_Hwi_Module__loggerObj__C;
#ifdef ti_sysbios_family_c7x_Hwi_Module__loggerObj__CR
#define ti_sysbios_family_c7x_Hwi_Module__loggerObj__C (*((CT__ti_sysbios_family_c7x_Hwi_Module__loggerObj*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__loggerObj__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Module__loggerObj (ti_sysbios_family_c7x_Hwi_Module__loggerObj__C)
#endif

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn0 ti_sysbios_family_c7x_Hwi_Module__loggerFxn0__C;
#ifdef ti_sysbios_family_c7x_Hwi_Module__loggerFxn0__CR
#define ti_sysbios_family_c7x_Hwi_Module__loggerFxn0__C (*((CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn0*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__loggerFxn0__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Module__loggerFxn0 (ti_sysbios_family_c7x_Hwi_Module__loggerFxn0__C)
#endif

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn1 ti_sysbios_family_c7x_Hwi_Module__loggerFxn1__C;
#ifdef ti_sysbios_family_c7x_Hwi_Module__loggerFxn1__CR
#define ti_sysbios_family_c7x_Hwi_Module__loggerFxn1__C (*((CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn1*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__loggerFxn1__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Module__loggerFxn1 (ti_sysbios_family_c7x_Hwi_Module__loggerFxn1__C)
#endif

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn2 ti_sysbios_family_c7x_Hwi_Module__loggerFxn2__C;
#ifdef ti_sysbios_family_c7x_Hwi_Module__loggerFxn2__CR
#define ti_sysbios_family_c7x_Hwi_Module__loggerFxn2__C (*((CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn2*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__loggerFxn2__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Module__loggerFxn2 (ti_sysbios_family_c7x_Hwi_Module__loggerFxn2__C)
#endif

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn4 ti_sysbios_family_c7x_Hwi_Module__loggerFxn4__C;
#ifdef ti_sysbios_family_c7x_Hwi_Module__loggerFxn4__CR
#define ti_sysbios_family_c7x_Hwi_Module__loggerFxn4__C (*((CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn4*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__loggerFxn4__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Module__loggerFxn4 (ti_sysbios_family_c7x_Hwi_Module__loggerFxn4__C)
#endif

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn8 ti_sysbios_family_c7x_Hwi_Module__loggerFxn8__C;
#ifdef ti_sysbios_family_c7x_Hwi_Module__loggerFxn8__CR
#define ti_sysbios_family_c7x_Hwi_Module__loggerFxn8__C (*((CT__ti_sysbios_family_c7x_Hwi_Module__loggerFxn8*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__loggerFxn8__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Module__loggerFxn8 (ti_sysbios_family_c7x_Hwi_Module__loggerFxn8__C)
#endif

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_c7x_Hwi_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Object__count ti_sysbios_family_c7x_Hwi_Object__count__C;
#ifdef ti_sysbios_family_c7x_Hwi_Object__count__CR
#define ti_sysbios_family_c7x_Hwi_Object__count__C (*((CT__ti_sysbios_family_c7x_Hwi_Object__count*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Object__count__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Object__count (ti_sysbios_family_c7x_Hwi_Object__count__C)
#endif

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_c7x_Hwi_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Object__heap ti_sysbios_family_c7x_Hwi_Object__heap__C;
#ifdef ti_sysbios_family_c7x_Hwi_Object__heap__CR
#define ti_sysbios_family_c7x_Hwi_Object__heap__C (*((CT__ti_sysbios_family_c7x_Hwi_Object__heap*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Object__heap__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Object__heap (ti_sysbios_family_c7x_Hwi_Object__heap__C)
#endif

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_c7x_Hwi_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Object__sizeof ti_sysbios_family_c7x_Hwi_Object__sizeof__C;
#ifdef ti_sysbios_family_c7x_Hwi_Object__sizeof__CR
#define ti_sysbios_family_c7x_Hwi_Object__sizeof__C (*((CT__ti_sysbios_family_c7x_Hwi_Object__sizeof*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Object__sizeof__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Object__sizeof (ti_sysbios_family_c7x_Hwi_Object__sizeof__C)
#endif

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_c7x_Hwi_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_Object__table ti_sysbios_family_c7x_Hwi_Object__table__C;
#ifdef ti_sysbios_family_c7x_Hwi_Object__table__CR
#define ti_sysbios_family_c7x_Hwi_Object__table__C (*((CT__ti_sysbios_family_c7x_Hwi_Object__table*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Object__table__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_Object__table (ti_sysbios_family_c7x_Hwi_Object__table__C)
#endif

/* dispatcherAutoNestingSupport */
typedef xdc_Bool CT__ti_sysbios_family_c7x_Hwi_dispatcherAutoNestingSupport;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_dispatcherAutoNestingSupport ti_sysbios_family_c7x_Hwi_dispatcherAutoNestingSupport__C;
#ifdef ti_sysbios_family_c7x_Hwi_dispatcherAutoNestingSupport__CR
#define ti_sysbios_family_c7x_Hwi_dispatcherAutoNestingSupport (*((CT__ti_sysbios_family_c7x_Hwi_dispatcherAutoNestingSupport*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_dispatcherAutoNestingSupport__C_offset)))
#else
#ifdef ti_sysbios_family_c7x_Hwi_dispatcherAutoNestingSupport__D
#define ti_sysbios_family_c7x_Hwi_dispatcherAutoNestingSupport (ti_sysbios_family_c7x_Hwi_dispatcherAutoNestingSupport__D)
#else
#define ti_sysbios_family_c7x_Hwi_dispatcherAutoNestingSupport (ti_sysbios_family_c7x_Hwi_dispatcherAutoNestingSupport__C)
#endif
#endif

/* dispatcherSwiSupport */
typedef xdc_Bool CT__ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport__C;
#ifdef ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport__CR
#define ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport (*((CT__ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport__C_offset)))
#else
#ifdef ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport__D
#define ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport (ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport__D)
#else
#define ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport (ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport__C)
#endif
#endif

/* dispatcherTaskSupport */
typedef xdc_Bool CT__ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport__C;
#ifdef ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport__CR
#define ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport (*((CT__ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport__C_offset)))
#else
#ifdef ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport__D
#define ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport (ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport__D)
#else
#define ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport (ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport__C)
#endif
#endif

/* dispatcherIrpTrackingSupport */
typedef xdc_Bool CT__ti_sysbios_family_c7x_Hwi_dispatcherIrpTrackingSupport;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_dispatcherIrpTrackingSupport ti_sysbios_family_c7x_Hwi_dispatcherIrpTrackingSupport__C;
#ifdef ti_sysbios_family_c7x_Hwi_dispatcherIrpTrackingSupport__CR
#define ti_sysbios_family_c7x_Hwi_dispatcherIrpTrackingSupport (*((CT__ti_sysbios_family_c7x_Hwi_dispatcherIrpTrackingSupport*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_dispatcherIrpTrackingSupport__C_offset)))
#else
#ifdef ti_sysbios_family_c7x_Hwi_dispatcherIrpTrackingSupport__D
#define ti_sysbios_family_c7x_Hwi_dispatcherIrpTrackingSupport (ti_sysbios_family_c7x_Hwi_dispatcherIrpTrackingSupport__D)
#else
#define ti_sysbios_family_c7x_Hwi_dispatcherIrpTrackingSupport (ti_sysbios_family_c7x_Hwi_dispatcherIrpTrackingSupport__C)
#endif
#endif

/* DEFAULT_INT_PRIORITY */
typedef xdc_UInt CT__ti_sysbios_family_c7x_Hwi_DEFAULT_INT_PRIORITY;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_DEFAULT_INT_PRIORITY ti_sysbios_family_c7x_Hwi_DEFAULT_INT_PRIORITY__C;
#ifdef ti_sysbios_family_c7x_Hwi_DEFAULT_INT_PRIORITY__CR
#define ti_sysbios_family_c7x_Hwi_DEFAULT_INT_PRIORITY (*((CT__ti_sysbios_family_c7x_Hwi_DEFAULT_INT_PRIORITY*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_DEFAULT_INT_PRIORITY__C_offset)))
#else
#ifdef ti_sysbios_family_c7x_Hwi_DEFAULT_INT_PRIORITY__D
#define ti_sysbios_family_c7x_Hwi_DEFAULT_INT_PRIORITY (ti_sysbios_family_c7x_Hwi_DEFAULT_INT_PRIORITY__D)
#else
#define ti_sysbios_family_c7x_Hwi_DEFAULT_INT_PRIORITY (ti_sysbios_family_c7x_Hwi_DEFAULT_INT_PRIORITY__C)
#endif
#endif

/* E_alreadyDefined */
typedef xdc_runtime_Error_Id CT__ti_sysbios_family_c7x_Hwi_E_alreadyDefined;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_E_alreadyDefined ti_sysbios_family_c7x_Hwi_E_alreadyDefined__C;
#ifdef ti_sysbios_family_c7x_Hwi_E_alreadyDefined__CR
#define ti_sysbios_family_c7x_Hwi_E_alreadyDefined (*((CT__ti_sysbios_family_c7x_Hwi_E_alreadyDefined*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_E_alreadyDefined__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_E_alreadyDefined (ti_sysbios_family_c7x_Hwi_E_alreadyDefined__C)
#endif

/* E_handleNotFound */
typedef xdc_runtime_Error_Id CT__ti_sysbios_family_c7x_Hwi_E_handleNotFound;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_E_handleNotFound ti_sysbios_family_c7x_Hwi_E_handleNotFound__C;
#ifdef ti_sysbios_family_c7x_Hwi_E_handleNotFound__CR
#define ti_sysbios_family_c7x_Hwi_E_handleNotFound (*((CT__ti_sysbios_family_c7x_Hwi_E_handleNotFound*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_E_handleNotFound__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_E_handleNotFound (ti_sysbios_family_c7x_Hwi_E_handleNotFound__C)
#endif

/* E_allocSCFailed */
typedef xdc_runtime_Error_Id CT__ti_sysbios_family_c7x_Hwi_E_allocSCFailed;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_E_allocSCFailed ti_sysbios_family_c7x_Hwi_E_allocSCFailed__C;
#ifdef ti_sysbios_family_c7x_Hwi_E_allocSCFailed__CR
#define ti_sysbios_family_c7x_Hwi_E_allocSCFailed (*((CT__ti_sysbios_family_c7x_Hwi_E_allocSCFailed*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_E_allocSCFailed__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_E_allocSCFailed (ti_sysbios_family_c7x_Hwi_E_allocSCFailed__C)
#endif

/* E_registerSCFailed */
typedef xdc_runtime_Error_Id CT__ti_sysbios_family_c7x_Hwi_E_registerSCFailed;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_E_registerSCFailed ti_sysbios_family_c7x_Hwi_E_registerSCFailed__C;
#ifdef ti_sysbios_family_c7x_Hwi_E_registerSCFailed__CR
#define ti_sysbios_family_c7x_Hwi_E_registerSCFailed (*((CT__ti_sysbios_family_c7x_Hwi_E_registerSCFailed*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_E_registerSCFailed__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_E_registerSCFailed (ti_sysbios_family_c7x_Hwi_E_registerSCFailed__C)
#endif

/* E_invalidIntNum */
typedef xdc_runtime_Error_Id CT__ti_sysbios_family_c7x_Hwi_E_invalidIntNum;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_E_invalidIntNum ti_sysbios_family_c7x_Hwi_E_invalidIntNum__C;
#ifdef ti_sysbios_family_c7x_Hwi_E_invalidIntNum__CR
#define ti_sysbios_family_c7x_Hwi_E_invalidIntNum (*((CT__ti_sysbios_family_c7x_Hwi_E_invalidIntNum*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_E_invalidIntNum__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_E_invalidIntNum (ti_sysbios_family_c7x_Hwi_E_invalidIntNum__C)
#endif

/* LM_begin */
typedef xdc_runtime_Log_Event CT__ti_sysbios_family_c7x_Hwi_LM_begin;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_LM_begin ti_sysbios_family_c7x_Hwi_LM_begin__C;
#ifdef ti_sysbios_family_c7x_Hwi_LM_begin__CR
#define ti_sysbios_family_c7x_Hwi_LM_begin (*((CT__ti_sysbios_family_c7x_Hwi_LM_begin*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_LM_begin__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_LM_begin (ti_sysbios_family_c7x_Hwi_LM_begin__C)
#endif

/* LD_end */
typedef xdc_runtime_Log_Event CT__ti_sysbios_family_c7x_Hwi_LD_end;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_LD_end ti_sysbios_family_c7x_Hwi_LD_end__C;
#ifdef ti_sysbios_family_c7x_Hwi_LD_end__CR
#define ti_sysbios_family_c7x_Hwi_LD_end (*((CT__ti_sysbios_family_c7x_Hwi_LD_end*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_LD_end__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_LD_end (ti_sysbios_family_c7x_Hwi_LD_end__C)
#endif

/* enableException */
typedef xdc_Bool CT__ti_sysbios_family_c7x_Hwi_enableException;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_enableException ti_sysbios_family_c7x_Hwi_enableException__C;
#ifdef ti_sysbios_family_c7x_Hwi_enableException__CR
#define ti_sysbios_family_c7x_Hwi_enableException (*((CT__ti_sysbios_family_c7x_Hwi_enableException*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_enableException__C_offset)))
#else
#ifdef ti_sysbios_family_c7x_Hwi_enableException__D
#define ti_sysbios_family_c7x_Hwi_enableException (ti_sysbios_family_c7x_Hwi_enableException__D)
#else
#define ti_sysbios_family_c7x_Hwi_enableException (ti_sysbios_family_c7x_Hwi_enableException__C)
#endif
#endif

/* swiDisable */
typedef xdc_UInt (*CT__ti_sysbios_family_c7x_Hwi_swiDisable)(void);
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_swiDisable ti_sysbios_family_c7x_Hwi_swiDisable__C;
#ifdef ti_sysbios_family_c7x_Hwi_swiDisable__CR
#define ti_sysbios_family_c7x_Hwi_swiDisable (*((CT__ti_sysbios_family_c7x_Hwi_swiDisable*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_swiDisable__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_swiDisable (ti_sysbios_family_c7x_Hwi_swiDisable__C)
#endif

/* swiRestoreHwi */
typedef xdc_Void (*CT__ti_sysbios_family_c7x_Hwi_swiRestoreHwi)(xdc_UInt arg1);
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_swiRestoreHwi ti_sysbios_family_c7x_Hwi_swiRestoreHwi__C;
#ifdef ti_sysbios_family_c7x_Hwi_swiRestoreHwi__CR
#define ti_sysbios_family_c7x_Hwi_swiRestoreHwi (*((CT__ti_sysbios_family_c7x_Hwi_swiRestoreHwi*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_swiRestoreHwi__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_swiRestoreHwi (ti_sysbios_family_c7x_Hwi_swiRestoreHwi__C)
#endif

/* taskDisable */
typedef xdc_UInt (*CT__ti_sysbios_family_c7x_Hwi_taskDisable)(void);
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_taskDisable ti_sysbios_family_c7x_Hwi_taskDisable__C;
#ifdef ti_sysbios_family_c7x_Hwi_taskDisable__CR
#define ti_sysbios_family_c7x_Hwi_taskDisable (*((CT__ti_sysbios_family_c7x_Hwi_taskDisable*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_taskDisable__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_taskDisable (ti_sysbios_family_c7x_Hwi_taskDisable__C)
#endif

/* taskRestoreHwi */
typedef xdc_Void (*CT__ti_sysbios_family_c7x_Hwi_taskRestoreHwi)(xdc_UInt arg1);
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_taskRestoreHwi ti_sysbios_family_c7x_Hwi_taskRestoreHwi__C;
#ifdef ti_sysbios_family_c7x_Hwi_taskRestoreHwi__CR
#define ti_sysbios_family_c7x_Hwi_taskRestoreHwi (*((CT__ti_sysbios_family_c7x_Hwi_taskRestoreHwi*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_taskRestoreHwi__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_taskRestoreHwi (ti_sysbios_family_c7x_Hwi_taskRestoreHwi__C)
#endif

/* hooks */
typedef ti_sysbios_family_c7x_Hwi_HookSet __T1_ti_sysbios_family_c7x_Hwi_hooks;
typedef struct { int length; ti_sysbios_family_c7x_Hwi_HookSet *elem; } ARRAY1_ti_sysbios_family_c7x_Hwi_hooks;
typedef struct { int length; ti_sysbios_family_c7x_Hwi_HookSet const *elem; } CARRAY1_ti_sysbios_family_c7x_Hwi_hooks;
typedef CARRAY1_ti_sysbios_family_c7x_Hwi_hooks __TA_ti_sysbios_family_c7x_Hwi_hooks;
typedef CARRAY1_ti_sysbios_family_c7x_Hwi_hooks CT__ti_sysbios_family_c7x_Hwi_hooks;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Hwi_hooks ti_sysbios_family_c7x_Hwi_hooks__C;
#ifdef ti_sysbios_family_c7x_Hwi_hooks__CR
#define ti_sysbios_family_c7x_Hwi_hooks (*((CT__ti_sysbios_family_c7x_Hwi_hooks*)(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_hooks__C_offset)))
#else
#define ti_sysbios_family_c7x_Hwi_hooks (ti_sysbios_family_c7x_Hwi_hooks__C)
#endif


/*
 * ======== PER-INSTANCE TYPES ========
 */

/* Params */
struct ti_sysbios_family_c7x_Hwi_Params {
    size_t __size;
    const void *__self;
    void *__fxns;
    xdc_runtime_IInstance_Params *instance;
    ti_sysbios_interfaces_IHwi_MaskingOption maskSetting;
    xdc_UArg arg;
    xdc_Bool enableInt;
    xdc_Int eventId;
    xdc_Int priority;
    xdc_ULong disableMask;
    xdc_ULong restoreMask;
    xdc_runtime_IInstance_Params __iprms;
};

/* Struct */
struct ti_sysbios_family_c7x_Hwi_Struct {
    const ti_sysbios_family_c7x_Hwi_Fxns__ *__fxns;
    xdc_ULong f0;
    xdc_ULong f1;
    xdc_UArg f2;
    ti_sysbios_family_c7x_Hwi_FuncPtr f3;
    xdc_Int f4;
    xdc_Int f5;
    ti_sysbios_family_c7x_Hwi_Irp f6;
    __TA_ti_sysbios_family_c7x_Hwi_Instance_State__hookEnv f7;
    xdc_runtime_Types_CordAddr __name;
};


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_sysbios_family_c7x_Hwi_Fxns__ {
    const xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2* __sysp;
    xdc_Bool (*getStackInfo)(ti_sysbios_interfaces_IHwi_StackInfo* stkInfo, xdc_Bool computeStackDepth);
    xdc_Bool (*getCoreStackInfo)(ti_sysbios_interfaces_IHwi_StackInfo* stkInfo, xdc_Bool computeStackDepth, xdc_UInt coreId);
    xdc_Void (*startup)(void);
    xdc_UInt (*disable)(void);
    xdc_UInt (*enable)(void);
    xdc_Void (*restore)(xdc_UInt key);
    xdc_Void (*switchFromBootStack)(void);
    xdc_Void (*post)(xdc_UInt intNum);
    xdc_Char *(*getTaskSP)(void);
    xdc_UInt (*disableInterrupt)(xdc_UInt intNum);
    xdc_UInt (*enableInterrupt)(xdc_UInt intNum);
    xdc_Void (*restoreInterrupt)(xdc_UInt intNum, xdc_UInt key);
    xdc_Void (*clearInterrupt)(xdc_UInt intNum);
    ti_sysbios_interfaces_IHwi_FuncPtr (*getFunc)(ti_sysbios_family_c7x_Hwi_Handle inst, xdc_UArg* arg);
    xdc_Void (*setFunc)(ti_sysbios_family_c7x_Hwi_Handle inst, ti_sysbios_interfaces_IHwi_FuncPtr fxn, xdc_UArg arg);
    xdc_Ptr (*getHookContext)(ti_sysbios_family_c7x_Hwi_Handle inst, xdc_Int id);
    xdc_Void (*setHookContext)(ti_sysbios_family_c7x_Hwi_Handle inst, xdc_Int id, xdc_Ptr hookContext);
    ti_sysbios_interfaces_IHwi_Irp (*getIrp)(ti_sysbios_family_c7x_Hwi_Handle inst);
    xdc_runtime_Types_SysFxns2 __sfxns;
};
#ifndef ti_sysbios_family_c7x_Hwi_Module__FXNS__CR

/* Module__FXNS__C */
__extern const ti_sysbios_family_c7x_Hwi_Fxns__ ti_sysbios_family_c7x_Hwi_Module__FXNS__C;
#else
#define ti_sysbios_family_c7x_Hwi_Module__FXNS__C (*(xdcRomConstPtr + ti_sysbios_family_c7x_Hwi_Module__FXNS__C_offset))
#endif


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_c7x_Hwi_Module_startup ti_sysbios_family_c7x_Hwi_Module_startup__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_Module_startup__E, "ti_sysbios_family_c7x_Hwi_Module_startup")
__extern xdc_Int ti_sysbios_family_c7x_Hwi_Module_startup__E( xdc_Int state );
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_Module_startup__F, "ti_sysbios_family_c7x_Hwi_Module_startup")
__extern xdc_Int ti_sysbios_family_c7x_Hwi_Module_startup__F( xdc_Int state );

/* Instance_init__E */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_Instance_init__E, "ti_sysbios_family_c7x_Hwi_Instance_init")
__extern xdc_Int ti_sysbios_family_c7x_Hwi_Instance_init__E(ti_sysbios_family_c7x_Hwi_Object *obj, xdc_Int intNum, ti_sysbios_interfaces_IHwi_FuncPtr hwiFxn, const ti_sysbios_family_c7x_Hwi_Params *prms, xdc_runtime_Error_Block *eb);

/* Instance_finalize__E */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_Instance_finalize__E, "ti_sysbios_family_c7x_Hwi_Instance_finalize")
__extern void ti_sysbios_family_c7x_Hwi_Instance_finalize__E(ti_sysbios_family_c7x_Hwi_Object *obj, int ec);

/* create */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_create, "ti_sysbios_family_c7x_Hwi_create")
__extern ti_sysbios_family_c7x_Hwi_Handle ti_sysbios_family_c7x_Hwi_create( xdc_Int intNum, ti_sysbios_interfaces_IHwi_FuncPtr hwiFxn, const ti_sysbios_family_c7x_Hwi_Params *prms, xdc_runtime_Error_Block *eb);

/* construct */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_construct, "ti_sysbios_family_c7x_Hwi_construct")
__extern void ti_sysbios_family_c7x_Hwi_construct(ti_sysbios_family_c7x_Hwi_Struct *obj, xdc_Int intNum, ti_sysbios_interfaces_IHwi_FuncPtr hwiFxn, const ti_sysbios_family_c7x_Hwi_Params *prms, xdc_runtime_Error_Block *eb);

/* delete */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_delete, "ti_sysbios_family_c7x_Hwi_delete")
__extern void ti_sysbios_family_c7x_Hwi_delete(ti_sysbios_family_c7x_Hwi_Handle *instp);

/* destruct */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_destruct, "ti_sysbios_family_c7x_Hwi_destruct")
__extern void ti_sysbios_family_c7x_Hwi_destruct(ti_sysbios_family_c7x_Hwi_Struct *obj);

/* Handle__label__S */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_Handle__label__S, "ti_sysbios_family_c7x_Hwi_Handle__label__S")
__extern xdc_runtime_Types_Label *ti_sysbios_family_c7x_Hwi_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label *lab );

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_Module__startupDone__S, "ti_sysbios_family_c7x_Hwi_Module__startupDone__S")
__extern xdc_Bool ti_sysbios_family_c7x_Hwi_Module__startupDone__S( void );

/* Object__create__S */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_Object__create__S, "ti_sysbios_family_c7x_Hwi_Object__create__S")
__extern xdc_Ptr ti_sysbios_family_c7x_Hwi_Object__create__S( xdc_CPtr aa, const xdc_UChar *pa, xdc_SizeT psz, xdc_runtime_Error_Block *eb );

/* Object__delete__S */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_Object__delete__S, "ti_sysbios_family_c7x_Hwi_Object__delete__S")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_Object__delete__S( xdc_Ptr instp );

/* Object__get__S */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_Object__get__S, "ti_sysbios_family_c7x_Hwi_Object__get__S")
__extern xdc_Ptr ti_sysbios_family_c7x_Hwi_Object__get__S( xdc_Ptr oarr, xdc_Int i );

/* Object__first__S */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_Object__first__S, "ti_sysbios_family_c7x_Hwi_Object__first__S")
__extern xdc_Ptr ti_sysbios_family_c7x_Hwi_Object__first__S( void );

/* Object__next__S */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_Object__next__S, "ti_sysbios_family_c7x_Hwi_Object__next__S")
__extern xdc_Ptr ti_sysbios_family_c7x_Hwi_Object__next__S( xdc_Ptr obj );

/* Params__init__S */
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_Params__init__S, "ti_sysbios_family_c7x_Hwi_Params__init__S")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_Params__init__S( xdc_Ptr dst, const xdc_Void *src, xdc_SizeT psz, xdc_SizeT isz );

/* getStackInfo__E */
#define ti_sysbios_family_c7x_Hwi_getStackInfo ti_sysbios_family_c7x_Hwi_getStackInfo__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_getStackInfo__E, "ti_sysbios_family_c7x_Hwi_getStackInfo")
__extern xdc_Bool ti_sysbios_family_c7x_Hwi_getStackInfo__E( ti_sysbios_interfaces_IHwi_StackInfo *stkInfo, xdc_Bool computeStackDepth );

/* getCoreStackInfo__E */
#define ti_sysbios_family_c7x_Hwi_getCoreStackInfo ti_sysbios_family_c7x_Hwi_getCoreStackInfo__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_getCoreStackInfo__E, "ti_sysbios_family_c7x_Hwi_getCoreStackInfo")
__extern xdc_Bool ti_sysbios_family_c7x_Hwi_getCoreStackInfo__E( ti_sysbios_interfaces_IHwi_StackInfo *stkInfo, xdc_Bool computeStackDepth, xdc_UInt coreId );

/* startup__E */
#define ti_sysbios_family_c7x_Hwi_startup ti_sysbios_family_c7x_Hwi_startup__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_startup__E, "ti_sysbios_family_c7x_Hwi_startup")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_startup__E( void );

/* switchFromBootStack__E */
#define ti_sysbios_family_c7x_Hwi_switchFromBootStack ti_sysbios_family_c7x_Hwi_switchFromBootStack__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_switchFromBootStack__E, "ti_sysbios_family_c7x_Hwi_switchFromBootStack")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_switchFromBootStack__E( void );

/* post__E */
#define ti_sysbios_family_c7x_Hwi_post ti_sysbios_family_c7x_Hwi_post__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_post__E, "ti_sysbios_family_c7x_Hwi_post")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_post__E( xdc_UInt intNum );

/* getTaskSP__E */
#define ti_sysbios_family_c7x_Hwi_getTaskSP ti_sysbios_family_c7x_Hwi_getTaskSP__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_getTaskSP__E, "ti_sysbios_family_c7x_Hwi_getTaskSP")
__extern xdc_Char *ti_sysbios_family_c7x_Hwi_getTaskSP__E( void );

/* disableInterrupt__E */
#define ti_sysbios_family_c7x_Hwi_disableInterrupt ti_sysbios_family_c7x_Hwi_disableInterrupt__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_disableInterrupt__E, "ti_sysbios_family_c7x_Hwi_disableInterrupt")
__extern xdc_UInt ti_sysbios_family_c7x_Hwi_disableInterrupt__E( xdc_UInt intNum );

/* enableInterrupt__E */
#define ti_sysbios_family_c7x_Hwi_enableInterrupt ti_sysbios_family_c7x_Hwi_enableInterrupt__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_enableInterrupt__E, "ti_sysbios_family_c7x_Hwi_enableInterrupt")
__extern xdc_UInt ti_sysbios_family_c7x_Hwi_enableInterrupt__E( xdc_UInt intNum );

/* restoreInterrupt__E */
#define ti_sysbios_family_c7x_Hwi_restoreInterrupt ti_sysbios_family_c7x_Hwi_restoreInterrupt__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_restoreInterrupt__E, "ti_sysbios_family_c7x_Hwi_restoreInterrupt")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_restoreInterrupt__E( xdc_UInt intNum, xdc_UInt key );

/* clearInterrupt__E */
#define ti_sysbios_family_c7x_Hwi_clearInterrupt ti_sysbios_family_c7x_Hwi_clearInterrupt__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_clearInterrupt__E, "ti_sysbios_family_c7x_Hwi_clearInterrupt")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_clearInterrupt__E( xdc_UInt intNum );

/* getFunc__E */
#define ti_sysbios_family_c7x_Hwi_getFunc ti_sysbios_family_c7x_Hwi_getFunc__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_getFunc__E, "ti_sysbios_family_c7x_Hwi_getFunc")
__extern ti_sysbios_interfaces_IHwi_FuncPtr ti_sysbios_family_c7x_Hwi_getFunc__E( ti_sysbios_family_c7x_Hwi_Handle __inst, xdc_UArg *arg );

/* setFunc__E */
#define ti_sysbios_family_c7x_Hwi_setFunc ti_sysbios_family_c7x_Hwi_setFunc__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_setFunc__E, "ti_sysbios_family_c7x_Hwi_setFunc")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_setFunc__E( ti_sysbios_family_c7x_Hwi_Handle __inst, ti_sysbios_interfaces_IHwi_FuncPtr fxn, xdc_UArg arg );

/* getHookContext__E */
#define ti_sysbios_family_c7x_Hwi_getHookContext ti_sysbios_family_c7x_Hwi_getHookContext__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_getHookContext__E, "ti_sysbios_family_c7x_Hwi_getHookContext")
__extern xdc_Ptr ti_sysbios_family_c7x_Hwi_getHookContext__E( ti_sysbios_family_c7x_Hwi_Handle __inst, xdc_Int id );

/* setHookContext__E */
#define ti_sysbios_family_c7x_Hwi_setHookContext ti_sysbios_family_c7x_Hwi_setHookContext__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_setHookContext__E, "ti_sysbios_family_c7x_Hwi_setHookContext")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_setHookContext__E( ti_sysbios_family_c7x_Hwi_Handle __inst, xdc_Int id, xdc_Ptr hookContext );

/* getIrp__E */
#define ti_sysbios_family_c7x_Hwi_getIrp ti_sysbios_family_c7x_Hwi_getIrp__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_getIrp__E, "ti_sysbios_family_c7x_Hwi_getIrp")
__extern ti_sysbios_interfaces_IHwi_Irp ti_sysbios_family_c7x_Hwi_getIrp__E( ti_sysbios_family_c7x_Hwi_Handle __inst );

/* eventMap__E */
#define ti_sysbios_family_c7x_Hwi_eventMap ti_sysbios_family_c7x_Hwi_eventMap__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_eventMap__E, "ti_sysbios_family_c7x_Hwi_eventMap")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_eventMap__E( xdc_Int intNum, xdc_Int eventId );

/* plug__E */
#define ti_sysbios_family_c7x_Hwi_plug ti_sysbios_family_c7x_Hwi_plug__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_plug__E, "ti_sysbios_family_c7x_Hwi_plug")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_plug__E( xdc_UInt intNum, ti_sysbios_family_c7x_Hwi_PlugFuncPtr fxn );

/* getEventId__E */
#define ti_sysbios_family_c7x_Hwi_getEventId ti_sysbios_family_c7x_Hwi_getEventId__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_getEventId__E, "ti_sysbios_family_c7x_Hwi_getEventId")
__extern xdc_Int ti_sysbios_family_c7x_Hwi_getEventId__E( xdc_UInt intNum );

/* getHandle__E */
#define ti_sysbios_family_c7x_Hwi_getHandle ti_sysbios_family_c7x_Hwi_getHandle__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_getHandle__E, "ti_sysbios_family_c7x_Hwi_getHandle")
__extern ti_sysbios_family_c7x_Hwi_Handle ti_sysbios_family_c7x_Hwi_getHandle__E( xdc_UInt intNum );

/* disableIER__E */
#define ti_sysbios_family_c7x_Hwi_disableIER ti_sysbios_family_c7x_Hwi_disableIER__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_disableIER__E, "ti_sysbios_family_c7x_Hwi_disableIER")
__extern xdc_ULong ti_sysbios_family_c7x_Hwi_disableIER__E( xdc_ULong mask );

/* enableIER__E */
#define ti_sysbios_family_c7x_Hwi_enableIER ti_sysbios_family_c7x_Hwi_enableIER__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_enableIER__E, "ti_sysbios_family_c7x_Hwi_enableIER")
__extern xdc_ULong ti_sysbios_family_c7x_Hwi_enableIER__E( xdc_ULong mask );

/* restoreIER__E */
#define ti_sysbios_family_c7x_Hwi_restoreIER ti_sysbios_family_c7x_Hwi_restoreIER__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_restoreIER__E, "ti_sysbios_family_c7x_Hwi_restoreIER")
__extern xdc_ULong ti_sysbios_family_c7x_Hwi_restoreIER__E( xdc_ULong mask );

/* setPriority__E */
#define ti_sysbios_family_c7x_Hwi_setPriority ti_sysbios_family_c7x_Hwi_setPriority__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_setPriority__E, "ti_sysbios_family_c7x_Hwi_setPriority")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_setPriority__E( xdc_UInt intNum, xdc_UInt priority );

/* setCOP__E */
#define ti_sysbios_family_c7x_Hwi_setCOP ti_sysbios_family_c7x_Hwi_setCOP__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_setCOP__E, "ti_sysbios_family_c7x_Hwi_setCOP")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_setCOP__E( xdc_Int cop );

/* reconfig__E */
#define ti_sysbios_family_c7x_Hwi_reconfig ti_sysbios_family_c7x_Hwi_reconfig__E
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_reconfig__E, "ti_sysbios_family_c7x_Hwi_reconfig")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_reconfig__E( ti_sysbios_family_c7x_Hwi_Handle __inst, ti_sysbios_family_c7x_Hwi_FuncPtr fxn, const ti_sysbios_family_c7x_Hwi_Params *params );

/* getIsrStackAddress__I */
#define ti_sysbios_family_c7x_Hwi_getIsrStackAddress ti_sysbios_family_c7x_Hwi_getIsrStackAddress__I
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_getIsrStackAddress__I, "ti_sysbios_family_c7x_Hwi_getIsrStackAddress")
__extern xdc_Char *ti_sysbios_family_c7x_Hwi_getIsrStackAddress__I( void );

/* dispatchC__I */
#define ti_sysbios_family_c7x_Hwi_dispatchC ti_sysbios_family_c7x_Hwi_dispatchC__I
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_dispatchC__I, "ti_sysbios_family_c7x_Hwi_dispatchC")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_dispatchC__I( xdc_Int intNum );

/* dispatchCore__I */
#define ti_sysbios_family_c7x_Hwi_dispatchCore ti_sysbios_family_c7x_Hwi_dispatchCore__I
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_dispatchCore__I, "ti_sysbios_family_c7x_Hwi_dispatchCore")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_dispatchCore__I( xdc_Int intNum );

/* unPluggedInterrupt__I */
#define ti_sysbios_family_c7x_Hwi_unPluggedInterrupt ti_sysbios_family_c7x_Hwi_unPluggedInterrupt__I
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_unPluggedInterrupt__I, "ti_sysbios_family_c7x_Hwi_unPluggedInterrupt")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_unPluggedInterrupt__I( void );

/* switchAndDispatch__I */
#define ti_sysbios_family_c7x_Hwi_switchAndDispatch ti_sysbios_family_c7x_Hwi_switchAndDispatch__I
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_switchAndDispatch__I, "ti_sysbios_family_c7x_Hwi_switchAndDispatch")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_switchAndDispatch__I( xdc_Int intNum );

/* setupSC__I */
#define ti_sysbios_family_c7x_Hwi_setupSC ti_sysbios_family_c7x_Hwi_setupSC__I
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_setupSC__I, "ti_sysbios_family_c7x_Hwi_setupSC")
__extern xdc_Void ti_sysbios_family_c7x_Hwi_setupSC__I( void );

/* postInit__I */
#define ti_sysbios_family_c7x_Hwi_postInit ti_sysbios_family_c7x_Hwi_postInit__I
xdc__CODESECT(ti_sysbios_family_c7x_Hwi_postInit__I, "ti_sysbios_family_c7x_Hwi_postInit")
__extern xdc_Int ti_sysbios_family_c7x_Hwi_postInit__I( ti_sysbios_family_c7x_Hwi_Object *hwi, xdc_runtime_Error_Block *eb );


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline ti_sysbios_interfaces_IHwi_Module ti_sysbios_family_c7x_Hwi_Module_upCast(void);
static inline ti_sysbios_interfaces_IHwi_Module ti_sysbios_family_c7x_Hwi_Module_upCast(void)
{
    return (ti_sysbios_interfaces_IHwi_Module)&ti_sysbios_family_c7x_Hwi_Module__FXNS__C;
}

/* Module_to_ti_sysbios_interfaces_IHwi */
#define ti_sysbios_family_c7x_Hwi_Module_to_ti_sysbios_interfaces_IHwi ti_sysbios_family_c7x_Hwi_Module_upCast

/* Handle_upCast */
static inline ti_sysbios_interfaces_IHwi_Handle ti_sysbios_family_c7x_Hwi_Handle_upCast(ti_sysbios_family_c7x_Hwi_Handle i);
static inline ti_sysbios_interfaces_IHwi_Handle ti_sysbios_family_c7x_Hwi_Handle_upCast(ti_sysbios_family_c7x_Hwi_Handle i)
{
    return (ti_sysbios_interfaces_IHwi_Handle)i;
}

/* Handle_to_ti_sysbios_interfaces_IHwi */
#define ti_sysbios_family_c7x_Hwi_Handle_to_ti_sysbios_interfaces_IHwi ti_sysbios_family_c7x_Hwi_Handle_upCast

/* Handle_downCast */
static inline ti_sysbios_family_c7x_Hwi_Handle ti_sysbios_family_c7x_Hwi_Handle_downCast(ti_sysbios_interfaces_IHwi_Handle i);
static inline ti_sysbios_family_c7x_Hwi_Handle ti_sysbios_family_c7x_Hwi_Handle_downCast(ti_sysbios_interfaces_IHwi_Handle i)
{
    ti_sysbios_interfaces_IHwi_Handle i2 = (ti_sysbios_interfaces_IHwi_Handle)i;
    return ((const void*)i2->__fxns == (const void*)&ti_sysbios_family_c7x_Hwi_Module__FXNS__C) ? (ti_sysbios_family_c7x_Hwi_Handle)i : (ti_sysbios_family_c7x_Hwi_Handle)NULL;
}

/* Handle_from_ti_sysbios_interfaces_IHwi */
#define ti_sysbios_family_c7x_Hwi_Handle_from_ti_sysbios_interfaces_IHwi ti_sysbios_family_c7x_Hwi_Handle_downCast


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_c7x_Hwi_Module_startupDone() ti_sysbios_family_c7x_Hwi_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_c7x_Hwi_Object_heap() ti_sysbios_family_c7x_Hwi_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_c7x_Hwi_Module_heap() ti_sysbios_family_c7x_Hwi_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_c7x_Hwi_Module__id ti_sysbios_family_c7x_Hwi_Module_id(void);
static inline CT__ti_sysbios_family_c7x_Hwi_Module__id ti_sysbios_family_c7x_Hwi_Module_id( void ) 
{
    return ti_sysbios_family_c7x_Hwi_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_c7x_Hwi_Module_hasMask(void);
static inline xdc_Bool ti_sysbios_family_c7x_Hwi_Module_hasMask(void)
{
    return (xdc_Bool)(ti_sysbios_family_c7x_Hwi_Module__diagsMask__C != (CT__ti_sysbios_family_c7x_Hwi_Module__diagsMask)NULL);
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_c7x_Hwi_Module_getMask(void);
static inline xdc_Bits16 ti_sysbios_family_c7x_Hwi_Module_getMask(void)
{
    return (ti_sysbios_family_c7x_Hwi_Module__diagsMask__C != (CT__ti_sysbios_family_c7x_Hwi_Module__diagsMask)NULL) ? *ti_sysbios_family_c7x_Hwi_Module__diagsMask__C : (xdc_Bits16)0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_c7x_Hwi_Module_setMask(xdc_Bits16 mask);
static inline xdc_Void ti_sysbios_family_c7x_Hwi_Module_setMask(xdc_Bits16 mask)
{
    if (ti_sysbios_family_c7x_Hwi_Module__diagsMask__C != (CT__ti_sysbios_family_c7x_Hwi_Module__diagsMask)NULL) {
        *ti_sysbios_family_c7x_Hwi_Module__diagsMask__C = mask;
    }
}

/* Params_init */
static inline void ti_sysbios_family_c7x_Hwi_Params_init(ti_sysbios_family_c7x_Hwi_Params *prms);
static inline void ti_sysbios_family_c7x_Hwi_Params_init( ti_sysbios_family_c7x_Hwi_Params *prms ) 
{
    if (prms != NULL) {
        ti_sysbios_family_c7x_Hwi_Params__init__S(prms, NULL, sizeof(ti_sysbios_family_c7x_Hwi_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Params_copy */
static inline void ti_sysbios_family_c7x_Hwi_Params_copy(ti_sysbios_family_c7x_Hwi_Params *dst, const ti_sysbios_family_c7x_Hwi_Params *src);
static inline void ti_sysbios_family_c7x_Hwi_Params_copy(ti_sysbios_family_c7x_Hwi_Params *dst, const ti_sysbios_family_c7x_Hwi_Params *src) 
{
    if (dst != NULL) {
        ti_sysbios_family_c7x_Hwi_Params__init__S(dst, (const void *)src, sizeof(ti_sysbios_family_c7x_Hwi_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Object_count */
#define ti_sysbios_family_c7x_Hwi_Object_count() ti_sysbios_family_c7x_Hwi_Object__count__C

/* Object_sizeof */
#define ti_sysbios_family_c7x_Hwi_Object_sizeof() ti_sysbios_family_c7x_Hwi_Object__sizeof__C

/* Object_get */
static inline ti_sysbios_family_c7x_Hwi_Handle ti_sysbios_family_c7x_Hwi_Object_get(ti_sysbios_family_c7x_Hwi_Instance_State *oarr, int i);
static inline ti_sysbios_family_c7x_Hwi_Handle ti_sysbios_family_c7x_Hwi_Object_get(ti_sysbios_family_c7x_Hwi_Instance_State *oarr, int i) 
{
    return (ti_sysbios_family_c7x_Hwi_Handle)ti_sysbios_family_c7x_Hwi_Object__get__S(oarr, i);
}

/* Object_first */
static inline ti_sysbios_family_c7x_Hwi_Handle ti_sysbios_family_c7x_Hwi_Object_first(void);
static inline ti_sysbios_family_c7x_Hwi_Handle ti_sysbios_family_c7x_Hwi_Object_first(void)
{
    return (ti_sysbios_family_c7x_Hwi_Handle)ti_sysbios_family_c7x_Hwi_Object__first__S();
}

/* Object_next */
static inline ti_sysbios_family_c7x_Hwi_Handle ti_sysbios_family_c7x_Hwi_Object_next(ti_sysbios_family_c7x_Hwi_Object *obj);
static inline ti_sysbios_family_c7x_Hwi_Handle ti_sysbios_family_c7x_Hwi_Object_next(ti_sysbios_family_c7x_Hwi_Object *obj)
{
    return (ti_sysbios_family_c7x_Hwi_Handle)ti_sysbios_family_c7x_Hwi_Object__next__S(obj);
}

/* Handle_label */
static inline xdc_runtime_Types_Label *ti_sysbios_family_c7x_Hwi_Handle_label(ti_sysbios_family_c7x_Hwi_Handle inst, xdc_runtime_Types_Label *lab);
static inline xdc_runtime_Types_Label *ti_sysbios_family_c7x_Hwi_Handle_label(ti_sysbios_family_c7x_Hwi_Handle inst, xdc_runtime_Types_Label *lab)
{
    return ti_sysbios_family_c7x_Hwi_Handle__label__S(inst, lab);
}

/* Handle_name */
static inline xdc_String ti_sysbios_family_c7x_Hwi_Handle_name(ti_sysbios_family_c7x_Hwi_Handle inst);
static inline xdc_String ti_sysbios_family_c7x_Hwi_Handle_name(ti_sysbios_family_c7x_Hwi_Handle inst)
{
    xdc_runtime_Types_Label lab;
    return ti_sysbios_family_c7x_Hwi_Handle__label__S(inst, &lab)->iname;
}

/* handle */
static inline ti_sysbios_family_c7x_Hwi_Handle ti_sysbios_family_c7x_Hwi_handle(ti_sysbios_family_c7x_Hwi_Struct *str);
static inline ti_sysbios_family_c7x_Hwi_Handle ti_sysbios_family_c7x_Hwi_handle(ti_sysbios_family_c7x_Hwi_Struct *str)
{
    return (ti_sysbios_family_c7x_Hwi_Handle)str;
}

/* struct */
static inline ti_sysbios_family_c7x_Hwi_Struct *ti_sysbios_family_c7x_Hwi_struct(ti_sysbios_family_c7x_Hwi_Handle inst);
static inline ti_sysbios_family_c7x_Hwi_Struct *ti_sysbios_family_c7x_Hwi_struct(ti_sysbios_family_c7x_Hwi_Handle inst)
{
    return (ti_sysbios_family_c7x_Hwi_Struct*)inst;
}


/*
 * ======== EPILOGUE ========
 */

#include <ti/sysbios/family/c7x/Hwi__epilogue.h>

#ifdef ti_sysbios_family_c7x_Hwi__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_c7x_Hwi__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_c7x_Hwi__internalaccess))

#ifndef ti_sysbios_family_c7x_Hwi__include_state
#define ti_sysbios_family_c7x_Hwi__include_state

/* Module_State */
struct ti_sysbios_family_c7x_Hwi_Module_State {
    __TA_ti_sysbios_family_c7x_Hwi_Module_State__intEvents intEvents;
    xdc_ULong ierMask;
    volatile xdc_Int intNum;
    xdc_Char *taskSP;
    xdc_Char *isrStack;
    xdc_Ptr vectorTableBase;
    xdc_Int scw;
    __TA_ti_sysbios_family_c7x_Hwi_Module_State__dispatchTable dispatchTable;
};

/* Module__state__V */
#ifndef ti_sysbios_family_c7x_Hwi_Module__state__VR
extern struct ti_sysbios_family_c7x_Hwi_Module_State__ ti_sysbios_family_c7x_Hwi_Module__state__V;
#else
#define ti_sysbios_family_c7x_Hwi_Module__state__V (*((struct ti_sysbios_family_c7x_Hwi_Module_State__*)(xdcRomStatePtr + ti_sysbios_family_c7x_Hwi_Module__state__V_offset)))
#endif

/* Object */
struct ti_sysbios_family_c7x_Hwi_Object {
    const ti_sysbios_family_c7x_Hwi_Fxns__ *__fxns;
    xdc_ULong disableMask;
    xdc_ULong restoreMask;
    xdc_UArg arg;
    ti_sysbios_family_c7x_Hwi_FuncPtr fxn;
    xdc_Int intNum;
    xdc_Int priority;
    ti_sysbios_family_c7x_Hwi_Irp irp;
    __TA_ti_sysbios_family_c7x_Hwi_Instance_State__hookEnv hookEnv;
};

#endif /* ti_sysbios_family_c7x_Hwi__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_c7x_Hwi__nolocalnames)

#ifndef ti_sysbios_family_c7x_Hwi__localnames__done
#define ti_sysbios_family_c7x_Hwi__localnames__done

/* module prefix */
#define Hwi_Instance ti_sysbios_family_c7x_Hwi_Instance
#define Hwi_Handle ti_sysbios_family_c7x_Hwi_Handle
#define Hwi_Module ti_sysbios_family_c7x_Hwi_Module
#define Hwi_Object ti_sysbios_family_c7x_Hwi_Object
#define Hwi_Struct ti_sysbios_family_c7x_Hwi_Struct
#define Hwi_FuncPtr ti_sysbios_family_c7x_Hwi_FuncPtr
#define Hwi_Irp ti_sysbios_family_c7x_Hwi_Irp
#define Hwi_HookSet ti_sysbios_family_c7x_Hwi_HookSet
#define Hwi_MaskingOption ti_sysbios_family_c7x_Hwi_MaskingOption
#define Hwi_StackInfo ti_sysbios_family_c7x_Hwi_StackInfo
#define Hwi_NUM_INTERRUPTS ti_sysbios_family_c7x_Hwi_NUM_INTERRUPTS
#define Hwi_PlugFuncPtr ti_sysbios_family_c7x_Hwi_PlugFuncPtr
#define Hwi_Instance_State ti_sysbios_family_c7x_Hwi_Instance_State
#define Hwi_Module_State ti_sysbios_family_c7x_Hwi_Module_State
#define Hwi_MaskingOption_NONE ti_sysbios_family_c7x_Hwi_MaskingOption_NONE
#define Hwi_MaskingOption_ALL ti_sysbios_family_c7x_Hwi_MaskingOption_ALL
#define Hwi_MaskingOption_SELF ti_sysbios_family_c7x_Hwi_MaskingOption_SELF
#define Hwi_MaskingOption_BITMASK ti_sysbios_family_c7x_Hwi_MaskingOption_BITMASK
#define Hwi_MaskingOption_LOWER ti_sysbios_family_c7x_Hwi_MaskingOption_LOWER
#define Hwi_dispatcherAutoNestingSupport ti_sysbios_family_c7x_Hwi_dispatcherAutoNestingSupport
#define Hwi_dispatcherSwiSupport ti_sysbios_family_c7x_Hwi_dispatcherSwiSupport
#define Hwi_dispatcherTaskSupport ti_sysbios_family_c7x_Hwi_dispatcherTaskSupport
#define Hwi_dispatcherIrpTrackingSupport ti_sysbios_family_c7x_Hwi_dispatcherIrpTrackingSupport
#define Hwi_DEFAULT_INT_PRIORITY ti_sysbios_family_c7x_Hwi_DEFAULT_INT_PRIORITY
#define Hwi_E_alreadyDefined ti_sysbios_family_c7x_Hwi_E_alreadyDefined
#define Hwi_E_handleNotFound ti_sysbios_family_c7x_Hwi_E_handleNotFound
#define Hwi_E_allocSCFailed ti_sysbios_family_c7x_Hwi_E_allocSCFailed
#define Hwi_E_registerSCFailed ti_sysbios_family_c7x_Hwi_E_registerSCFailed
#define Hwi_E_invalidIntNum ti_sysbios_family_c7x_Hwi_E_invalidIntNum
#define Hwi_LM_begin ti_sysbios_family_c7x_Hwi_LM_begin
#define Hwi_LD_end ti_sysbios_family_c7x_Hwi_LD_end
#define Hwi_enableException ti_sysbios_family_c7x_Hwi_enableException
#define Hwi_swiDisable ti_sysbios_family_c7x_Hwi_swiDisable
#define Hwi_swiRestoreHwi ti_sysbios_family_c7x_Hwi_swiRestoreHwi
#define Hwi_taskDisable ti_sysbios_family_c7x_Hwi_taskDisable
#define Hwi_taskRestoreHwi ti_sysbios_family_c7x_Hwi_taskRestoreHwi
#define Hwi_hooks ti_sysbios_family_c7x_Hwi_hooks
#define Hwi_Params ti_sysbios_family_c7x_Hwi_Params
#define Hwi_getStackInfo ti_sysbios_family_c7x_Hwi_getStackInfo
#define Hwi_getCoreStackInfo ti_sysbios_family_c7x_Hwi_getCoreStackInfo
#define Hwi_startup ti_sysbios_family_c7x_Hwi_startup
#define Hwi_switchFromBootStack ti_sysbios_family_c7x_Hwi_switchFromBootStack
#define Hwi_post ti_sysbios_family_c7x_Hwi_post
#define Hwi_getTaskSP ti_sysbios_family_c7x_Hwi_getTaskSP
#define Hwi_disableInterrupt ti_sysbios_family_c7x_Hwi_disableInterrupt
#define Hwi_enableInterrupt ti_sysbios_family_c7x_Hwi_enableInterrupt
#define Hwi_restoreInterrupt ti_sysbios_family_c7x_Hwi_restoreInterrupt
#define Hwi_clearInterrupt ti_sysbios_family_c7x_Hwi_clearInterrupt
#define Hwi_getFunc ti_sysbios_family_c7x_Hwi_getFunc
#define Hwi_setFunc ti_sysbios_family_c7x_Hwi_setFunc
#define Hwi_getHookContext ti_sysbios_family_c7x_Hwi_getHookContext
#define Hwi_setHookContext ti_sysbios_family_c7x_Hwi_setHookContext
#define Hwi_getIrp ti_sysbios_family_c7x_Hwi_getIrp
#define Hwi_disable ti_sysbios_family_c7x_Hwi_disable
#define Hwi_enable ti_sysbios_family_c7x_Hwi_enable
#define Hwi_restore ti_sysbios_family_c7x_Hwi_restore
#define Hwi_eventMap ti_sysbios_family_c7x_Hwi_eventMap
#define Hwi_plug ti_sysbios_family_c7x_Hwi_plug
#define Hwi_getEventId ti_sysbios_family_c7x_Hwi_getEventId
#define Hwi_getHandle ti_sysbios_family_c7x_Hwi_getHandle
#define Hwi_disableIER ti_sysbios_family_c7x_Hwi_disableIER
#define Hwi_enableIER ti_sysbios_family_c7x_Hwi_enableIER
#define Hwi_restoreIER ti_sysbios_family_c7x_Hwi_restoreIER
#define Hwi_setPriority ti_sysbios_family_c7x_Hwi_setPriority
#define Hwi_setCOP ti_sysbios_family_c7x_Hwi_setCOP
#define Hwi_reconfig ti_sysbios_family_c7x_Hwi_reconfig
#define Hwi_Module_name ti_sysbios_family_c7x_Hwi_Module_name
#define Hwi_Module_id ti_sysbios_family_c7x_Hwi_Module_id
#define Hwi_Module_startup ti_sysbios_family_c7x_Hwi_Module_startup
#define Hwi_Module_startupDone ti_sysbios_family_c7x_Hwi_Module_startupDone
#define Hwi_Module_hasMask ti_sysbios_family_c7x_Hwi_Module_hasMask
#define Hwi_Module_getMask ti_sysbios_family_c7x_Hwi_Module_getMask
#define Hwi_Module_setMask ti_sysbios_family_c7x_Hwi_Module_setMask
#define Hwi_Object_heap ti_sysbios_family_c7x_Hwi_Object_heap
#define Hwi_Module_heap ti_sysbios_family_c7x_Hwi_Module_heap
#define Hwi_construct ti_sysbios_family_c7x_Hwi_construct
#define Hwi_create ti_sysbios_family_c7x_Hwi_create
#define Hwi_handle ti_sysbios_family_c7x_Hwi_handle
#define Hwi_struct ti_sysbios_family_c7x_Hwi_struct
#define Hwi_Handle_label ti_sysbios_family_c7x_Hwi_Handle_label
#define Hwi_Handle_name ti_sysbios_family_c7x_Hwi_Handle_name
#define Hwi_Instance_init ti_sysbios_family_c7x_Hwi_Instance_init
#define Hwi_Object_count ti_sysbios_family_c7x_Hwi_Object_count
#define Hwi_Object_get ti_sysbios_family_c7x_Hwi_Object_get
#define Hwi_Object_first ti_sysbios_family_c7x_Hwi_Object_first
#define Hwi_Object_next ti_sysbios_family_c7x_Hwi_Object_next
#define Hwi_Object_sizeof ti_sysbios_family_c7x_Hwi_Object_sizeof
#define Hwi_Params_copy ti_sysbios_family_c7x_Hwi_Params_copy
#define Hwi_Params_init ti_sysbios_family_c7x_Hwi_Params_init
#define Hwi_Instance_finalize ti_sysbios_family_c7x_Hwi_Instance_finalize
#define Hwi_delete ti_sysbios_family_c7x_Hwi_delete
#define Hwi_destruct ti_sysbios_family_c7x_Hwi_destruct
#define Hwi_Module_upCast ti_sysbios_family_c7x_Hwi_Module_upCast
#define Hwi_Module_to_ti_sysbios_interfaces_IHwi ti_sysbios_family_c7x_Hwi_Module_to_ti_sysbios_interfaces_IHwi
#define Hwi_Handle_upCast ti_sysbios_family_c7x_Hwi_Handle_upCast
#define Hwi_Handle_to_ti_sysbios_interfaces_IHwi ti_sysbios_family_c7x_Hwi_Handle_to_ti_sysbios_interfaces_IHwi
#define Hwi_Handle_downCast ti_sysbios_family_c7x_Hwi_Handle_downCast
#define Hwi_Handle_from_ti_sysbios_interfaces_IHwi ti_sysbios_family_c7x_Hwi_Handle_from_ti_sysbios_interfaces_IHwi

#endif /* ti_sysbios_family_c7x_Hwi__localnames__done */
#endif
