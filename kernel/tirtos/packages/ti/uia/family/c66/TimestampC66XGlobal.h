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

#ifndef ti_uia_family_c66_TimestampC66XGlobal__include
#define ti_uia_family_c66_TimestampC66XGlobal__include

#ifndef __nested__
#define __nested__
#define ti_uia_family_c66_TimestampC66XGlobal__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_uia_family_c66_TimestampC66XGlobal___VERS 200


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/IInstance.h>
#include <ti/uia/family/c66/package/package.defs.h>

#include <xdc/runtime/Types.h>
#include <ti/uia/runtime/IUIATimestampProvider.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */


/*
 * ======== INTERNAL DEFINITIONS ========
 */


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_uia_family_c66_TimestampC66XGlobal_Module__diagsEnabled;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Module__diagsEnabled ti_uia_family_c66_TimestampC66XGlobal_Module__diagsEnabled__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Module__diagsEnabled__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Module__diagsEnabled__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Module__diagsEnabled*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__diagsEnabled__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__diagsEnabled (ti_uia_family_c66_TimestampC66XGlobal_Module__diagsEnabled__C)
#endif

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_uia_family_c66_TimestampC66XGlobal_Module__diagsIncluded;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Module__diagsIncluded ti_uia_family_c66_TimestampC66XGlobal_Module__diagsIncluded__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Module__diagsIncluded__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Module__diagsIncluded__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Module__diagsIncluded*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__diagsIncluded__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__diagsIncluded (ti_uia_family_c66_TimestampC66XGlobal_Module__diagsIncluded__C)
#endif

/* Module__diagsMask */
typedef xdc_Bits16 *CT__ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask (ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask__C)
#endif

/* Module__gateObj */
typedef xdc_Ptr CT__ti_uia_family_c66_TimestampC66XGlobal_Module__gateObj;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Module__gateObj ti_uia_family_c66_TimestampC66XGlobal_Module__gateObj__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Module__gateObj__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Module__gateObj__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Module__gateObj*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__gateObj__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__gateObj (ti_uia_family_c66_TimestampC66XGlobal_Module__gateObj__C)
#endif

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_uia_family_c66_TimestampC66XGlobal_Module__gatePrms;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Module__gatePrms ti_uia_family_c66_TimestampC66XGlobal_Module__gatePrms__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Module__gatePrms__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Module__gatePrms__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Module__gatePrms*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__gatePrms__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__gatePrms (ti_uia_family_c66_TimestampC66XGlobal_Module__gatePrms__C)
#endif

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_uia_family_c66_TimestampC66XGlobal_Module__id;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Module__id ti_uia_family_c66_TimestampC66XGlobal_Module__id__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Module__id__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Module__id__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Module__id*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__id__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__id (ti_uia_family_c66_TimestampC66XGlobal_Module__id__C)
#endif

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerDefined;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerDefined ti_uia_family_c66_TimestampC66XGlobal_Module__loggerDefined__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Module__loggerDefined__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerDefined__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerDefined*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__loggerDefined__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerDefined (ti_uia_family_c66_TimestampC66XGlobal_Module__loggerDefined__C)
#endif

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerObj;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerObj ti_uia_family_c66_TimestampC66XGlobal_Module__loggerObj__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Module__loggerObj__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerObj__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerObj*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__loggerObj__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerObj (ti_uia_family_c66_TimestampC66XGlobal_Module__loggerObj__C)
#endif

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn0;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn0 ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn0__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn0__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn0__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn0*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn0__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn0 (ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn0__C)
#endif

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn1;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn1 ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn1__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn1__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn1__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn1*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn1__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn1 (ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn1__C)
#endif

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn2;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn2 ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn2__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn2__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn2__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn2*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn2__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn2 (ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn2__C)
#endif

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn4;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn4 ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn4__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn4__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn4__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn4*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn4__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn4 (ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn4__C)
#endif

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn8;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn8 ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn8__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn8__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn8__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn8*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn8__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn8 (ti_uia_family_c66_TimestampC66XGlobal_Module__loggerFxn8__C)
#endif

/* Object__count */
typedef xdc_Int CT__ti_uia_family_c66_TimestampC66XGlobal_Object__count;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Object__count ti_uia_family_c66_TimestampC66XGlobal_Object__count__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Object__count__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Object__count__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Object__count*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Object__count__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Object__count (ti_uia_family_c66_TimestampC66XGlobal_Object__count__C)
#endif

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_uia_family_c66_TimestampC66XGlobal_Object__heap;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Object__heap ti_uia_family_c66_TimestampC66XGlobal_Object__heap__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Object__heap__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Object__heap__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Object__heap*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Object__heap__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Object__heap (ti_uia_family_c66_TimestampC66XGlobal_Object__heap__C)
#endif

/* Object__sizeof */
typedef xdc_SizeT CT__ti_uia_family_c66_TimestampC66XGlobal_Object__sizeof;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Object__sizeof ti_uia_family_c66_TimestampC66XGlobal_Object__sizeof__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Object__sizeof__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Object__sizeof__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Object__sizeof*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Object__sizeof__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Object__sizeof (ti_uia_family_c66_TimestampC66XGlobal_Object__sizeof__C)
#endif

/* Object__table */
typedef xdc_Ptr CT__ti_uia_family_c66_TimestampC66XGlobal_Object__table;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_Object__table ti_uia_family_c66_TimestampC66XGlobal_Object__table__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_Object__table__CR
#define ti_uia_family_c66_TimestampC66XGlobal_Object__table__C (*((CT__ti_uia_family_c66_TimestampC66XGlobal_Object__table*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Object__table__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Object__table (ti_uia_family_c66_TimestampC66XGlobal_Object__table__C)
#endif

/* timerBaseAdrs */
typedef xdc_UInt32 CT__ti_uia_family_c66_TimestampC66XGlobal_timerBaseAdrs;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_timerBaseAdrs ti_uia_family_c66_TimestampC66XGlobal_timerBaseAdrs__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_timerBaseAdrs__CR
#define ti_uia_family_c66_TimestampC66XGlobal_timerBaseAdrs (*((CT__ti_uia_family_c66_TimestampC66XGlobal_timerBaseAdrs*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_timerBaseAdrs__C_offset)))
#else
#ifdef ti_uia_family_c66_TimestampC66XGlobal_timerBaseAdrs__D
#define ti_uia_family_c66_TimestampC66XGlobal_timerBaseAdrs (ti_uia_family_c66_TimestampC66XGlobal_timerBaseAdrs__D)
#else
#define ti_uia_family_c66_TimestampC66XGlobal_timerBaseAdrs (ti_uia_family_c66_TimestampC66XGlobal_timerBaseAdrs__C)
#endif
#endif

/* maxTimerClockFreq */
typedef xdc_runtime_Types_FreqHz CT__ti_uia_family_c66_TimestampC66XGlobal_maxTimerClockFreq;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_maxTimerClockFreq ti_uia_family_c66_TimestampC66XGlobal_maxTimerClockFreq__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_maxTimerClockFreq__CR
#define ti_uia_family_c66_TimestampC66XGlobal_maxTimerClockFreq (*((CT__ti_uia_family_c66_TimestampC66XGlobal_maxTimerClockFreq*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_maxTimerClockFreq__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_maxTimerClockFreq (ti_uia_family_c66_TimestampC66XGlobal_maxTimerClockFreq__C)
#endif

/* maxBusClockFreq */
typedef xdc_runtime_Types_FreqHz CT__ti_uia_family_c66_TimestampC66XGlobal_maxBusClockFreq;
__extern __FAR__ const CT__ti_uia_family_c66_TimestampC66XGlobal_maxBusClockFreq ti_uia_family_c66_TimestampC66XGlobal_maxBusClockFreq__C;
#ifdef ti_uia_family_c66_TimestampC66XGlobal_maxBusClockFreq__CR
#define ti_uia_family_c66_TimestampC66XGlobal_maxBusClockFreq (*((CT__ti_uia_family_c66_TimestampC66XGlobal_maxBusClockFreq*)(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_maxBusClockFreq__C_offset)))
#else
#define ti_uia_family_c66_TimestampC66XGlobal_maxBusClockFreq (ti_uia_family_c66_TimestampC66XGlobal_maxBusClockFreq__C)
#endif


/*
 * ======== PER-INSTANCE TYPES ========
 */

/* Params */
struct ti_uia_family_c66_TimestampC66XGlobal_Params {
    size_t __size;
    const void *__self;
    void *__fxns;
    xdc_runtime_IInstance_Params *instance;
    xdc_runtime_IInstance_Params __iprms;
};

/* Struct */
struct ti_uia_family_c66_TimestampC66XGlobal_Struct {
    const ti_uia_family_c66_TimestampC66XGlobal_Fxns__ *__fxns;
    xdc_runtime_Types_CordAddr __name;
};


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_uia_family_c66_TimestampC66XGlobal_Fxns__ {
    const xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2* __sysp;
    xdc_Bits32 (*get32)(void);
    xdc_Void (*get64)(xdc_runtime_Types_Timestamp64* result);
    xdc_Void (*getFreq)(xdc_runtime_Types_FreqHz* freq);
    xdc_runtime_Types_SysFxns2 __sfxns;
};
#ifndef ti_uia_family_c66_TimestampC66XGlobal_Module__FXNS__CR

/* Module__FXNS__C */
__extern const ti_uia_family_c66_TimestampC66XGlobal_Fxns__ ti_uia_family_c66_TimestampC66XGlobal_Module__FXNS__C;
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__FXNS__C (*(xdcRomConstPtr + ti_uia_family_c66_TimestampC66XGlobal_Module__FXNS__C_offset))
#endif


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_uia_family_c66_TimestampC66XGlobal_Module_startup ti_uia_family_c66_TimestampC66XGlobal_Module_startup__E
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_Module_startup__E, "ti_uia_family_c66_TimestampC66XGlobal_Module_startup")
__extern xdc_Int ti_uia_family_c66_TimestampC66XGlobal_Module_startup__E( xdc_Int state );
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_Module_startup__F, "ti_uia_family_c66_TimestampC66XGlobal_Module_startup")
__extern xdc_Int ti_uia_family_c66_TimestampC66XGlobal_Module_startup__F( xdc_Int state );

/* Instance_init__E */
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_Instance_init__E, "ti_uia_family_c66_TimestampC66XGlobal_Instance_init")
__extern xdc_Void ti_uia_family_c66_TimestampC66XGlobal_Instance_init__E(ti_uia_family_c66_TimestampC66XGlobal_Object *obj, const ti_uia_family_c66_TimestampC66XGlobal_Params *prms);

/* create */
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_create, "ti_uia_family_c66_TimestampC66XGlobal_create")
__extern ti_uia_family_c66_TimestampC66XGlobal_Handle ti_uia_family_c66_TimestampC66XGlobal_create( const ti_uia_family_c66_TimestampC66XGlobal_Params *prms, xdc_runtime_Error_Block *eb);

/* construct */
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_construct, "ti_uia_family_c66_TimestampC66XGlobal_construct")
__extern void ti_uia_family_c66_TimestampC66XGlobal_construct(ti_uia_family_c66_TimestampC66XGlobal_Struct *obj, const ti_uia_family_c66_TimestampC66XGlobal_Params *prms);

/* delete */
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_delete, "ti_uia_family_c66_TimestampC66XGlobal_delete")
__extern void ti_uia_family_c66_TimestampC66XGlobal_delete(ti_uia_family_c66_TimestampC66XGlobal_Handle *instp);

/* destruct */
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_destruct, "ti_uia_family_c66_TimestampC66XGlobal_destruct")
__extern void ti_uia_family_c66_TimestampC66XGlobal_destruct(ti_uia_family_c66_TimestampC66XGlobal_Struct *obj);

/* Handle__label__S */
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_Handle__label__S, "ti_uia_family_c66_TimestampC66XGlobal_Handle__label__S")
__extern xdc_runtime_Types_Label *ti_uia_family_c66_TimestampC66XGlobal_Handle__label__S( xdc_Ptr obj, xdc_runtime_Types_Label *lab );

/* Module__startupDone__S */
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_Module__startupDone__S, "ti_uia_family_c66_TimestampC66XGlobal_Module__startupDone__S")
__extern xdc_Bool ti_uia_family_c66_TimestampC66XGlobal_Module__startupDone__S( void );

/* Object__create__S */
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_Object__create__S, "ti_uia_family_c66_TimestampC66XGlobal_Object__create__S")
__extern xdc_Ptr ti_uia_family_c66_TimestampC66XGlobal_Object__create__S( xdc_CPtr aa, const xdc_UChar *pa, xdc_SizeT psz, xdc_runtime_Error_Block *eb );

/* Object__delete__S */
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_Object__delete__S, "ti_uia_family_c66_TimestampC66XGlobal_Object__delete__S")
__extern xdc_Void ti_uia_family_c66_TimestampC66XGlobal_Object__delete__S( xdc_Ptr instp );

/* Object__get__S */
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_Object__get__S, "ti_uia_family_c66_TimestampC66XGlobal_Object__get__S")
__extern xdc_Ptr ti_uia_family_c66_TimestampC66XGlobal_Object__get__S( xdc_Ptr oarr, xdc_Int i );

/* Object__first__S */
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_Object__first__S, "ti_uia_family_c66_TimestampC66XGlobal_Object__first__S")
__extern xdc_Ptr ti_uia_family_c66_TimestampC66XGlobal_Object__first__S( void );

/* Object__next__S */
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_Object__next__S, "ti_uia_family_c66_TimestampC66XGlobal_Object__next__S")
__extern xdc_Ptr ti_uia_family_c66_TimestampC66XGlobal_Object__next__S( xdc_Ptr obj );

/* Params__init__S */
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_Params__init__S, "ti_uia_family_c66_TimestampC66XGlobal_Params__init__S")
__extern xdc_Void ti_uia_family_c66_TimestampC66XGlobal_Params__init__S( xdc_Ptr dst, const xdc_Void *src, xdc_SizeT psz, xdc_SizeT isz );

/* get32__E */
#define ti_uia_family_c66_TimestampC66XGlobal_get32 ti_uia_family_c66_TimestampC66XGlobal_get32__E
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_get32__E, "ti_uia_family_c66_TimestampC66XGlobal_get32")
__extern xdc_Bits32 ti_uia_family_c66_TimestampC66XGlobal_get32__E( void );

/* get64__E */
#define ti_uia_family_c66_TimestampC66XGlobal_get64 ti_uia_family_c66_TimestampC66XGlobal_get64__E
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_get64__E, "ti_uia_family_c66_TimestampC66XGlobal_get64")
__extern xdc_Void ti_uia_family_c66_TimestampC66XGlobal_get64__E( xdc_runtime_Types_Timestamp64 *result );

/* getFreq__E */
#define ti_uia_family_c66_TimestampC66XGlobal_getFreq ti_uia_family_c66_TimestampC66XGlobal_getFreq__E
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_getFreq__E, "ti_uia_family_c66_TimestampC66XGlobal_getFreq")
__extern xdc_Void ti_uia_family_c66_TimestampC66XGlobal_getFreq__E( xdc_runtime_Types_FreqHz *freq );

/* setFreq__E */
#define ti_uia_family_c66_TimestampC66XGlobal_setFreq ti_uia_family_c66_TimestampC66XGlobal_setFreq__E
xdc__CODESECT(ti_uia_family_c66_TimestampC66XGlobal_setFreq__E, "ti_uia_family_c66_TimestampC66XGlobal_setFreq")
__extern xdc_Void ti_uia_family_c66_TimestampC66XGlobal_setFreq__E( xdc_runtime_Types_FreqHz *freq );


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline ti_uia_runtime_IUIATimestampProvider_Module ti_uia_family_c66_TimestampC66XGlobal_Module_upCast(void);
static inline ti_uia_runtime_IUIATimestampProvider_Module ti_uia_family_c66_TimestampC66XGlobal_Module_upCast(void)
{
    return (ti_uia_runtime_IUIATimestampProvider_Module)&ti_uia_family_c66_TimestampC66XGlobal_Module__FXNS__C;
}

/* Module_to_ti_uia_runtime_IUIATimestampProvider */
#define ti_uia_family_c66_TimestampC66XGlobal_Module_to_ti_uia_runtime_IUIATimestampProvider ti_uia_family_c66_TimestampC66XGlobal_Module_upCast

/* Module_upCast2 */
static inline xdc_runtime_ITimestampProvider_Module ti_uia_family_c66_TimestampC66XGlobal_Module_upCast2(void);
static inline xdc_runtime_ITimestampProvider_Module ti_uia_family_c66_TimestampC66XGlobal_Module_upCast2(void)
{
    return (xdc_runtime_ITimestampProvider_Module)&ti_uia_family_c66_TimestampC66XGlobal_Module__FXNS__C;
}

/* Module_to_xdc_runtime_ITimestampProvider */
#define ti_uia_family_c66_TimestampC66XGlobal_Module_to_xdc_runtime_ITimestampProvider ti_uia_family_c66_TimestampC66XGlobal_Module_upCast2

/* Module_upCast3 */
static inline xdc_runtime_ITimestampClient_Module ti_uia_family_c66_TimestampC66XGlobal_Module_upCast3(void);
static inline xdc_runtime_ITimestampClient_Module ti_uia_family_c66_TimestampC66XGlobal_Module_upCast3(void)
{
    return (xdc_runtime_ITimestampClient_Module)&ti_uia_family_c66_TimestampC66XGlobal_Module__FXNS__C;
}

/* Module_to_xdc_runtime_ITimestampClient */
#define ti_uia_family_c66_TimestampC66XGlobal_Module_to_xdc_runtime_ITimestampClient ti_uia_family_c66_TimestampC66XGlobal_Module_upCast3


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_uia_family_c66_TimestampC66XGlobal_Module_startupDone() ti_uia_family_c66_TimestampC66XGlobal_Module__startupDone__S()

/* Object_heap */
#define ti_uia_family_c66_TimestampC66XGlobal_Object_heap() ti_uia_family_c66_TimestampC66XGlobal_Object__heap__C

/* Module_heap */
#define ti_uia_family_c66_TimestampC66XGlobal_Module_heap() ti_uia_family_c66_TimestampC66XGlobal_Object__heap__C

/* Module_id */
static inline CT__ti_uia_family_c66_TimestampC66XGlobal_Module__id ti_uia_family_c66_TimestampC66XGlobal_Module_id(void);
static inline CT__ti_uia_family_c66_TimestampC66XGlobal_Module__id ti_uia_family_c66_TimestampC66XGlobal_Module_id( void ) 
{
    return ti_uia_family_c66_TimestampC66XGlobal_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_uia_family_c66_TimestampC66XGlobal_Module_hasMask(void);
static inline xdc_Bool ti_uia_family_c66_TimestampC66XGlobal_Module_hasMask(void)
{
    return (xdc_Bool)(ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask__C != (CT__ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask)NULL);
}

/* Module_getMask */
static inline xdc_Bits16 ti_uia_family_c66_TimestampC66XGlobal_Module_getMask(void);
static inline xdc_Bits16 ti_uia_family_c66_TimestampC66XGlobal_Module_getMask(void)
{
    return (ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask__C != (CT__ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask)NULL) ? *ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask__C : (xdc_Bits16)0;
}

/* Module_setMask */
static inline xdc_Void ti_uia_family_c66_TimestampC66XGlobal_Module_setMask(xdc_Bits16 mask);
static inline xdc_Void ti_uia_family_c66_TimestampC66XGlobal_Module_setMask(xdc_Bits16 mask)
{
    if (ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask__C != (CT__ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask)NULL) {
        *ti_uia_family_c66_TimestampC66XGlobal_Module__diagsMask__C = mask;
    }
}

/* Params_init */
static inline void ti_uia_family_c66_TimestampC66XGlobal_Params_init(ti_uia_family_c66_TimestampC66XGlobal_Params *prms);
static inline void ti_uia_family_c66_TimestampC66XGlobal_Params_init( ti_uia_family_c66_TimestampC66XGlobal_Params *prms ) 
{
    if (prms != NULL) {
        ti_uia_family_c66_TimestampC66XGlobal_Params__init__S(prms, NULL, sizeof(ti_uia_family_c66_TimestampC66XGlobal_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Params_copy */
static inline void ti_uia_family_c66_TimestampC66XGlobal_Params_copy(ti_uia_family_c66_TimestampC66XGlobal_Params *dst, const ti_uia_family_c66_TimestampC66XGlobal_Params *src);
static inline void ti_uia_family_c66_TimestampC66XGlobal_Params_copy(ti_uia_family_c66_TimestampC66XGlobal_Params *dst, const ti_uia_family_c66_TimestampC66XGlobal_Params *src) 
{
    if (dst != NULL) {
        ti_uia_family_c66_TimestampC66XGlobal_Params__init__S(dst, (const void *)src, sizeof(ti_uia_family_c66_TimestampC66XGlobal_Params), sizeof(xdc_runtime_IInstance_Params));
    }
}

/* Object_count */
#define ti_uia_family_c66_TimestampC66XGlobal_Object_count() ti_uia_family_c66_TimestampC66XGlobal_Object__count__C

/* Object_sizeof */
#define ti_uia_family_c66_TimestampC66XGlobal_Object_sizeof() ti_uia_family_c66_TimestampC66XGlobal_Object__sizeof__C

/* Object_get */
static inline ti_uia_family_c66_TimestampC66XGlobal_Handle ti_uia_family_c66_TimestampC66XGlobal_Object_get(ti_uia_family_c66_TimestampC66XGlobal_Instance_State *oarr, int i);
static inline ti_uia_family_c66_TimestampC66XGlobal_Handle ti_uia_family_c66_TimestampC66XGlobal_Object_get(ti_uia_family_c66_TimestampC66XGlobal_Instance_State *oarr, int i) 
{
    return (ti_uia_family_c66_TimestampC66XGlobal_Handle)ti_uia_family_c66_TimestampC66XGlobal_Object__get__S(oarr, i);
}

/* Object_first */
static inline ti_uia_family_c66_TimestampC66XGlobal_Handle ti_uia_family_c66_TimestampC66XGlobal_Object_first(void);
static inline ti_uia_family_c66_TimestampC66XGlobal_Handle ti_uia_family_c66_TimestampC66XGlobal_Object_first(void)
{
    return (ti_uia_family_c66_TimestampC66XGlobal_Handle)ti_uia_family_c66_TimestampC66XGlobal_Object__first__S();
}

/* Object_next */
static inline ti_uia_family_c66_TimestampC66XGlobal_Handle ti_uia_family_c66_TimestampC66XGlobal_Object_next(ti_uia_family_c66_TimestampC66XGlobal_Object *obj);
static inline ti_uia_family_c66_TimestampC66XGlobal_Handle ti_uia_family_c66_TimestampC66XGlobal_Object_next(ti_uia_family_c66_TimestampC66XGlobal_Object *obj)
{
    return (ti_uia_family_c66_TimestampC66XGlobal_Handle)ti_uia_family_c66_TimestampC66XGlobal_Object__next__S(obj);
}

/* Handle_label */
static inline xdc_runtime_Types_Label *ti_uia_family_c66_TimestampC66XGlobal_Handle_label(ti_uia_family_c66_TimestampC66XGlobal_Handle inst, xdc_runtime_Types_Label *lab);
static inline xdc_runtime_Types_Label *ti_uia_family_c66_TimestampC66XGlobal_Handle_label(ti_uia_family_c66_TimestampC66XGlobal_Handle inst, xdc_runtime_Types_Label *lab)
{
    return ti_uia_family_c66_TimestampC66XGlobal_Handle__label__S(inst, lab);
}

/* Handle_name */
static inline xdc_String ti_uia_family_c66_TimestampC66XGlobal_Handle_name(ti_uia_family_c66_TimestampC66XGlobal_Handle inst);
static inline xdc_String ti_uia_family_c66_TimestampC66XGlobal_Handle_name(ti_uia_family_c66_TimestampC66XGlobal_Handle inst)
{
    xdc_runtime_Types_Label lab;
    return ti_uia_family_c66_TimestampC66XGlobal_Handle__label__S(inst, &lab)->iname;
}

/* handle */
static inline ti_uia_family_c66_TimestampC66XGlobal_Handle ti_uia_family_c66_TimestampC66XGlobal_handle(ti_uia_family_c66_TimestampC66XGlobal_Struct *str);
static inline ti_uia_family_c66_TimestampC66XGlobal_Handle ti_uia_family_c66_TimestampC66XGlobal_handle(ti_uia_family_c66_TimestampC66XGlobal_Struct *str)
{
    return (ti_uia_family_c66_TimestampC66XGlobal_Handle)str;
}

/* struct */
static inline ti_uia_family_c66_TimestampC66XGlobal_Struct *ti_uia_family_c66_TimestampC66XGlobal_struct(ti_uia_family_c66_TimestampC66XGlobal_Handle inst);
static inline ti_uia_family_c66_TimestampC66XGlobal_Struct *ti_uia_family_c66_TimestampC66XGlobal_struct(ti_uia_family_c66_TimestampC66XGlobal_Handle inst)
{
    return (ti_uia_family_c66_TimestampC66XGlobal_Struct*)inst;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_uia_family_c66_TimestampC66XGlobal__top__
#undef __nested__
#endif

#endif /* ti_uia_family_c66_TimestampC66XGlobal__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_uia_family_c66_TimestampC66XGlobal__internalaccess))

#ifndef ti_uia_family_c66_TimestampC66XGlobal__include_state
#define ti_uia_family_c66_TimestampC66XGlobal__include_state

/* Module_State */
struct ti_uia_family_c66_TimestampC66XGlobal_Module_State {
    xdc_runtime_Types_FreqHz freq;
};

/* Module__state__V */
#ifndef ti_uia_family_c66_TimestampC66XGlobal_Module__state__VR
extern struct ti_uia_family_c66_TimestampC66XGlobal_Module_State__ ti_uia_family_c66_TimestampC66XGlobal_Module__state__V;
#else
#define ti_uia_family_c66_TimestampC66XGlobal_Module__state__V (*((struct ti_uia_family_c66_TimestampC66XGlobal_Module_State__*)(xdcRomStatePtr + ti_uia_family_c66_TimestampC66XGlobal_Module__state__V_offset)))
#endif

/* Object */
struct ti_uia_family_c66_TimestampC66XGlobal_Object {
    const ti_uia_family_c66_TimestampC66XGlobal_Fxns__ *__fxns;
};

#endif /* ti_uia_family_c66_TimestampC66XGlobal__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_uia_family_c66_TimestampC66XGlobal__nolocalnames)

#ifndef ti_uia_family_c66_TimestampC66XGlobal__localnames__done
#define ti_uia_family_c66_TimestampC66XGlobal__localnames__done

/* module prefix */
#define TimestampC66XGlobal_Instance ti_uia_family_c66_TimestampC66XGlobal_Instance
#define TimestampC66XGlobal_Handle ti_uia_family_c66_TimestampC66XGlobal_Handle
#define TimestampC66XGlobal_Module ti_uia_family_c66_TimestampC66XGlobal_Module
#define TimestampC66XGlobal_Object ti_uia_family_c66_TimestampC66XGlobal_Object
#define TimestampC66XGlobal_Struct ti_uia_family_c66_TimestampC66XGlobal_Struct
#define TimestampC66XGlobal_Module_State ti_uia_family_c66_TimestampC66XGlobal_Module_State
#define TimestampC66XGlobal_Instance_State ti_uia_family_c66_TimestampC66XGlobal_Instance_State
#define TimestampC66XGlobal_timerBaseAdrs ti_uia_family_c66_TimestampC66XGlobal_timerBaseAdrs
#define TimestampC66XGlobal_maxTimerClockFreq ti_uia_family_c66_TimestampC66XGlobal_maxTimerClockFreq
#define TimestampC66XGlobal_maxBusClockFreq ti_uia_family_c66_TimestampC66XGlobal_maxBusClockFreq
#define TimestampC66XGlobal_Params ti_uia_family_c66_TimestampC66XGlobal_Params
#define TimestampC66XGlobal_get32 ti_uia_family_c66_TimestampC66XGlobal_get32
#define TimestampC66XGlobal_get64 ti_uia_family_c66_TimestampC66XGlobal_get64
#define TimestampC66XGlobal_getFreq ti_uia_family_c66_TimestampC66XGlobal_getFreq
#define TimestampC66XGlobal_setFreq ti_uia_family_c66_TimestampC66XGlobal_setFreq
#define TimestampC66XGlobal_Module_name ti_uia_family_c66_TimestampC66XGlobal_Module_name
#define TimestampC66XGlobal_Module_id ti_uia_family_c66_TimestampC66XGlobal_Module_id
#define TimestampC66XGlobal_Module_startup ti_uia_family_c66_TimestampC66XGlobal_Module_startup
#define TimestampC66XGlobal_Module_startupDone ti_uia_family_c66_TimestampC66XGlobal_Module_startupDone
#define TimestampC66XGlobal_Module_hasMask ti_uia_family_c66_TimestampC66XGlobal_Module_hasMask
#define TimestampC66XGlobal_Module_getMask ti_uia_family_c66_TimestampC66XGlobal_Module_getMask
#define TimestampC66XGlobal_Module_setMask ti_uia_family_c66_TimestampC66XGlobal_Module_setMask
#define TimestampC66XGlobal_Object_heap ti_uia_family_c66_TimestampC66XGlobal_Object_heap
#define TimestampC66XGlobal_Module_heap ti_uia_family_c66_TimestampC66XGlobal_Module_heap
#define TimestampC66XGlobal_construct ti_uia_family_c66_TimestampC66XGlobal_construct
#define TimestampC66XGlobal_create ti_uia_family_c66_TimestampC66XGlobal_create
#define TimestampC66XGlobal_handle ti_uia_family_c66_TimestampC66XGlobal_handle
#define TimestampC66XGlobal_struct ti_uia_family_c66_TimestampC66XGlobal_struct
#define TimestampC66XGlobal_Handle_label ti_uia_family_c66_TimestampC66XGlobal_Handle_label
#define TimestampC66XGlobal_Handle_name ti_uia_family_c66_TimestampC66XGlobal_Handle_name
#define TimestampC66XGlobal_Instance_init ti_uia_family_c66_TimestampC66XGlobal_Instance_init
#define TimestampC66XGlobal_Object_count ti_uia_family_c66_TimestampC66XGlobal_Object_count
#define TimestampC66XGlobal_Object_get ti_uia_family_c66_TimestampC66XGlobal_Object_get
#define TimestampC66XGlobal_Object_first ti_uia_family_c66_TimestampC66XGlobal_Object_first
#define TimestampC66XGlobal_Object_next ti_uia_family_c66_TimestampC66XGlobal_Object_next
#define TimestampC66XGlobal_Object_sizeof ti_uia_family_c66_TimestampC66XGlobal_Object_sizeof
#define TimestampC66XGlobal_Params_copy ti_uia_family_c66_TimestampC66XGlobal_Params_copy
#define TimestampC66XGlobal_Params_init ti_uia_family_c66_TimestampC66XGlobal_Params_init
#define TimestampC66XGlobal_Instance_State ti_uia_family_c66_TimestampC66XGlobal_Instance_State
#define TimestampC66XGlobal_delete ti_uia_family_c66_TimestampC66XGlobal_delete
#define TimestampC66XGlobal_destruct ti_uia_family_c66_TimestampC66XGlobal_destruct
#define TimestampC66XGlobal_Module_upCast ti_uia_family_c66_TimestampC66XGlobal_Module_upCast
#define TimestampC66XGlobal_Module_to_ti_uia_runtime_IUIATimestampProvider ti_uia_family_c66_TimestampC66XGlobal_Module_to_ti_uia_runtime_IUIATimestampProvider
#define TimestampC66XGlobal_Module_upCast2 ti_uia_family_c66_TimestampC66XGlobal_Module_upCast2
#define TimestampC66XGlobal_Module_to_xdc_runtime_ITimestampProvider ti_uia_family_c66_TimestampC66XGlobal_Module_to_xdc_runtime_ITimestampProvider
#define TimestampC66XGlobal_Module_upCast3 ti_uia_family_c66_TimestampC66XGlobal_Module_upCast3
#define TimestampC66XGlobal_Module_to_xdc_runtime_ITimestampClient ti_uia_family_c66_TimestampC66XGlobal_Module_to_xdc_runtime_ITimestampClient

#endif /* ti_uia_family_c66_TimestampC66XGlobal__localnames__done */
#endif
