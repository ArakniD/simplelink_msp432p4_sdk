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

#ifndef ti_sysbios_family_c7x_Cache__include
#define ti_sysbios_family_c7x_Cache__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_family_c7x_Cache__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_family_c7x_Cache___VERS 200


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/family/c7x/package/package.defs.h>

#include <ti/sysbios/interfaces/ICache.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* Type */
typedef ti_sysbios_interfaces_ICache_Type ti_sysbios_family_c7x_Cache_Type;

/* Mode */
enum ti_sysbios_family_c7x_Cache_Mode {
    ti_sysbios_family_c7x_Cache_Mode_FREEZE,
    ti_sysbios_family_c7x_Cache_Mode_BYPASS,
    ti_sysbios_family_c7x_Cache_Mode_NORMAL
};
typedef enum ti_sysbios_family_c7x_Cache_Mode ti_sysbios_family_c7x_Cache_Mode;

/* L1Size */
enum ti_sysbios_family_c7x_Cache_L1Size {
    ti_sysbios_family_c7x_Cache_L1Size_0K = 0,
    ti_sysbios_family_c7x_Cache_L1Size_4K = 1,
    ti_sysbios_family_c7x_Cache_L1Size_8K = 2,
    ti_sysbios_family_c7x_Cache_L1Size_16K = 3,
    ti_sysbios_family_c7x_Cache_L1Size_32K = 4
};
typedef enum ti_sysbios_family_c7x_Cache_L1Size ti_sysbios_family_c7x_Cache_L1Size;

/* L2Size */
enum ti_sysbios_family_c7x_Cache_L2Size {
    ti_sysbios_family_c7x_Cache_L2Size_0K = 0,
    ti_sysbios_family_c7x_Cache_L2Size_32K = 1,
    ti_sysbios_family_c7x_Cache_L2Size_64K = 2,
    ti_sysbios_family_c7x_Cache_L2Size_128K = 3,
    ti_sysbios_family_c7x_Cache_L2Size_256K = 4,
    ti_sysbios_family_c7x_Cache_L2Size_512K = 5,
    ti_sysbios_family_c7x_Cache_L2Size_1024K = 6
};
typedef enum ti_sysbios_family_c7x_Cache_L2Size ti_sysbios_family_c7x_Cache_L2Size;

/* PC */
#define ti_sysbios_family_c7x_Cache_PC (1)

/* WTE */
#define ti_sysbios_family_c7x_Cache_WTE (2)

/* PCX */
#define ti_sysbios_family_c7x_Cache_PCX (4)

/* PFX */
#define ti_sysbios_family_c7x_Cache_PFX (8)

/* Size */
struct ti_sysbios_family_c7x_Cache_Size {
    ti_sysbios_family_c7x_Cache_L1Size l1pSize;
    ti_sysbios_family_c7x_Cache_L1Size l1dSize;
    ti_sysbios_family_c7x_Cache_L2Size l2Size;
};

/* Type_L1P */
#define ti_sysbios_family_c7x_Cache_Type_L1P ti_sysbios_interfaces_ICache_Type_L1P

/* Type_L1D */
#define ti_sysbios_family_c7x_Cache_Type_L1D ti_sysbios_interfaces_ICache_Type_L1D

/* Type_L1 */
#define ti_sysbios_family_c7x_Cache_Type_L1 ti_sysbios_interfaces_ICache_Type_L1

/* Type_L2P */
#define ti_sysbios_family_c7x_Cache_Type_L2P ti_sysbios_interfaces_ICache_Type_L2P

/* Type_L2D */
#define ti_sysbios_family_c7x_Cache_Type_L2D ti_sysbios_interfaces_ICache_Type_L2D

/* Type_L2 */
#define ti_sysbios_family_c7x_Cache_Type_L2 ti_sysbios_interfaces_ICache_Type_L2

/* Type_ALLP */
#define ti_sysbios_family_c7x_Cache_Type_ALLP ti_sysbios_interfaces_ICache_Type_ALLP

/* Type_ALLD */
#define ti_sysbios_family_c7x_Cache_Type_ALLD ti_sysbios_interfaces_ICache_Type_ALLD

/* Type_ALL */
#define ti_sysbios_family_c7x_Cache_Type_ALL ti_sysbios_interfaces_ICache_Type_ALL


/*
 * ======== INTERNAL DEFINITIONS ========
 */


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_sysbios_family_c7x_Cache_Module__diagsEnabled;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Module__diagsEnabled ti_sysbios_family_c7x_Cache_Module__diagsEnabled__C;
#ifdef ti_sysbios_family_c7x_Cache_Module__diagsEnabled__CR
#define ti_sysbios_family_c7x_Cache_Module__diagsEnabled__C (*((CT__ti_sysbios_family_c7x_Cache_Module__diagsEnabled*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__diagsEnabled__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Module__diagsEnabled (ti_sysbios_family_c7x_Cache_Module__diagsEnabled__C)
#endif

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_sysbios_family_c7x_Cache_Module__diagsIncluded;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Module__diagsIncluded ti_sysbios_family_c7x_Cache_Module__diagsIncluded__C;
#ifdef ti_sysbios_family_c7x_Cache_Module__diagsIncluded__CR
#define ti_sysbios_family_c7x_Cache_Module__diagsIncluded__C (*((CT__ti_sysbios_family_c7x_Cache_Module__diagsIncluded*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__diagsIncluded__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Module__diagsIncluded (ti_sysbios_family_c7x_Cache_Module__diagsIncluded__C)
#endif

/* Module__diagsMask */
typedef xdc_Bits16 *CT__ti_sysbios_family_c7x_Cache_Module__diagsMask;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Module__diagsMask ti_sysbios_family_c7x_Cache_Module__diagsMask__C;
#ifdef ti_sysbios_family_c7x_Cache_Module__diagsMask__CR
#define ti_sysbios_family_c7x_Cache_Module__diagsMask__C (*((CT__ti_sysbios_family_c7x_Cache_Module__diagsMask*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__diagsMask__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Module__diagsMask (ti_sysbios_family_c7x_Cache_Module__diagsMask__C)
#endif

/* Module__gateObj */
typedef xdc_Ptr CT__ti_sysbios_family_c7x_Cache_Module__gateObj;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Module__gateObj ti_sysbios_family_c7x_Cache_Module__gateObj__C;
#ifdef ti_sysbios_family_c7x_Cache_Module__gateObj__CR
#define ti_sysbios_family_c7x_Cache_Module__gateObj__C (*((CT__ti_sysbios_family_c7x_Cache_Module__gateObj*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__gateObj__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Module__gateObj (ti_sysbios_family_c7x_Cache_Module__gateObj__C)
#endif

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_sysbios_family_c7x_Cache_Module__gatePrms;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Module__gatePrms ti_sysbios_family_c7x_Cache_Module__gatePrms__C;
#ifdef ti_sysbios_family_c7x_Cache_Module__gatePrms__CR
#define ti_sysbios_family_c7x_Cache_Module__gatePrms__C (*((CT__ti_sysbios_family_c7x_Cache_Module__gatePrms*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__gatePrms__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Module__gatePrms (ti_sysbios_family_c7x_Cache_Module__gatePrms__C)
#endif

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_sysbios_family_c7x_Cache_Module__id;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Module__id ti_sysbios_family_c7x_Cache_Module__id__C;
#ifdef ti_sysbios_family_c7x_Cache_Module__id__CR
#define ti_sysbios_family_c7x_Cache_Module__id__C (*((CT__ti_sysbios_family_c7x_Cache_Module__id*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__id__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Module__id (ti_sysbios_family_c7x_Cache_Module__id__C)
#endif

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_sysbios_family_c7x_Cache_Module__loggerDefined;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Module__loggerDefined ti_sysbios_family_c7x_Cache_Module__loggerDefined__C;
#ifdef ti_sysbios_family_c7x_Cache_Module__loggerDefined__CR
#define ti_sysbios_family_c7x_Cache_Module__loggerDefined__C (*((CT__ti_sysbios_family_c7x_Cache_Module__loggerDefined*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__loggerDefined__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Module__loggerDefined (ti_sysbios_family_c7x_Cache_Module__loggerDefined__C)
#endif

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_sysbios_family_c7x_Cache_Module__loggerObj;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Module__loggerObj ti_sysbios_family_c7x_Cache_Module__loggerObj__C;
#ifdef ti_sysbios_family_c7x_Cache_Module__loggerObj__CR
#define ti_sysbios_family_c7x_Cache_Module__loggerObj__C (*((CT__ti_sysbios_family_c7x_Cache_Module__loggerObj*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__loggerObj__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Module__loggerObj (ti_sysbios_family_c7x_Cache_Module__loggerObj__C)
#endif

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn0;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn0 ti_sysbios_family_c7x_Cache_Module__loggerFxn0__C;
#ifdef ti_sysbios_family_c7x_Cache_Module__loggerFxn0__CR
#define ti_sysbios_family_c7x_Cache_Module__loggerFxn0__C (*((CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn0*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__loggerFxn0__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Module__loggerFxn0 (ti_sysbios_family_c7x_Cache_Module__loggerFxn0__C)
#endif

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn1;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn1 ti_sysbios_family_c7x_Cache_Module__loggerFxn1__C;
#ifdef ti_sysbios_family_c7x_Cache_Module__loggerFxn1__CR
#define ti_sysbios_family_c7x_Cache_Module__loggerFxn1__C (*((CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn1*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__loggerFxn1__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Module__loggerFxn1 (ti_sysbios_family_c7x_Cache_Module__loggerFxn1__C)
#endif

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn2;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn2 ti_sysbios_family_c7x_Cache_Module__loggerFxn2__C;
#ifdef ti_sysbios_family_c7x_Cache_Module__loggerFxn2__CR
#define ti_sysbios_family_c7x_Cache_Module__loggerFxn2__C (*((CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn2*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__loggerFxn2__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Module__loggerFxn2 (ti_sysbios_family_c7x_Cache_Module__loggerFxn2__C)
#endif

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn4;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn4 ti_sysbios_family_c7x_Cache_Module__loggerFxn4__C;
#ifdef ti_sysbios_family_c7x_Cache_Module__loggerFxn4__CR
#define ti_sysbios_family_c7x_Cache_Module__loggerFxn4__C (*((CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn4*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__loggerFxn4__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Module__loggerFxn4 (ti_sysbios_family_c7x_Cache_Module__loggerFxn4__C)
#endif

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn8;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn8 ti_sysbios_family_c7x_Cache_Module__loggerFxn8__C;
#ifdef ti_sysbios_family_c7x_Cache_Module__loggerFxn8__CR
#define ti_sysbios_family_c7x_Cache_Module__loggerFxn8__C (*((CT__ti_sysbios_family_c7x_Cache_Module__loggerFxn8*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__loggerFxn8__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Module__loggerFxn8 (ti_sysbios_family_c7x_Cache_Module__loggerFxn8__C)
#endif

/* Object__count */
typedef xdc_Int CT__ti_sysbios_family_c7x_Cache_Object__count;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Object__count ti_sysbios_family_c7x_Cache_Object__count__C;
#ifdef ti_sysbios_family_c7x_Cache_Object__count__CR
#define ti_sysbios_family_c7x_Cache_Object__count__C (*((CT__ti_sysbios_family_c7x_Cache_Object__count*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Object__count__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Object__count (ti_sysbios_family_c7x_Cache_Object__count__C)
#endif

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_sysbios_family_c7x_Cache_Object__heap;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Object__heap ti_sysbios_family_c7x_Cache_Object__heap__C;
#ifdef ti_sysbios_family_c7x_Cache_Object__heap__CR
#define ti_sysbios_family_c7x_Cache_Object__heap__C (*((CT__ti_sysbios_family_c7x_Cache_Object__heap*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Object__heap__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Object__heap (ti_sysbios_family_c7x_Cache_Object__heap__C)
#endif

/* Object__sizeof */
typedef xdc_SizeT CT__ti_sysbios_family_c7x_Cache_Object__sizeof;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Object__sizeof ti_sysbios_family_c7x_Cache_Object__sizeof__C;
#ifdef ti_sysbios_family_c7x_Cache_Object__sizeof__CR
#define ti_sysbios_family_c7x_Cache_Object__sizeof__C (*((CT__ti_sysbios_family_c7x_Cache_Object__sizeof*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Object__sizeof__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Object__sizeof (ti_sysbios_family_c7x_Cache_Object__sizeof__C)
#endif

/* Object__table */
typedef xdc_Ptr CT__ti_sysbios_family_c7x_Cache_Object__table;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_Object__table ti_sysbios_family_c7x_Cache_Object__table__C;
#ifdef ti_sysbios_family_c7x_Cache_Object__table__CR
#define ti_sysbios_family_c7x_Cache_Object__table__C (*((CT__ti_sysbios_family_c7x_Cache_Object__table*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Object__table__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_Object__table (ti_sysbios_family_c7x_Cache_Object__table__C)
#endif

/* initSize */
typedef ti_sysbios_family_c7x_Cache_Size CT__ti_sysbios_family_c7x_Cache_initSize;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_initSize ti_sysbios_family_c7x_Cache_initSize__C;
#ifdef ti_sysbios_family_c7x_Cache_initSize__CR
#define ti_sysbios_family_c7x_Cache_initSize (*((CT__ti_sysbios_family_c7x_Cache_initSize*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_initSize__C_offset)))
#else
#define ti_sysbios_family_c7x_Cache_initSize (ti_sysbios_family_c7x_Cache_initSize__C)
#endif

/* atomicBlockSize */
typedef xdc_UInt32 CT__ti_sysbios_family_c7x_Cache_atomicBlockSize;
__extern __FAR__ const CT__ti_sysbios_family_c7x_Cache_atomicBlockSize ti_sysbios_family_c7x_Cache_atomicBlockSize__C;
#ifdef ti_sysbios_family_c7x_Cache_atomicBlockSize__CR
#define ti_sysbios_family_c7x_Cache_atomicBlockSize (*((CT__ti_sysbios_family_c7x_Cache_atomicBlockSize*)(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_atomicBlockSize__C_offset)))
#else
#ifdef ti_sysbios_family_c7x_Cache_atomicBlockSize__D
#define ti_sysbios_family_c7x_Cache_atomicBlockSize (ti_sysbios_family_c7x_Cache_atomicBlockSize__D)
#else
#define ti_sysbios_family_c7x_Cache_atomicBlockSize (ti_sysbios_family_c7x_Cache_atomicBlockSize__C)
#endif
#endif


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_sysbios_family_c7x_Cache_Fxns__ {
    const xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2* __sysp;
    xdc_Void (*enable)(xdc_Bits16 type);
    xdc_Void (*disable)(xdc_Bits16 type);
    xdc_Void (*inv)(xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait);
    xdc_Void (*wb)(xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait);
    xdc_Void (*wbInv)(xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait);
    xdc_Void (*wbAll)(void);
    xdc_Void (*wbInvAll)(void);
    xdc_Void (*wait)(void);
    xdc_runtime_Types_SysFxns2 __sfxns;
};
#ifndef ti_sysbios_family_c7x_Cache_Module__FXNS__CR

/* Module__FXNS__C */
__extern const ti_sysbios_family_c7x_Cache_Fxns__ ti_sysbios_family_c7x_Cache_Module__FXNS__C;
#else
#define ti_sysbios_family_c7x_Cache_Module__FXNS__C (*(xdcRomConstPtr + ti_sysbios_family_c7x_Cache_Module__FXNS__C_offset))
#endif


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_sysbios_family_c7x_Cache_Module_startup( state ) (-1)

/* Module__startupDone__S */
xdc__CODESECT(ti_sysbios_family_c7x_Cache_Module__startupDone__S, "ti_sysbios_family_c7x_Cache_Module__startupDone__S")
__extern xdc_Bool ti_sysbios_family_c7x_Cache_Module__startupDone__S( void );

/* enable__E */
#define ti_sysbios_family_c7x_Cache_enable ti_sysbios_family_c7x_Cache_enable__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_enable__E, "ti_sysbios_family_c7x_Cache_enable")
__extern xdc_Void ti_sysbios_family_c7x_Cache_enable__E( xdc_Bits16 type );

/* inv__E */
#define ti_sysbios_family_c7x_Cache_inv ti_sysbios_family_c7x_Cache_inv__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_inv__E, "ti_sysbios_family_c7x_Cache_inv")
__extern xdc_Void ti_sysbios_family_c7x_Cache_inv__E( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );

/* wb__E */
#define ti_sysbios_family_c7x_Cache_wb ti_sysbios_family_c7x_Cache_wb__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_wb__E, "ti_sysbios_family_c7x_Cache_wb")
__extern xdc_Void ti_sysbios_family_c7x_Cache_wb__E( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );

/* wbInv__E */
#define ti_sysbios_family_c7x_Cache_wbInv ti_sysbios_family_c7x_Cache_wbInv__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_wbInv__E, "ti_sysbios_family_c7x_Cache_wbInv")
__extern xdc_Void ti_sysbios_family_c7x_Cache_wbInv__E( xdc_Ptr blockPtr, xdc_SizeT byteCnt, xdc_Bits16 type, xdc_Bool wait );

/* wait__E */
#define ti_sysbios_family_c7x_Cache_wait ti_sysbios_family_c7x_Cache_wait__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_wait__E, "ti_sysbios_family_c7x_Cache_wait")
__extern xdc_Void ti_sysbios_family_c7x_Cache_wait__E( void );

/* disable__E */
#define ti_sysbios_family_c7x_Cache_disable ti_sysbios_family_c7x_Cache_disable__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_disable__E, "ti_sysbios_family_c7x_Cache_disable")
__extern xdc_Void ti_sysbios_family_c7x_Cache_disable__E( xdc_Bits16 type );

/* getSize__E */
#define ti_sysbios_family_c7x_Cache_getSize ti_sysbios_family_c7x_Cache_getSize__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_getSize__E, "ti_sysbios_family_c7x_Cache_getSize")
__extern xdc_Void ti_sysbios_family_c7x_Cache_getSize__E( ti_sysbios_family_c7x_Cache_Size *size );

/* setSize__E */
#define ti_sysbios_family_c7x_Cache_setSize ti_sysbios_family_c7x_Cache_setSize__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_setSize__E, "ti_sysbios_family_c7x_Cache_setSize")
__extern xdc_Void ti_sysbios_family_c7x_Cache_setSize__E( ti_sysbios_family_c7x_Cache_Size *size );

/* wbAll__E */
#define ti_sysbios_family_c7x_Cache_wbAll ti_sysbios_family_c7x_Cache_wbAll__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_wbAll__E, "ti_sysbios_family_c7x_Cache_wbAll")
__extern xdc_Void ti_sysbios_family_c7x_Cache_wbAll__E( void );

/* wbL1dAll__E */
#define ti_sysbios_family_c7x_Cache_wbL1dAll ti_sysbios_family_c7x_Cache_wbL1dAll__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_wbL1dAll__E, "ti_sysbios_family_c7x_Cache_wbL1dAll")
__extern xdc_Void ti_sysbios_family_c7x_Cache_wbL1dAll__E( void );

/* wbInvAll__E */
#define ti_sysbios_family_c7x_Cache_wbInvAll ti_sysbios_family_c7x_Cache_wbInvAll__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_wbInvAll__E, "ti_sysbios_family_c7x_Cache_wbInvAll")
__extern xdc_Void ti_sysbios_family_c7x_Cache_wbInvAll__E( void );

/* wbInvL1dAll__E */
#define ti_sysbios_family_c7x_Cache_wbInvL1dAll ti_sysbios_family_c7x_Cache_wbInvL1dAll__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_wbInvL1dAll__E, "ti_sysbios_family_c7x_Cache_wbInvL1dAll")
__extern xdc_Void ti_sysbios_family_c7x_Cache_wbInvL1dAll__E( void );

/* setL2CFG__E */
#define ti_sysbios_family_c7x_Cache_setL2CFG ti_sysbios_family_c7x_Cache_setL2CFG__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_setL2CFG__E, "ti_sysbios_family_c7x_Cache_setL2CFG")
__extern xdc_Void ti_sysbios_family_c7x_Cache_setL2CFG__E( xdc_UInt size );

/* getL2CFG__E */
#define ti_sysbios_family_c7x_Cache_getL2CFG ti_sysbios_family_c7x_Cache_getL2CFG__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_getL2CFG__E, "ti_sysbios_family_c7x_Cache_getL2CFG")
__extern xdc_ULong ti_sysbios_family_c7x_Cache_getL2CFG__E( void );

/* setL1DCFG__E */
#define ti_sysbios_family_c7x_Cache_setL1DCFG ti_sysbios_family_c7x_Cache_setL1DCFG__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_setL1DCFG__E, "ti_sysbios_family_c7x_Cache_setL1DCFG")
__extern xdc_Void ti_sysbios_family_c7x_Cache_setL1DCFG__E( xdc_UInt size );

/* getL1DCFG__E */
#define ti_sysbios_family_c7x_Cache_getL1DCFG ti_sysbios_family_c7x_Cache_getL1DCFG__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_getL1DCFG__E, "ti_sysbios_family_c7x_Cache_getL1DCFG")
__extern xdc_ULong ti_sysbios_family_c7x_Cache_getL1DCFG__E( void );

/* setL2WB__E */
#define ti_sysbios_family_c7x_Cache_setL2WB ti_sysbios_family_c7x_Cache_setL2WB__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_setL2WB__E, "ti_sysbios_family_c7x_Cache_setL2WB")
__extern xdc_Void ti_sysbios_family_c7x_Cache_setL2WB__E( xdc_UInt flag );

/* setL2WBINV__E */
#define ti_sysbios_family_c7x_Cache_setL2WBINV ti_sysbios_family_c7x_Cache_setL2WBINV__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_setL2WBINV__E, "ti_sysbios_family_c7x_Cache_setL2WBINV")
__extern xdc_Void ti_sysbios_family_c7x_Cache_setL2WBINV__E( xdc_UInt flag );

/* setL1DWB__E */
#define ti_sysbios_family_c7x_Cache_setL1DWB ti_sysbios_family_c7x_Cache_setL1DWB__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_setL1DWB__E, "ti_sysbios_family_c7x_Cache_setL1DWB")
__extern xdc_Void ti_sysbios_family_c7x_Cache_setL1DWB__E( xdc_UInt flag );

/* setL1DWBINV__E */
#define ti_sysbios_family_c7x_Cache_setL1DWBINV ti_sysbios_family_c7x_Cache_setL1DWBINV__E
xdc__CODESECT(ti_sysbios_family_c7x_Cache_setL1DWBINV__E, "ti_sysbios_family_c7x_Cache_setL1DWBINV")
__extern xdc_Void ti_sysbios_family_c7x_Cache_setL1DWBINV__E( xdc_UInt flag );

/* startup__I */
#define ti_sysbios_family_c7x_Cache_startup ti_sysbios_family_c7x_Cache_startup__I
xdc__CODESECT(ti_sysbios_family_c7x_Cache_startup__I, "ti_sysbios_family_c7x_Cache_startup")
__extern xdc_Void ti_sysbios_family_c7x_Cache_startup__I( void );


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline ti_sysbios_interfaces_ICache_Module ti_sysbios_family_c7x_Cache_Module_upCast(void);
static inline ti_sysbios_interfaces_ICache_Module ti_sysbios_family_c7x_Cache_Module_upCast(void)
{
    return (ti_sysbios_interfaces_ICache_Module)&ti_sysbios_family_c7x_Cache_Module__FXNS__C;
}

/* Module_to_ti_sysbios_interfaces_ICache */
#define ti_sysbios_family_c7x_Cache_Module_to_ti_sysbios_interfaces_ICache ti_sysbios_family_c7x_Cache_Module_upCast


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_sysbios_family_c7x_Cache_Module_startupDone() ti_sysbios_family_c7x_Cache_Module__startupDone__S()

/* Object_heap */
#define ti_sysbios_family_c7x_Cache_Object_heap() ti_sysbios_family_c7x_Cache_Object__heap__C

/* Module_heap */
#define ti_sysbios_family_c7x_Cache_Module_heap() ti_sysbios_family_c7x_Cache_Object__heap__C

/* Module_id */
static inline CT__ti_sysbios_family_c7x_Cache_Module__id ti_sysbios_family_c7x_Cache_Module_id(void);
static inline CT__ti_sysbios_family_c7x_Cache_Module__id ti_sysbios_family_c7x_Cache_Module_id( void ) 
{
    return ti_sysbios_family_c7x_Cache_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_sysbios_family_c7x_Cache_Module_hasMask(void);
static inline xdc_Bool ti_sysbios_family_c7x_Cache_Module_hasMask(void)
{
    return (xdc_Bool)(ti_sysbios_family_c7x_Cache_Module__diagsMask__C != (CT__ti_sysbios_family_c7x_Cache_Module__diagsMask)NULL);
}

/* Module_getMask */
static inline xdc_Bits16 ti_sysbios_family_c7x_Cache_Module_getMask(void);
static inline xdc_Bits16 ti_sysbios_family_c7x_Cache_Module_getMask(void)
{
    return (ti_sysbios_family_c7x_Cache_Module__diagsMask__C != (CT__ti_sysbios_family_c7x_Cache_Module__diagsMask)NULL) ? *ti_sysbios_family_c7x_Cache_Module__diagsMask__C : (xdc_Bits16)0;
}

/* Module_setMask */
static inline xdc_Void ti_sysbios_family_c7x_Cache_Module_setMask(xdc_Bits16 mask);
static inline xdc_Void ti_sysbios_family_c7x_Cache_Module_setMask(xdc_Bits16 mask)
{
    if (ti_sysbios_family_c7x_Cache_Module__diagsMask__C != (CT__ti_sysbios_family_c7x_Cache_Module__diagsMask)NULL) {
        *ti_sysbios_family_c7x_Cache_Module__diagsMask__C = mask;
    }
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_family_c7x_Cache__top__
#undef __nested__
#endif

#endif /* ti_sysbios_family_c7x_Cache__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_sysbios_family_c7x_Cache__internalaccess))

#ifndef ti_sysbios_family_c7x_Cache__include_state
#define ti_sysbios_family_c7x_Cache__include_state


#endif /* ti_sysbios_family_c7x_Cache__include_state */

#endif

/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_family_c7x_Cache__nolocalnames)

#ifndef ti_sysbios_family_c7x_Cache__localnames__done
#define ti_sysbios_family_c7x_Cache__localnames__done

/* module prefix */
#define Cache_Type ti_sysbios_family_c7x_Cache_Type
#define Cache_Mode ti_sysbios_family_c7x_Cache_Mode
#define Cache_L1Size ti_sysbios_family_c7x_Cache_L1Size
#define Cache_L2Size ti_sysbios_family_c7x_Cache_L2Size
#define Cache_PC ti_sysbios_family_c7x_Cache_PC
#define Cache_WTE ti_sysbios_family_c7x_Cache_WTE
#define Cache_PCX ti_sysbios_family_c7x_Cache_PCX
#define Cache_PFX ti_sysbios_family_c7x_Cache_PFX
#define Cache_Size ti_sysbios_family_c7x_Cache_Size
#define Cache_Type_L1P ti_sysbios_family_c7x_Cache_Type_L1P
#define Cache_Type_L1D ti_sysbios_family_c7x_Cache_Type_L1D
#define Cache_Type_L1 ti_sysbios_family_c7x_Cache_Type_L1
#define Cache_Type_L2P ti_sysbios_family_c7x_Cache_Type_L2P
#define Cache_Type_L2D ti_sysbios_family_c7x_Cache_Type_L2D
#define Cache_Type_L2 ti_sysbios_family_c7x_Cache_Type_L2
#define Cache_Type_ALLP ti_sysbios_family_c7x_Cache_Type_ALLP
#define Cache_Type_ALLD ti_sysbios_family_c7x_Cache_Type_ALLD
#define Cache_Type_ALL ti_sysbios_family_c7x_Cache_Type_ALL
#define Cache_Mode_FREEZE ti_sysbios_family_c7x_Cache_Mode_FREEZE
#define Cache_Mode_BYPASS ti_sysbios_family_c7x_Cache_Mode_BYPASS
#define Cache_Mode_NORMAL ti_sysbios_family_c7x_Cache_Mode_NORMAL
#define Cache_L1Size_0K ti_sysbios_family_c7x_Cache_L1Size_0K
#define Cache_L1Size_4K ti_sysbios_family_c7x_Cache_L1Size_4K
#define Cache_L1Size_8K ti_sysbios_family_c7x_Cache_L1Size_8K
#define Cache_L1Size_16K ti_sysbios_family_c7x_Cache_L1Size_16K
#define Cache_L1Size_32K ti_sysbios_family_c7x_Cache_L1Size_32K
#define Cache_L2Size_0K ti_sysbios_family_c7x_Cache_L2Size_0K
#define Cache_L2Size_32K ti_sysbios_family_c7x_Cache_L2Size_32K
#define Cache_L2Size_64K ti_sysbios_family_c7x_Cache_L2Size_64K
#define Cache_L2Size_128K ti_sysbios_family_c7x_Cache_L2Size_128K
#define Cache_L2Size_256K ti_sysbios_family_c7x_Cache_L2Size_256K
#define Cache_L2Size_512K ti_sysbios_family_c7x_Cache_L2Size_512K
#define Cache_L2Size_1024K ti_sysbios_family_c7x_Cache_L2Size_1024K
#define Cache_initSize ti_sysbios_family_c7x_Cache_initSize
#define Cache_atomicBlockSize ti_sysbios_family_c7x_Cache_atomicBlockSize
#define Cache_enable ti_sysbios_family_c7x_Cache_enable
#define Cache_inv ti_sysbios_family_c7x_Cache_inv
#define Cache_wb ti_sysbios_family_c7x_Cache_wb
#define Cache_wbInv ti_sysbios_family_c7x_Cache_wbInv
#define Cache_wait ti_sysbios_family_c7x_Cache_wait
#define Cache_disable ti_sysbios_family_c7x_Cache_disable
#define Cache_getSize ti_sysbios_family_c7x_Cache_getSize
#define Cache_setSize ti_sysbios_family_c7x_Cache_setSize
#define Cache_wbAll ti_sysbios_family_c7x_Cache_wbAll
#define Cache_wbL1dAll ti_sysbios_family_c7x_Cache_wbL1dAll
#define Cache_wbInvAll ti_sysbios_family_c7x_Cache_wbInvAll
#define Cache_wbInvL1dAll ti_sysbios_family_c7x_Cache_wbInvL1dAll
#define Cache_setL2CFG ti_sysbios_family_c7x_Cache_setL2CFG
#define Cache_getL2CFG ti_sysbios_family_c7x_Cache_getL2CFG
#define Cache_setL1DCFG ti_sysbios_family_c7x_Cache_setL1DCFG
#define Cache_getL1DCFG ti_sysbios_family_c7x_Cache_getL1DCFG
#define Cache_setL2WB ti_sysbios_family_c7x_Cache_setL2WB
#define Cache_setL2WBINV ti_sysbios_family_c7x_Cache_setL2WBINV
#define Cache_setL1DWB ti_sysbios_family_c7x_Cache_setL1DWB
#define Cache_setL1DWBINV ti_sysbios_family_c7x_Cache_setL1DWBINV
#define Cache_Module_name ti_sysbios_family_c7x_Cache_Module_name
#define Cache_Module_id ti_sysbios_family_c7x_Cache_Module_id
#define Cache_Module_startup ti_sysbios_family_c7x_Cache_Module_startup
#define Cache_Module_startupDone ti_sysbios_family_c7x_Cache_Module_startupDone
#define Cache_Module_hasMask ti_sysbios_family_c7x_Cache_Module_hasMask
#define Cache_Module_getMask ti_sysbios_family_c7x_Cache_Module_getMask
#define Cache_Module_setMask ti_sysbios_family_c7x_Cache_Module_setMask
#define Cache_Object_heap ti_sysbios_family_c7x_Cache_Object_heap
#define Cache_Module_heap ti_sysbios_family_c7x_Cache_Module_heap
#define Cache_Module_upCast ti_sysbios_family_c7x_Cache_Module_upCast
#define Cache_Module_to_ti_sysbios_interfaces_ICache ti_sysbios_family_c7x_Cache_Module_to_ti_sysbios_interfaces_ICache

#endif /* ti_sysbios_family_c7x_Cache__localnames__done */
#endif
