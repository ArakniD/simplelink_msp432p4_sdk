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

#ifndef ti_sysbios_timers_rti_Timer__include
#define ti_sysbios_timers_rti_Timer__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_timers_rti_Timer__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_timers_rti_Timer___VERS 200


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/IInstance.h>
#include <ti/sysbios/timers/rti/package/package.defs.h>

#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/interfaces/ITimer.h>
#include <ti/sysbios/hal/Hwi.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* FuncPtr */
typedef ti_sysbios_interfaces_ITimer_FuncPtr ti_sysbios_timers_rti_Timer_FuncPtr;

/* ANY */
#define ti_sysbios_timers_rti_Timer_ANY (~0)

/* StartMode */
typedef ti_sysbios_interfaces_ITimer_StartMode ti_sysbios_timers_rti_Timer_StartMode;

/* RunMode */
typedef ti_sysbios_interfaces_ITimer_RunMode ti_sysbios_timers_rti_Timer_RunMode;

/* Status */
typedef ti_sysbios_interfaces_ITimer_Status ti_sysbios_timers_rti_Timer_Status;

/* PeriodType */
typedef ti_sysbios_interfaces_ITimer_PeriodType ti_sysbios_timers_rti_Timer_PeriodType;

/* MAX_PERIOD */
#define ti_sysbios_timers_rti_Timer_MAX_PERIOD (0xffffffff)

/* MIN_SWEEP_PERIOD */
#define ti_sysbios_timers_rti_Timer_MIN_SWEEP_PERIOD (8)

/* DeviceRegs */
struct ti_sysbios_timers_rti_Timer_DeviceRegs {
    xdc_UInt32 RTIGCTRL;
    xdc_UInt32 RTITBCTRL;
    xdc_UInt32 RTICAPCTRL;
    xdc_UInt32 RTICOMPCTRL;
    xdc_UInt32 RTIFRC0;
    xdc_UInt32 RTIUC0;
    xdc_UInt32 RTICPUC0;
    xdc_UInt32 RESERVED0;
    xdc_UInt32 RTICAFRC0;
    xdc_UInt32 RTICAUC0;
    xdc_UInt32 RESERVED1;
    xdc_UInt32 RESERVED2;
    xdc_UInt32 RTIFRC1;
    xdc_UInt32 RTIUC1;
    xdc_UInt32 RTICPUC1;
    xdc_UInt32 RESERVED3;
    xdc_UInt32 RTICAFRC1;
    xdc_UInt32 RTICAUC1;
    xdc_UInt32 RESERVED4;
    xdc_UInt32 RESERVED5;
    xdc_UInt32 RTICOMP0;
    xdc_UInt32 RTIUDCP0;
    xdc_UInt32 RTICOMP1;
    xdc_UInt32 RTIUDCP1;
    xdc_UInt32 RTICOMP2;
    xdc_UInt32 RTIUDCP2;
    xdc_UInt32 RTICOMP3;
    xdc_UInt32 RTIUDCP3;
    xdc_UInt32 RTITBLCOMP;
    xdc_UInt32 RTITBLHCOMP;
    xdc_UInt32 RESERVED6;
    xdc_UInt32 RESERVED7;
    xdc_UInt32 RTISETINTENA;
    xdc_UInt32 RTICLEARINTENA;
    xdc_UInt32 RTIINTFLAG;
    xdc_UInt32 RESERVED8;
    xdc_UInt32 RTIDWDCTRL;
    xdc_UInt32 RTIDWDPRLD;
    xdc_UInt32 RTIWDSTATUS;
    xdc_UInt32 RTIWDKEY;
    xdc_UInt32 RTIDWDCNTR;
    xdc_UInt32 RTIWWDRXNCTRL;
    xdc_UInt32 RTIWWDSIZECTRL;
    xdc_UInt32 RTIINTCLRENABLE;
    xdc_UInt32 RTICOMP0CLR;
    xdc_UInt32 RTICOMP1CLR;
    xdc_UInt32 RTICOMP2CLR;
    xdc_UInt32 RTICOMP3CLR;
};

/* StartMode_AUTO */
#define ti_sysbios_timers_rti_Timer_StartMode_AUTO ti_sysbios_interfaces_ITimer_StartMode_AUTO

/* StartMode_USER */
#define ti_sysbios_timers_rti_Timer_StartMode_USER ti_sysbios_interfaces_ITimer_StartMode_USER

/* RunMode_CONTINUOUS */
#define ti_sysbios_timers_rti_Timer_RunMode_CONTINUOUS ti_sysbios_interfaces_ITimer_RunMode_CONTINUOUS

/* RunMode_ONESHOT */
#define ti_sysbios_timers_rti_Timer_RunMode_ONESHOT ti_sysbios_interfaces_ITimer_RunMode_ONESHOT

/* RunMode_DYNAMIC */
#define ti_sysbios_timers_rti_Timer_RunMode_DYNAMIC ti_sysbios_interfaces_ITimer_RunMode_DYNAMIC

/* Status_INUSE */
#define ti_sysbios_timers_rti_Timer_Status_INUSE ti_sysbios_interfaces_ITimer_Status_INUSE

/* Status_FREE */
#define ti_sysbios_timers_rti_Timer_Status_FREE ti_sysbios_interfaces_ITimer_Status_FREE

/* PeriodType_MICROSECS */
#define ti_sysbios_timers_rti_Timer_PeriodType_MICROSECS ti_sysbios_interfaces_ITimer_PeriodType_MICROSECS

/* PeriodType_COUNTS */
#define ti_sysbios_timers_rti_Timer_PeriodType_COUNTS ti_sysbios_interfaces_ITimer_PeriodType_COUNTS


/*
 * ======== CREATE ARGS ========
 */

/* Args__create */
typedef struct ti_sysbios_timers_rti_Timer_Args__create {
    xdc_Int id;
    ti_sysbios_interfaces_ITimer_FuncPtr tickFxn;
} ti_sysbios_timers_rti_Timer_Args__create;


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* TimerDevice */
struct ti_sysbios_timers_rti_Timer_TimerDevice {
    xdc_Int intNum;
    xdc_Int eventId;
    xdc_Ptr baseAddr;
};

/* Module_State */
typedef ti_sysbios_timers_rti_Timer_TimerDevice __T1_ti_sysbios_timers_rti_Timer_Module_State__device;
typedef ti_sysbios_timers_rti_Timer_TimerDevice *ARRAY1_ti_sysbios_timers_rti_Timer_Module_State__device;
typedef const ti_sysbios_timers_rti_Timer_TimerDevice *CARRAY1_ti_sysbios_timers_rti_Timer_Module_State__device;
typedef ARRAY1_ti_sysbios_timers_rti_Timer_Module_State__device __TA_ti_sysbios_timers_rti_Timer_Module_State__device;
typedef xdc_runtime_Types_FreqHz __T1_ti_sysbios_timers_rti_Timer_Module_State__intFreqs;
typedef xdc_runtime_Types_FreqHz *ARRAY1_ti_sysbios_timers_rti_Timer_Module_State__intFreqs;
typedef const xdc_runtime_Types_FreqHz *CARRAY1_ti_sysbios_timers_rti_Timer_Module_State__intFreqs;
typedef ARRAY1_ti_sysbios_timers_rti_Timer_Module_State__intFreqs __TA_ti_sysbios_timers_rti_Timer_Module_State__intFreqs;
typedef ti_sysbios_timers_rti_Timer_Handle __T1_ti_sysbios_timers_rti_Timer_Module_State__handles;
typedef ti_sysbios_timers_rti_Timer_Handle *ARRAY1_ti_sysbios_timers_rti_Timer_Module_State__handles;
typedef const ti_sysbios_timers_rti_Timer_Handle *CARRAY1_ti_sysbios_timers_rti_Timer_Module_State__handles;
typedef ARRAY1_ti_sysbios_timers_rti_Timer_Module_State__handles __TA_ti_sysbios_timers_rti_Timer_Module_State__handles;


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_timers_rti_Timer_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__diagsEnabled ti_sysbios_timers_rti_Timer_Module__diagsEnabled__C;
#ifdef ti_sysbios_timers_rti_Timer_Module__diagsEnabled__CR
#define ti_sysbios_timers_rti_Timer_Module__diagsEnabled__C (*((CT__ti_sysbios_timers_rti_Timer_Module__diagsEnabled*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__diagsEnabled__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Module__diagsEnabled (ti_sysbios_timers_rti_Timer_Module__diagsEnabled__C)
#endif

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_timers_rti_Timer_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__diagsIncluded ti_sysbios_timers_rti_Timer_Module__diagsIncluded__C;
#ifdef ti_sysbios_timers_rti_Timer_Module__diagsIncluded__CR
#define ti_sysbios_timers_rti_Timer_Module__diagsIncluded__C (*((CT__ti_sysbios_timers_rti_Timer_Module__diagsIncluded*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__diagsIncluded__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Module__diagsIncluded (ti_sysbios_timers_rti_Timer_Module__diagsIncluded__C)
#endif

/* Module__diagsMask */
typedef xdc_Bits16 *CT__ti_sysbios_timers_rti_Timer_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__diagsMask ti_sysbios_timers_rti_Timer_Module__diagsMask__C;
#ifdef ti_sysbios_timers_rti_Timer_Module__diagsMask__CR
#define ti_sysbios_timers_rti_Timer_Module__diagsMask__C (*((CT__ti_sysbios_timers_rti_Timer_Module__diagsMask*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__diagsMask__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Module__diagsMask (ti_sysbios_timers_rti_Timer_Module__diagsMask__C)
#endif

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_timers_rti_Timer_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__gateObj ti_sysbios_timers_rti_Timer_Module__gateObj__C;
#ifdef ti_sysbios_timers_rti_Timer_Module__gateObj__CR
#define ti_sysbios_timers_rti_Timer_Module__gateObj__C (*((CT__ti_sysbios_timers_rti_Timer_Module__gateObj*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__gateObj__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Module__gateObj (ti_sysbios_timers_rti_Timer_Module__gateObj__C)
#endif

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_timers_rti_Timer_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__gatePrms ti_sysbios_timers_rti_Timer_Module__gatePrms__C;
#ifdef ti_sysbios_timers_rti_Timer_Module__gatePrms__CR
#define ti_sysbios_timers_rti_Timer_Module__gatePrms__C (*((CT__ti_sysbios_timers_rti_Timer_Module__gatePrms*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__gatePrms__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Module__gatePrms (ti_sysbios_timers_rti_Timer_Module__gatePrms__C)
#endif

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_timers_rti_Timer_Module__id;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__id ti_sysbios_timers_rti_Timer_Module__id__C;
#ifdef ti_sysbios_timers_rti_Timer_Module__id__CR
#define ti_sysbios_timers_rti_Timer_Module__id__C (*((CT__ti_sysbios_timers_rti_Timer_Module__id*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__id__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Module__id (ti_sysbios_timers_rti_Timer_Module__id__C)
#endif

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_timers_rti_Timer_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerDefined ti_sysbios_timers_rti_Timer_Module__loggerDefined__C;
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerDefined__CR
#define ti_sysbios_timers_rti_Timer_Module__loggerDefined__C (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerDefined*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerDefined__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Module__loggerDefined (ti_sysbios_timers_rti_Timer_Module__loggerDefined__C)
#endif

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_timers_rti_Timer_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerObj ti_sysbios_timers_rti_Timer_Module__loggerObj__C;
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerObj__CR
#define ti_sysbios_timers_rti_Timer_Module__loggerObj__C (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerObj*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerObj__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Module__loggerObj (ti_sysbios_timers_rti_Timer_Module__loggerObj__C)
#endif

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn0 ti_sysbios_timers_rti_Timer_Module__loggerFxn0__C;
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerFxn0__CR
#define ti_sysbios_timers_rti_Timer_Module__loggerFxn0__C (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn0*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerFxn0__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Module__loggerFxn0 (ti_sysbios_timers_rti_Timer_Module__loggerFxn0__C)
#endif

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn1 ti_sysbios_timers_rti_Timer_Module__loggerFxn1__C;
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerFxn1__CR
#define ti_sysbios_timers_rti_Timer_Module__loggerFxn1__C (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn1*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerFxn1__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Module__loggerFxn1 (ti_sysbios_timers_rti_Timer_Module__loggerFxn1__C)
#endif

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn2 ti_sysbios_timers_rti_Timer_Module__loggerFxn2__C;
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerFxn2__CR
#define ti_sysbios_timers_rti_Timer_Module__loggerFxn2__C (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn2*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerFxn2__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Module__loggerFxn2 (ti_sysbios_timers_rti_Timer_Module__loggerFxn2__C)
#endif

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn4 ti_sysbios_timers_rti_Timer_Module__loggerFxn4__C;
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerFxn4__CR
#define ti_sysbios_timers_rti_Timer_Module__loggerFxn4__C (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn4*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerFxn4__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Module__loggerFxn4 (ti_sysbios_timers_rti_Timer_Module__loggerFxn4__C)
#endif

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn8 ti_sysbios_timers_rti_Timer_Module__loggerFxn8__C;
#ifdef ti_sysbios_timers_rti_Timer_Module__loggerFxn8__CR
#define ti_sysbios_timers_rti_Timer_Module__loggerFxn8__C (*((CT__ti_sysbios_timers_rti_Timer_Module__loggerFxn8*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__loggerFxn8__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Module__loggerFxn8 (ti_sysbios_timers_rti_Timer_Module__loggerFxn8__C)
#endif

/* Object__count */
typedef xdc_Int CT__ti_sysbios_timers_rti_Timer_Object__count;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Object__count ti_sysbios_timers_rti_Timer_Object__count__C;
#ifdef ti_sysbios_timers_rti_Timer_Object__count__CR
#define ti_sysbios_timers_rti_Timer_Object__count__C (*((CT__ti_sysbios_timers_rti_Timer_Object__count*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Object__count__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Object__count (ti_sysbios_timers_rti_Timer_Object__count__C)
#endif

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_timers_rti_Timer_Object__heap;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Object__heap ti_sysbios_timers_rti_Timer_Object__heap__C;
#ifdef ti_sysbios_timers_rti_Timer_Object__heap__CR
#define ti_sysbios_timers_rti_Timer_Object__heap__C (*((CT__ti_sysbios_timers_rti_Timer_Object__heap*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Object__heap__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Object__heap (ti_sysbios_timers_rti_Timer_Object__heap__C)
#endif

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_timers_rti_Timer_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Object__sizeof ti_sysbios_timers_rti_Timer_Object__sizeof__C;
#ifdef ti_sysbios_timers_rti_Timer_Object__sizeof__CR
#define ti_sysbios_timers_rti_Timer_Object__sizeof__C (*((CT__ti_sysbios_timers_rti_Timer_Object__sizeof*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Object__sizeof__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Object__sizeof (ti_sysbios_timers_rti_Timer_Object__sizeof__C)
#endif

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_timers_rti_Timer_Object__table;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_Object__table ti_sysbios_timers_rti_Timer_Object__table__C;
#ifdef ti_sysbios_timers_rti_Timer_Object__table__CR
#define ti_sysbios_timers_rti_Timer_Object__table__C (*((CT__ti_sysbios_timers_rti_Timer_Object__table*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Object__table__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_Object__table (ti_sysbios_timers_rti_Timer_Object__table__C)
#endif

/* A_invalidTimer */
typedef xdc_runtime_Assert_Id CT__ti_sysbios_timers_rti_Timer_A_invalidTimer;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_A_invalidTimer ti_sysbios_timers_rti_Timer_A_invalidTimer__C;
#ifdef ti_sysbios_timers_rti_Timer_A_invalidTimer__CR
#define ti_sysbios_timers_rti_Timer_A_invalidTimer (*((CT__ti_sysbios_timers_rti_Timer_A_invalidTimer*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_A_invalidTimer__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_A_invalidTimer (ti_sysbios_timers_rti_Timer_A_invalidTimer__C)
#endif

/* E_invalidTimer */
typedef xdc_runtime_Error_Id CT__ti_sysbios_timers_rti_Timer_E_invalidTimer;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_E_invalidTimer ti_sysbios_timers_rti_Timer_E_invalidTimer__C;
#ifdef ti_sysbios_timers_rti_Timer_E_invalidTimer__CR
#define ti_sysbios_timers_rti_Timer_E_invalidTimer (*((CT__ti_sysbios_timers_rti_Timer_E_invalidTimer*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_E_invalidTimer__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_E_invalidTimer (ti_sysbios_timers_rti_Timer_E_invalidTimer__C)
#endif

/* E_notAvailable */
typedef xdc_runtime_Error_Id CT__ti_sysbios_timers_rti_Timer_E_notAvailable;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_E_notAvailable ti_sysbios_timers_rti_Timer_E_notAvailable__C;
#ifdef ti_sysbios_timers_rti_Timer_E_notAvailable__CR
#define ti_sysbios_timers_rti_Timer_E_notAvailable (*((CT__ti_sysbios_timers_rti_Timer_E_notAvailable*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_E_notAvailable__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_E_notAvailable (ti_sysbios_timers_rti_Timer_E_notAvailable__C)
#endif

/* E_invalidHwiMask */
typedef xdc_runtime_Error_Id CT__ti_sysbios_timers_rti_Timer_E_invalidHwiMask;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_E_invalidHwiMask ti_sysbios_timers_rti_Timer_E_invalidHwiMask__C;
#ifdef ti_sysbios_timers_rti_Timer_E_invalidHwiMask__CR
#define ti_sysbios_timers_rti_Timer_E_invalidHwiMask (*((CT__ti_sysbios_timers_rti_Timer_E_invalidHwiMask*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_E_invalidHwiMask__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_E_invalidHwiMask (ti_sysbios_timers_rti_Timer_E_invalidHwiMask__C)
#endif

/* E_cannotSupport */
typedef xdc_runtime_Error_Id CT__ti_sysbios_timers_rti_Timer_E_cannotSupport;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_E_cannotSupport ti_sysbios_timers_rti_Timer_E_cannotSupport__C;
#ifdef ti_sysbios_timers_rti_Timer_E_cannotSupport__CR
#define ti_sysbios_timers_rti_Timer_E_cannotSupport (*((CT__ti_sysbios_timers_rti_Timer_E_cannotSupport*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_E_cannotSupport__C_offset)))
#else
#define ti_sysbios_timers_rti_Timer_E_cannotSupport (ti_sysbios_timers_rti_Timer_E_cannotSupport__C)
#endif

/* anyMask */
typedef xdc_UInt CT__ti_sysbios_timers_rti_Timer_anyMask;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_anyMask ti_sysbios_timers_rti_Timer_anyMask__C;
#ifdef ti_sysbios_timers_rti_Timer_anyMask__CR
#define ti_sysbios_timers_rti_Timer_anyMask (*((CT__ti_sysbios_timers_rti_Timer_anyMask*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_anyMask__C_offset)))
#else
#ifdef ti_sysbios_timers_rti_Timer_anyMask__D
#define ti_sysbios_timers_rti_Timer_anyMask (ti_sysbios_timers_rti_Timer_anyMask__D)
#else
#define ti_sysbios_timers_rti_Timer_anyMask (ti_sysbios_timers_rti_Timer_anyMask__C)
#endif
#endif

/* continueOnSuspend */
typedef xdc_Bool CT__ti_sysbios_timers_rti_Timer_continueOnSuspend;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_continueOnSuspend ti_sysbios_timers_rti_Timer_continueOnSuspend__C;
#ifdef ti_sysbios_timers_rti_Timer_continueOnSuspend__CR
#define ti_sysbios_timers_rti_Timer_continueOnSuspend (*((CT__ti_sysbios_timers_rti_Timer_continueOnSuspend*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_continueOnSuspend__C_offset)))
#else
#ifdef ti_sysbios_timers_rti_Timer_continueOnSuspend__D
#define ti_sysbios_timers_rti_Timer_continueOnSuspend (ti_sysbios_timers_rti_Timer_continueOnSuspend__D)
#else
#define ti_sysbios_timers_rti_Timer_continueOnSuspend (ti_sysbios_timers_rti_Timer_continueOnSuspend__C)
#endif
#endif

/* startupNeeded */
typedef xdc_UInt CT__ti_sysbios_timers_rti_Timer_startupNeeded;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_startupNeeded ti_sysbios_timers_rti_Timer_startupNeeded__C;
#ifdef ti_sysbios_timers_rti_Timer_startupNeeded__CR
#define ti_sysbios_timers_rti_Timer_startupNeeded (*((CT__ti_sysbios_timers_rti_Timer_startupNeeded*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_startupNeeded__C_offset)))
#else
#ifdef ti_sysbios_timers_rti_Timer_startupNeeded__D
#define ti_sysbios_timers_rti_Timer_startupNeeded (ti_sysbios_timers_rti_Timer_startupNeeded__D)
#else
#define ti_sysbios_timers_rti_Timer_startupNeeded (ti_sysbios_timers_rti_Timer_startupNeeded__C)
#endif
#endif

/* numTimerDevices */
typedef xdc_Int CT__ti_sysbios_timers_rti_Timer_numTimerDevices;
__extern __FAR__ const CT__ti_sysbios_timers_rti_Timer_numTimerDevices ti_sysbios_timers_rti_Timer_numTimerDevices__C;
#ifdef ti_sysbios_timers_rti_Timer_numTimerDevices__CR
#define ti_sysbios_timers_rti_Timer_numTimerDevices (*((CT__ti_sysbios_timers_rti_Timer_numTimerDevices*)(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_numTimerDevices__C_offset)))
#else
#ifdef ti_sysbios_timers_rti_Timer_numTimerDevices__D
#define ti_sysbios_timers_rti_Timer_numTimerDevices (ti_sysbios_timers_rti_Timer_numTimerDevices__D)
#else
#define ti_sysbios_timers_rti_Timer_numTimerDevices (ti_sysbios_timers_rti_Timer_numTimerDevices__C)
#endif
#endif


/*
 * ======== PER-INSTANCE TYPES ========
 */

/* Params */
struct ti_sysbios_timers_rti_Timer_Params {
    size_t __size;
    const void *__self;
    void *__fxns;
    xdc_runtime_IInstance_Params *instance;
    ti_sysbios_interfaces_ITimer_RunMode runMode;
    ti_sysbios_interfaces_ITimer_StartMode startMode;
    xdc_UArg arg;
    xdc_UInt32 period;
    ti_sysbios_interfaces_ITimer_PeriodType periodType;
    xdc_runtime_Types_FreqHz extFreq;
    xdc_Bool createHwi;
    ti_sysbios_hal_Hwi_Params *hwiParams;
    xdc_UInt8 prescale;
    xdc_runtime_IInstance_Params __iprms;
};

/* Struct */
struct ti_sysbios_timers_rti_Timer_Struct {
    const ti_sysbios_timers_rti_Timer_Fxns__ *__fxns;
    xdc_Bool f0;
    xdc_Int f1;
    ti_sysbios_interfaces_ITimer_RunMode f2;
    ti_sysbios_interfaces_ITimer_StartMode f3;
    xdc_UInt f4;
    xdc_UInt f5;
    ti_sysbios_interfaces_ITimer_PeriodType f6;
    xdc_UInt f7;
    xdc_UArg f8;
    ti_sysbios_hal_Hwi_FuncPtr f9;
    xdc_runtime_Types_FreqHz f10;
    ti_sysbios_hal_Hwi_Handle f11;
    xdc_Bool f12;
    xdc_runtime_Types_CordAddr __name;
};


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_sysbios_timers_rti_Timer_Fxns__ {
    const xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2* __sysp;
    xdc_UInt (*getNumTimers)(void);
    ti_sysbios_interfaces_ITimer_Status (*getStatus)(xdc_UInt id);
    xdc_Void (*startup)(void);
    xdc_UInt32 (*getMaxTicks)(ti_sysbios_timers_rti_Timer_Handle inst);
    xdc_Void (*setNextTick)(ti_sysbios_timers_rti_Timer_Handle inst, xdc_UInt32 ticks);
    xdc_Void (*start)(ti_sysbios_timers_rti_Timer_Handle inst);
    xdc_Void (*stop)(ti_sysbios_timers_rti_Timer_Handle inst);
    xdc_Void (*setPeriod)(ti_sysbios_timers_rti_Timer_Handle inst, xdc_UInt32 period);
    xdc_Bool (*setPeriodMicroSecs)(ti_sysbios_timers_rti_Timer_Handle inst, xdc_UInt32 microsecs);
    xdc_UInt32 (*getPeriod)(ti_sysbios_timers_rti_Timer_Handle inst);
    xdc_UInt32 (*getCount)(ti_sysbios_timers_rti_Timer_Handle inst);
    xdc_Void (*getFreq)(ti_sysbios_timers_rti_Timer_Handle inst, xdc_runtime_Types_FreqHz* freq);
    ti_sysbios_interfaces_ITimer_FuncPtr (*getFunc)(ti_sysbios_timers_rti_Timer_Handle inst, xdc_UArg* arg);
    xdc_Void (*setFunc)(ti_sysbios_timers_rti_Timer_Handle inst, ti_sysbios_interfaces_ITimer_FuncPtr fxn, xdc_UArg arg);
    xdc_Void (*trigger)(ti_sysbios_timers_rti_Timer_Handle inst, xdc_UInt32 cycles);
    xdc_UInt32 (*getExpiredCounts)(ti_sysbios_timers_rti_Timer_Handle inst);
    xdc_UInt32 (*getExpiredTicks)(ti_sysbios_timers_rti_Timer_Handle inst, xdc_UInt32 tickPeriod);
    xdc_UInt32 (*getCurrentTick)(ti_sysbios_timers_rti_Timer_Handle inst, xdc_Bool save);
    xdc_runtime_Types_SysFxns2 __sfxns;
};
#ifndef ti_sysbios_timers_rti_Timer_Module__FXNS__CR

/* Module__FXNS__C */
__extern const ti_sysbios_timers_rti_Timer_Fxns__ ti_sysbios_timers_rti_Timer_Module__FXNS__C;
#else
#define ti_sysbios_timers_rti_Timer_Module__FXNS__C (*(xdcRomConstPtr + ti_sysbios_timers_rti_Timer_Module__FXNS__C_offset))
#endif


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_timers_rti_Timer_Module_startup ti_sysbios_timers_rti_Timer_Module_startup__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_Module_startup__E, "ti_sysbios_timers_rti_Timer_Module_startup")
__extern xdc_Int ti_sysbios_timers_rti_Timer_Module_startup__E( xdc_Int state );
xdc__CODESECT(ti_sysbios_timers_rti_Timer_Module_startup__F, "ti_sysbios_timers_rti_Timer_Module_startup")
__extern xdc_Int ti_sysbios_timers_rti_Timer_Module_startup__F( xdc_Int state );

/* Instance_init__E */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_Instance_init__E, "ti_sysbios_timers_rti_Timer_Instance_init")
__extern xdc_Int ti_sysbios_timers_rti_Timer_Instance_init__E(ti_sysbios_timers_rti_Timer_Object *obj, xdc_Int id, ti_sysbios_interfaces_ITimer_FuncPtr tickFxn, const ti_sysbios_timers_rti_Timer_Params *prms, xdc_runtime_Error_Block *eb);

/* Instance_finalize__E */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_Instance_finalize__E, "ti_sysbios_timers_rti_Timer_Instance_finalize")
__extern void ti_sysbios_timers_rti_Timer_Instance_finalize__E(ti_sysbios_timers_rti_Timer_Object *obj, int ec);

/* create */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_create, "ti_sysbios_timers_rti_Timer_create")
__extern ti_sysbios_timers_rti_Timer_Handle ti_sysbios_timers_rti_Timer_create( xdc_Int id, ti_sysbios_interfaces_ITimer_FuncPtr tickFxn, const ti_sysbios_timers_rti_Timer_Params *prms, xdc_runtime_Error_Block *eb);

/* construct */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_construct, "ti_sysbios_timers_rti_Timer_construct")
__extern void ti_sysbios_timers_rti_Timer_construct(ti_sysbios_timers_rti_Timer_Struct *obj, xdc_Int id, ti_sysbios_interfaces_ITimer_FuncPtr tickFxn, const ti_sysbios_timers_rti_Timer_Params *prms, xdc_runtime_Error_Block *eb);

/* delete */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_delete, "ti_sysbios_timers_rti_Timer_delete")
__extern void ti_sysbios_timers_rti_Timer_delete(ti_sysbios_timers_rti_Timer_Handle *instp);

/* destruct */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_destruct, "ti_sysbios_timers_rti_Timer_destruct")
__extern void ti_sysbios_timers_rti_Timer_destruct(ti_sysbios_timers_rti_Timer_Struct *obj);

/* Handle__label__S */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_Handle__label__S, "ti_sysbios_timers_rti_Timer_Handle__label__S")
__extern xdc_runtime_Types_Label *ti_sysbios_timers_rti_Timer_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label *lab );

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_Module__startupDone__S, "ti_sysbios_timers_rti_Timer_Module__startupDone__S")
__extern xdc_Bool ti_sysbios_timers_rti_Timer_Module__startupDone__S( void );

/* Object__create__S */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_Object__create__S, "ti_sysbios_timers_rti_Timer_Object__create__S")
__extern xdc_Ptr ti_sysbios_timers_rti_Timer_Object__create__S( xdc_CPtr aa, const xdc_UChar *pa, xdc_SizeT psz, xdc_runtime_Error_Block *eb );

/* Object__delete__S */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_Object__delete__S, "ti_sysbios_timers_rti_Timer_Object__delete__S")
__extern xdc_Void ti_sysbios_timers_rti_Timer_Object__delete__S( xdc_Ptr instp );

/* Object__get__S */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_Object__get__S, "ti_sysbios_timers_rti_Timer_Object__get__S")
__extern xdc_Ptr ti_sysbios_timers_rti_Timer_Object__get__S( xdc_Ptr oarr, xdc_Int i );

/* Object__first__S */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_Object__first__S, "ti_sysbios_timers_rti_Timer_Object__first__S")
__extern xdc_Ptr ti_sysbios_timers_rti_Timer_Object__first__S( void );

/* Object__next__S */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_Object__next__S, "ti_sysbios_timers_rti_Timer_Object__next__S")
__extern xdc_Ptr ti_sysbios_timers_rti_Timer_Object__next__S( xdc_Ptr obj );

/* Params__init__S */
xdc__CODESECT(ti_sysbios_timers_rti_Timer_Params__init__S, "ti_sysbios_timers_rti_Timer_Params__init__S")
__extern xdc_Void ti_sysbios_timers_rti_Timer_Params__init__S( xdc_Ptr dst, const xdc_Void *src, xdc_SizeT psz, xdc_SizeT isz );

/* getNumTimers__E */
#define ti_sysbios_timers_rti_Timer_getNumTimers ti_sysbios_timers_rti_Timer_getNumTimers__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_getNumTimers__E, "ti_sysbios_timers_rti_Timer_getNumTimers")
__extern xdc_UInt ti_sysbios_timers_rti_Timer_getNumTimers__E( void );

/* getStatus__E */
#define ti_sysbios_timers_rti_Timer_getStatus ti_sysbios_timers_rti_Timer_getStatus__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_getStatus__E, "ti_sysbios_timers_rti_Timer_getStatus")
__extern ti_sysbios_interfaces_ITimer_Status ti_sysbios_timers_rti_Timer_getStatus__E( xdc_UInt id );

/* startup__E */
#define ti_sysbios_timers_rti_Timer_startup ti_sysbios_timers_rti_Timer_startup__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_startup__E, "ti_sysbios_timers_rti_Timer_startup")
__extern xdc_Void ti_sysbios_timers_rti_Timer_startup__E( void );

/* getMaxTicks__E */
#define ti_sysbios_timers_rti_Timer_getMaxTicks ti_sysbios_timers_rti_Timer_getMaxTicks__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_getMaxTicks__E, "ti_sysbios_timers_rti_Timer_getMaxTicks")
__extern xdc_UInt32 ti_sysbios_timers_rti_Timer_getMaxTicks__E( ti_sysbios_timers_rti_Timer_Handle __inst );

/* setNextTick__E */
#define ti_sysbios_timers_rti_Timer_setNextTick ti_sysbios_timers_rti_Timer_setNextTick__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_setNextTick__E, "ti_sysbios_timers_rti_Timer_setNextTick")
__extern xdc_Void ti_sysbios_timers_rti_Timer_setNextTick__E( ti_sysbios_timers_rti_Timer_Handle __inst, xdc_UInt32 ticks );

/* start__E */
#define ti_sysbios_timers_rti_Timer_start ti_sysbios_timers_rti_Timer_start__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_start__E, "ti_sysbios_timers_rti_Timer_start")
__extern xdc_Void ti_sysbios_timers_rti_Timer_start__E( ti_sysbios_timers_rti_Timer_Handle __inst );

/* stop__E */
#define ti_sysbios_timers_rti_Timer_stop ti_sysbios_timers_rti_Timer_stop__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_stop__E, "ti_sysbios_timers_rti_Timer_stop")
__extern xdc_Void ti_sysbios_timers_rti_Timer_stop__E( ti_sysbios_timers_rti_Timer_Handle __inst );

/* setPeriod__E */
#define ti_sysbios_timers_rti_Timer_setPeriod ti_sysbios_timers_rti_Timer_setPeriod__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_setPeriod__E, "ti_sysbios_timers_rti_Timer_setPeriod")
__extern xdc_Void ti_sysbios_timers_rti_Timer_setPeriod__E( ti_sysbios_timers_rti_Timer_Handle __inst, xdc_UInt32 period );

/* setPeriodMicroSecs__E */
#define ti_sysbios_timers_rti_Timer_setPeriodMicroSecs ti_sysbios_timers_rti_Timer_setPeriodMicroSecs__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_setPeriodMicroSecs__E, "ti_sysbios_timers_rti_Timer_setPeriodMicroSecs")
__extern xdc_Bool ti_sysbios_timers_rti_Timer_setPeriodMicroSecs__E( ti_sysbios_timers_rti_Timer_Handle __inst, xdc_UInt32 microsecs );

/* getPeriod__E */
#define ti_sysbios_timers_rti_Timer_getPeriod ti_sysbios_timers_rti_Timer_getPeriod__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_getPeriod__E, "ti_sysbios_timers_rti_Timer_getPeriod")
__extern xdc_UInt32 ti_sysbios_timers_rti_Timer_getPeriod__E( ti_sysbios_timers_rti_Timer_Handle __inst );

/* getCount__E */
#define ti_sysbios_timers_rti_Timer_getCount ti_sysbios_timers_rti_Timer_getCount__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_getCount__E, "ti_sysbios_timers_rti_Timer_getCount")
__extern xdc_UInt32 ti_sysbios_timers_rti_Timer_getCount__E( ti_sysbios_timers_rti_Timer_Handle __inst );

/* getFreq__E */
#define ti_sysbios_timers_rti_Timer_getFreq ti_sysbios_timers_rti_Timer_getFreq__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_getFreq__E, "ti_sysbios_timers_rti_Timer_getFreq")
__extern xdc_Void ti_sysbios_timers_rti_Timer_getFreq__E( ti_sysbios_timers_rti_Timer_Handle __inst, xdc_runtime_Types_FreqHz *freq );

/* getFunc__E */
#define ti_sysbios_timers_rti_Timer_getFunc ti_sysbios_timers_rti_Timer_getFunc__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_getFunc__E, "ti_sysbios_timers_rti_Timer_getFunc")
__extern ti_sysbios_interfaces_ITimer_FuncPtr ti_sysbios_timers_rti_Timer_getFunc__E( ti_sysbios_timers_rti_Timer_Handle __inst, xdc_UArg *arg );

/* setFunc__E */
#define ti_sysbios_timers_rti_Timer_setFunc ti_sysbios_timers_rti_Timer_setFunc__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_setFunc__E, "ti_sysbios_timers_rti_Timer_setFunc")
__extern xdc_Void ti_sysbios_timers_rti_Timer_setFunc__E( ti_sysbios_timers_rti_Timer_Handle __inst, ti_sysbios_interfaces_ITimer_FuncPtr fxn, xdc_UArg arg );

/* trigger__E */
#define ti_sysbios_timers_rti_Timer_trigger ti_sysbios_timers_rti_Timer_trigger__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_trigger__E, "ti_sysbios_timers_rti_Timer_trigger")
__extern xdc_Void ti_sysbios_timers_rti_Timer_trigger__E( ti_sysbios_timers_rti_Timer_Handle __inst, xdc_UInt32 cycles );

/* getExpiredCounts__E */
#define ti_sysbios_timers_rti_Timer_getExpiredCounts ti_sysbios_timers_rti_Timer_getExpiredCounts__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_getExpiredCounts__E, "ti_sysbios_timers_rti_Timer_getExpiredCounts")
__extern xdc_UInt32 ti_sysbios_timers_rti_Timer_getExpiredCounts__E( ti_sysbios_timers_rti_Timer_Handle __inst );

/* getExpiredTicks__E */
#define ti_sysbios_timers_rti_Timer_getExpiredTicks ti_sysbios_timers_rti_Timer_getExpiredTicks__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_getExpiredTicks__E, "ti_sysbios_timers_rti_Timer_getExpiredTicks")
__extern xdc_UInt32 ti_sysbios_timers_rti_Timer_getExpiredTicks__E( ti_sysbios_timers_rti_Timer_Handle __inst, xdc_UInt32 tickPeriod );

/* getCurrentTick__E */
#define ti_sysbios_timers_rti_Timer_getCurrentTick ti_sysbios_timers_rti_Timer_getCurrentTick__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_getCurrentTick__E, "ti_sysbios_timers_rti_Timer_getCurrentTick")
__extern xdc_UInt32 ti_sysbios_timers_rti_Timer_getCurrentTick__E( ti_sysbios_timers_rti_Timer_Handle __inst, xdc_Bool save );

/* oneShotStub__E */
#define ti_sysbios_timers_rti_Timer_oneShotStub ti_sysbios_timers_rti_Timer_oneShotStub__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_oneShotStub__E, "ti_sysbios_timers_rti_Timer_oneShotStub")
__extern xdc_Void ti_sysbios_timers_rti_Timer_oneShotStub__E( xdc_UArg arg );

/* periodicStub__E */
#define ti_sysbios_timers_rti_Timer_periodicStub ti_sysbios_timers_rti_Timer_periodicStub__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_periodicStub__E, "ti_sysbios_timers_rti_Timer_periodicStub")
__extern xdc_Void ti_sysbios_timers_rti_Timer_periodicStub__E( xdc_UArg arg );

/* getHandle__E */
#define ti_sysbios_timers_rti_Timer_getHandle ti_sysbios_timers_rti_Timer_getHandle__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_getHandle__E, "ti_sysbios_timers_rti_Timer_getHandle")
__extern ti_sysbios_timers_rti_Timer_Handle ti_sysbios_timers_rti_Timer_getHandle__E( xdc_UInt id );

/* reconfig__E */
#define ti_sysbios_timers_rti_Timer_reconfig ti_sysbios_timers_rti_Timer_reconfig__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_reconfig__E, "ti_sysbios_timers_rti_Timer_reconfig")
__extern xdc_Void ti_sysbios_timers_rti_Timer_reconfig__E( ti_sysbios_timers_rti_Timer_Handle __inst, ti_sysbios_timers_rti_Timer_FuncPtr tickFxn, const ti_sysbios_timers_rti_Timer_Params *timerParams, xdc_runtime_Error_Block *eb );

/* ackInterrupt__E */
#define ti_sysbios_timers_rti_Timer_ackInterrupt ti_sysbios_timers_rti_Timer_ackInterrupt__E
xdc__CODESECT(ti_sysbios_timers_rti_Timer_ackInterrupt__E, "ti_sysbios_timers_rti_Timer_ackInterrupt")
__extern xdc_Void ti_sysbios_timers_rti_Timer_ackInterrupt__E( ti_sysbios_timers_rti_Timer_Handle __inst );

/* initDevice__I */
#define ti_sysbios_timers_rti_Timer_initDevice ti_sysbios_timers_rti_Timer_initDevice__I
xdc__CODESECT(ti_sysbios_timers_rti_Timer_initDevice__I, "ti_sysbios_timers_rti_Timer_initDevice")
__extern xdc_Void ti_sysbios_timers_rti_Timer_initDevice__I( ti_sysbios_timers_rti_Timer_Object *timer );

/* postInit__I */
#define ti_sysbios_timers_rti_Timer_postInit ti_sysbios_timers_rti_Timer_postInit__I
xdc__CODESECT(ti_sysbios_timers_rti_Timer_postInit__I, "ti_sysbios_timers_rti_Timer_postInit")
__extern xdc_Int ti_sysbios_timers_rti_Timer_postInit__I( ti_sysbios_timers_rti_Timer_Object *timer, xdc_runtime_Error_Block *eb );

/* checkOverflow__I */
#define ti_sysbios_timers_rti_Timer_checkOverflow ti_sysbios_timers_rti_Timer_checkOverflow__I
xdc__CODESECT(ti_sysbios_timers_rti_Timer_checkOverflow__I, "ti_sysbios_timers_rti_Timer_checkOverflow")
__extern xdc_Bool ti_sysbios_timers_rti_Timer_checkOverflow__I( xdc_UInt32 a, xdc_UInt32 b );


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline ti_sysbios_interfaces_ITimer_Module ti_sysbios_timers_rti_Timer_Module_upCast(void);
static inline ti_sysbios_interfaces_ITimer_Module ti_sysbios_timers_rti_Timer_Module_upCast(void)
{
    return (ti_sysbios_interfaces_ITimer_Module)&ti_sysbios_timers_rti_Timer_Module__FXNS__C;
}

/* Module_to_ti_sysbios_interfaces_ITimer */
#define ti_sysbios_timers_rti_Timer_Module_to_ti_sysbios_interfaces_ITimer ti_sysbios_timers_rti_Timer_Module_upCast

/* Handle_upCast */
static inline ti_sysbios_interfaces_ITimer_Handle ti_sysbios_timers_rti_Timer_Handle_upCast(ti_sysbios_timers_rti_Timer_Handle i);
static inline ti_sysbios_interfaces_ITimer_Handle ti_sysbios_timers_rti_Timer_Handle_upCast(ti_sysbios_timers_rti_Timer_Handle i)
{
    return (ti_sysbios_interfaces_ITimer_Handle)i;
}

/* Handle_to_ti_sysbios_interfaces_ITimer */
#define ti_sysbios_timers_rti_Timer_Handle_to_ti_sysbios_interfaces_ITimer ti_sysbios_timers_rti_Timer_Handle_upCast

/* Handle_downCast */
static inline ti_sysbios_timers_rti_Timer_Handle ti_sysbios_timers_rti_Timer_Handle_downCast(ti_sysbios_interfaces_ITimer_Handle i);
static inline ti_sysbios_timers_rti_Timer_Handle ti_sysbios_timers_rti_Timer_Handle_downCast(ti_sysbios_interfaces_ITimer_Handle i)
{
    ti_sysbios_interfaces_ITimer_Handle i2 = (ti_sysbios_interfaces_ITimer_Handle)i;
    return ((const void*)i2->__fxns == (const void*)&ti_sysbios_timers_rti_Timer_Module__FXNS__C) ? (ti_sysbios_timers_rti_Timer_Handle)i : (ti_sysbios_timers_rti_Timer_Handle)NULL;
}

/* Handle_from_ti_sysbios_interfaces_ITimer */
#define ti_sysbios_timers_rti_Timer_Handle_from_ti_sysbios_interfaces_ITimer ti_sysbios_timers_rti_Timer_Handle_downCast


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_timers_rti_Timer_Module_startupDone() ti_sysbios_timers_rti_Timer_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_timers_rti_Timer_Object_heap() ti_sysbios_timers_rti_Timer_Object__heap__C

/* Module_heap */
#define ti_sysbios_timers_rti_Timer_Module_heap() ti_sysbios_timers_rti_Timer_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_timers_rti_Timer_Module__id ti_sysbios_timers_rti_Timer_Module_id(void);
static inline CT__ti_sysbios_timers_rti_Timer_Module__id ti_sysbios_timers_rti_Timer_Module_id( void ) 
{
    return ti_sysbios_timers_rti_Timer_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_timers_rti_Timer_Module_hasMask(void);
static inline xdc_Bool ti_sysbios_timers_rti_Timer_Module_hasMask(void)
{
    return (xdc_Bool)(ti_sysbios_timers_rti_Timer_Module__diagsMask__C != (CT__ti_sysbios_timers_rti_Timer_Module__diagsMask)NULL);
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_timers_rti_Timer_Module_getMask(void);
static inline xdc_Bits16 ti_sysbios_timers_rti_Timer_Module_getMask(void)
{
    return (ti_sysbios_timers_rti_Timer_Module__diagsMask__C != (CT__ti_sysbios_timers_rti_Timer_Module__diagsMask)NULL) ? *ti_sysbios_timers_rti_Timer_Module__diagsMask__C : (xdc_Bits16)0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_timers_rti_Timer_Module_setMask(xdc_Bits16 mask);
static inline xdc_Void ti_sysbios_timers_rti_Timer_Module_setMask(xdc_Bits16 mask)
{
    if (ti_sysbios_timers_rti_Timer_Module__diagsMask__C != (CT__ti_sysbios_timers_rti_Timer_Module__diagsMask)NULL) {
        *ti_sysbios_timers_rti_Timer_Module__diagsMask__C = mask;
    }
}

/* Params_init */
static inline void ti_sysbios_timers_rti_Timer_Params_init(ti_sysbios_timers_rti_Timer_Params *prms);
static inline void ti_sysbios_timers_rti_Timer_Params_init( ti_sysbios_timers_rti_Timer_Params *prms ) 
{
    if (prms != NULL) {
        ti_sysbios_timers_rti_Timer_Params__init__S(prms, NULL, sizeof(ti_sysbios_timers_rti_Timer_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Params_copy */
static inline void ti_sysbios_timers_rti_Timer_Params_copy(ti_sysbios_timers_rti_Timer_Params *dst, const ti_sysbios_timers_rti_Timer_Params *src);
static inline void ti_sysbios_timers_rti_Timer_Params_copy(ti_sysbios_timers_rti_Timer_Params *dst, const ti_sysbios_timers_rti_Timer_Params *src) 
{
    if (dst != NULL) {
        ti_sysbios_timers_rti_Timer_Params__init__S(dst, (const void *)src, sizeof(ti_sysbios_timers_rti_Timer_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Object_count */
#define ti_sysbios_timers_rti_Timer_Object_count() ti_sysbios_timers_rti_Timer_Object__count__C

/* Object_sizeof */
#define ti_sysbios_timers_rti_Timer_Object_sizeof() ti_sysbios_timers_rti_Timer_Object__sizeof__C

/* Object_get */
static inline ti_sysbios_timers_rti_Timer_Handle ti_sysbios_timers_rti_Timer_Object_get(ti_sysbios_timers_rti_Timer_Instance_State *oarr, int i);
static inline ti_sysbios_timers_rti_Timer_Handle ti_sysbios_timers_rti_Timer_Object_get(ti_sysbios_timers_rti_Timer_Instance_State *oarr, int i) 
{
    return (ti_sysbios_timers_rti_Timer_Handle)ti_sysbios_timers_rti_Timer_Object__get__S(oarr, i);
}

/* Object_first */
static inline ti_sysbios_timers_rti_Timer_Handle ti_sysbios_timers_rti_Timer_Object_first(void);
static inline ti_sysbios_timers_rti_Timer_Handle ti_sysbios_timers_rti_Timer_Object_first(void)
{
    return (ti_sysbios_timers_rti_Timer_Handle)ti_sysbios_timers_rti_Timer_Object__first__S();
}

/* Object_next */
static inline ti_sysbios_timers_rti_Timer_Handle ti_sysbios_timers_rti_Timer_Object_next(ti_sysbios_timers_rti_Timer_Object *obj);
static inline ti_sysbios_timers_rti_Timer_Handle ti_sysbios_timers_rti_Timer_Object_next(ti_sysbios_timers_rti_Timer_Object *obj)
{
    return (ti_sysbios_timers_rti_Timer_Handle)ti_sysbios_timers_rti_Timer_Object__next__S(obj);
}

/* Handle_label */
static inline xdc_runtime_Types_Label *ti_sysbios_timers_rti_Timer_Handle_label(ti_sysbios_timers_rti_Timer_Handle inst, xdc_runtime_Types_Label *lab);
static inline xdc_runtime_Types_Label *ti_sysbios_timers_rti_Timer_Handle_label(ti_sysbios_timers_rti_Timer_Handle inst, xdc_runtime_Types_Label *lab)
{
    return ti_sysbios_timers_rti_Timer_Handle__label__S(inst, lab);
}

/* Handle_name */
static inline xdc_String ti_sysbios_timers_rti_Timer_Handle_name(ti_sysbios_timers_rti_Timer_Handle inst);
static inline xdc_String ti_sysbios_timers_rti_Timer_Handle_name(ti_sysbios_timers_rti_Timer_Handle inst)
{
    xdc_runtime_Types_Label lab;
    return ti_sysbios_timers_rti_Timer_Handle__label__S(inst, &lab)->iname;
}

/* handle */
static inline ti_sysbios_timers_rti_Timer_Handle ti_sysbios_timers_rti_Timer_handle(ti_sysbios_timers_rti_Timer_Struct *str);
static inline ti_sysbios_timers_rti_Timer_Handle ti_sysbios_timers_rti_Timer_handle(ti_sysbios_timers_rti_Timer_Struct *str)
{
    return (ti_sysbios_timers_rti_Timer_Handle)str;
}

/* struct */
static inline ti_sysbios_timers_rti_Timer_Struct *ti_sysbios_timers_rti_Timer_struct(ti_sysbios_timers_rti_Timer_Handle inst);
static inline ti_sysbios_timers_rti_Timer_Struct *ti_sysbios_timers_rti_Timer_struct(ti_sysbios_timers_rti_Timer_Handle inst)
{
    return (ti_sysbios_timers_rti_Timer_Struct*)inst;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_timers_rti_Timer__top__
#undef __nested__
#endif

#endif /* ti_sysbios_timers_rti_Timer__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_timers_rti_Timer__internalaccess))

#ifndef ti_sysbios_timers_rti_Timer__include_state
#define ti_sysbios_timers_rti_Timer__include_state

/* Module_State */
struct ti_sysbios_timers_rti_Timer_Module_State {
    xdc_UInt availMask;
    __TA_ti_sysbios_timers_rti_Timer_Module_State__device device;
    __TA_ti_sysbios_timers_rti_Timer_Module_State__intFreqs intFreqs;
    __TA_ti_sysbios_timers_rti_Timer_Module_State__handles handles;
};

/* Module__state__V */
#ifndef ti_sysbios_timers_rti_Timer_Module__state__VR
extern struct ti_sysbios_timers_rti_Timer_Module_State__ ti_sysbios_timers_rti_Timer_Module__state__V;
#else
#define ti_sysbios_timers_rti_Timer_Module__state__V (*((struct ti_sysbios_timers_rti_Timer_Module_State__*)(xdcRomStatePtr + ti_sysbios_timers_rti_Timer_Module__state__V_offset)))
#endif

/* Object */
struct ti_sysbios_timers_rti_Timer_Object {
    const ti_sysbios_timers_rti_Timer_Fxns__ *__fxns;
    xdc_Bool staticInst;
    xdc_Int id;
    ti_sysbios_interfaces_ITimer_RunMode runMode;
    ti_sysbios_interfaces_ITimer_StartMode startMode;
    xdc_UInt prescale;
    xdc_UInt period;
    ti_sysbios_interfaces_ITimer_PeriodType periodType;
    xdc_UInt intNum;
    xdc_UArg arg;
    ti_sysbios_hal_Hwi_FuncPtr tickFxn;
    xdc_runtime_Types_FreqHz extFreq;
    ti_sysbios_hal_Hwi_Handle hwi;
    xdc_Bool createHwi;
};

#endif /* ti_sysbios_timers_rti_Timer__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_timers_rti_Timer__nolocalnames)

#ifndef ti_sysbios_timers_rti_Timer__localnames__done
#define ti_sysbios_timers_rti_Timer__localnames__done

/* module prefix */
#define Timer_Instance ti_sysbios_timers_rti_Timer_Instance
#define Timer_Handle ti_sysbios_timers_rti_Timer_Handle
#define Timer_Module ti_sysbios_timers_rti_Timer_Module
#define Timer_Object ti_sysbios_timers_rti_Timer_Object
#define Timer_Struct ti_sysbios_timers_rti_Timer_Struct
#define Timer_FuncPtr ti_sysbios_timers_rti_Timer_FuncPtr
#define Timer_ANY ti_sysbios_timers_rti_Timer_ANY
#define Timer_StartMode ti_sysbios_timers_rti_Timer_StartMode
#define Timer_RunMode ti_sysbios_timers_rti_Timer_RunMode
#define Timer_Status ti_sysbios_timers_rti_Timer_Status
#define Timer_PeriodType ti_sysbios_timers_rti_Timer_PeriodType
#define Timer_MAX_PERIOD ti_sysbios_timers_rti_Timer_MAX_PERIOD
#define Timer_MIN_SWEEP_PERIOD ti_sysbios_timers_rti_Timer_MIN_SWEEP_PERIOD
#define Timer_DeviceRegs ti_sysbios_timers_rti_Timer_DeviceRegs
#define Timer_TimerDevice ti_sysbios_timers_rti_Timer_TimerDevice
#define Timer_Instance_State ti_sysbios_timers_rti_Timer_Instance_State
#define Timer_Module_State ti_sysbios_timers_rti_Timer_Module_State
#define Timer_StartMode_AUTO ti_sysbios_timers_rti_Timer_StartMode_AUTO
#define Timer_StartMode_USER ti_sysbios_timers_rti_Timer_StartMode_USER
#define Timer_RunMode_CONTINUOUS ti_sysbios_timers_rti_Timer_RunMode_CONTINUOUS
#define Timer_RunMode_ONESHOT ti_sysbios_timers_rti_Timer_RunMode_ONESHOT
#define Timer_RunMode_DYNAMIC ti_sysbios_timers_rti_Timer_RunMode_DYNAMIC
#define Timer_Status_INUSE ti_sysbios_timers_rti_Timer_Status_INUSE
#define Timer_Status_FREE ti_sysbios_timers_rti_Timer_Status_FREE
#define Timer_PeriodType_MICROSECS ti_sysbios_timers_rti_Timer_PeriodType_MICROSECS
#define Timer_PeriodType_COUNTS ti_sysbios_timers_rti_Timer_PeriodType_COUNTS
#define Timer_A_invalidTimer ti_sysbios_timers_rti_Timer_A_invalidTimer
#define Timer_E_invalidTimer ti_sysbios_timers_rti_Timer_E_invalidTimer
#define Timer_E_notAvailable ti_sysbios_timers_rti_Timer_E_notAvailable
#define Timer_E_invalidHwiMask ti_sysbios_timers_rti_Timer_E_invalidHwiMask
#define Timer_E_cannotSupport ti_sysbios_timers_rti_Timer_E_cannotSupport
#define Timer_anyMask ti_sysbios_timers_rti_Timer_anyMask
#define Timer_continueOnSuspend ti_sysbios_timers_rti_Timer_continueOnSuspend
#define Timer_startupNeeded ti_sysbios_timers_rti_Timer_startupNeeded
#define Timer_numTimerDevices ti_sysbios_timers_rti_Timer_numTimerDevices
#define Timer_Params ti_sysbios_timers_rti_Timer_Params
#define Timer_getNumTimers ti_sysbios_timers_rti_Timer_getNumTimers
#define Timer_getStatus ti_sysbios_timers_rti_Timer_getStatus
#define Timer_startup ti_sysbios_timers_rti_Timer_startup
#define Timer_getMaxTicks ti_sysbios_timers_rti_Timer_getMaxTicks
#define Timer_setNextTick ti_sysbios_timers_rti_Timer_setNextTick
#define Timer_start ti_sysbios_timers_rti_Timer_start
#define Timer_stop ti_sysbios_timers_rti_Timer_stop
#define Timer_setPeriod ti_sysbios_timers_rti_Timer_setPeriod
#define Timer_setPeriodMicroSecs ti_sysbios_timers_rti_Timer_setPeriodMicroSecs
#define Timer_getPeriod ti_sysbios_timers_rti_Timer_getPeriod
#define Timer_getCount ti_sysbios_timers_rti_Timer_getCount
#define Timer_getFreq ti_sysbios_timers_rti_Timer_getFreq
#define Timer_getFunc ti_sysbios_timers_rti_Timer_getFunc
#define Timer_setFunc ti_sysbios_timers_rti_Timer_setFunc
#define Timer_trigger ti_sysbios_timers_rti_Timer_trigger
#define Timer_getExpiredCounts ti_sysbios_timers_rti_Timer_getExpiredCounts
#define Timer_getExpiredTicks ti_sysbios_timers_rti_Timer_getExpiredTicks
#define Timer_getCurrentTick ti_sysbios_timers_rti_Timer_getCurrentTick
#define Timer_oneShotStub ti_sysbios_timers_rti_Timer_oneShotStub
#define Timer_periodicStub ti_sysbios_timers_rti_Timer_periodicStub
#define Timer_getHandle ti_sysbios_timers_rti_Timer_getHandle
#define Timer_reconfig ti_sysbios_timers_rti_Timer_reconfig
#define Timer_ackInterrupt ti_sysbios_timers_rti_Timer_ackInterrupt
#define Timer_Module_name ti_sysbios_timers_rti_Timer_Module_name
#define Timer_Module_id ti_sysbios_timers_rti_Timer_Module_id
#define Timer_Module_startup ti_sysbios_timers_rti_Timer_Module_startup
#define Timer_Module_startupDone ti_sysbios_timers_rti_Timer_Module_startupDone
#define Timer_Module_hasMask ti_sysbios_timers_rti_Timer_Module_hasMask
#define Timer_Module_getMask ti_sysbios_timers_rti_Timer_Module_getMask
#define Timer_Module_setMask ti_sysbios_timers_rti_Timer_Module_setMask
#define Timer_Object_heap ti_sysbios_timers_rti_Timer_Object_heap
#define Timer_Module_heap ti_sysbios_timers_rti_Timer_Module_heap
#define Timer_construct ti_sysbios_timers_rti_Timer_construct
#define Timer_create ti_sysbios_timers_rti_Timer_create
#define Timer_handle ti_sysbios_timers_rti_Timer_handle
#define Timer_struct ti_sysbios_timers_rti_Timer_struct
#define Timer_Handle_label ti_sysbios_timers_rti_Timer_Handle_label
#define Timer_Handle_name ti_sysbios_timers_rti_Timer_Handle_name
#define Timer_Instance_init ti_sysbios_timers_rti_Timer_Instance_init
#define Timer_Object_count ti_sysbios_timers_rti_Timer_Object_count
#define Timer_Object_get ti_sysbios_timers_rti_Timer_Object_get
#define Timer_Object_first ti_sysbios_timers_rti_Timer_Object_first
#define Timer_Object_next ti_sysbios_timers_rti_Timer_Object_next
#define Timer_Object_sizeof ti_sysbios_timers_rti_Timer_Object_sizeof
#define Timer_Params_copy ti_sysbios_timers_rti_Timer_Params_copy
#define Timer_Params_init ti_sysbios_timers_rti_Timer_Params_init
#define Timer_Instance_finalize ti_sysbios_timers_rti_Timer_Instance_finalize
#define Timer_delete ti_sysbios_timers_rti_Timer_delete
#define Timer_destruct ti_sysbios_timers_rti_Timer_destruct
#define Timer_Module_upCast ti_sysbios_timers_rti_Timer_Module_upCast
#define Timer_Module_to_ti_sysbios_interfaces_ITimer ti_sysbios_timers_rti_Timer_Module_to_ti_sysbios_interfaces_ITimer
#define Timer_Handle_upCast ti_sysbios_timers_rti_Timer_Handle_upCast
#define Timer_Handle_to_ti_sysbios_interfaces_ITimer ti_sysbios_timers_rti_Timer_Handle_to_ti_sysbios_interfaces_ITimer
#define Timer_Handle_downCast ti_sysbios_timers_rti_Timer_Handle_downCast
#define Timer_Handle_from_ti_sysbios_interfaces_ITimer ti_sysbios_timers_rti_Timer_Handle_from_ti_sysbios_interfaces_ITimer

#endif /* ti_sysbios_timers_rti_Timer__localnames__done */
#endif
