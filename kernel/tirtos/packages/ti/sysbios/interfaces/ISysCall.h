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
 *     VIRTUAL FUNCTIONS
 *     FUNCTION STUBS
 *     FUNCTION SELECTORS
 *
 *     EPILOGUE
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef ti_sysbios_interfaces_ISysCall__include
#define ti_sysbios_interfaces_ISysCall__include

#ifndef __nested__
#define __nested__
#define ti_sysbios_interfaces_ISysCall__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_sysbios_interfaces_ISysCall___VERS 200


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/interfaces/package/package.defs.h>

#include <xdc/runtime/IModule.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_sysbios_interfaces_ISysCall_Fxns__ {
    const xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2* __sysp;
    xdc_Void (*enterPrivMode)(void);
    xdc_Void (*enterUnprivMode)(void);
    xdc_Void (*restorePrivMode)(void);
    xdc_runtime_Types_SysFxns2 __sfxns;
};
#ifndef ti_sysbios_interfaces_ISysCall_Module__BASE__CR

/* Interface__BASE__C */
__extern const xdc_runtime_Types_Base ti_sysbios_interfaces_ISysCall_Interface__BASE__C;
#else
#define ti_sysbios_interfaces_ISysCall_Interface__BASE__C (*((xdc_runtime_Types_Base *)(xdcRomConstPtr + ti_sysbios_interfaces_ISysCall_Interface__BASE__C_offset)))
#endif


/*
 * ======== FUNCTION STUBS ========
 */

/* Module_id */
static inline xdc_runtime_Types_ModuleId ti_sysbios_interfaces_ISysCall_Module_id(ti_sysbios_interfaces_ISysCall_Module mod);
static inline xdc_runtime_Types_ModuleId ti_sysbios_interfaces_ISysCall_Module_id(ti_sysbios_interfaces_ISysCall_Module mod)
{
    return mod->__sysp->__mid;
}

/* enterPrivMode */
static inline xdc_Void ti_sysbios_interfaces_ISysCall_enterPrivMode(ti_sysbios_interfaces_ISysCall_Module mod);
static inline xdc_Void ti_sysbios_interfaces_ISysCall_enterPrivMode( ti_sysbios_interfaces_ISysCall_Module mod )
{
    mod->enterPrivMode();
}

/* enterUnprivMode */
static inline xdc_Void ti_sysbios_interfaces_ISysCall_enterUnprivMode(ti_sysbios_interfaces_ISysCall_Module mod);
static inline xdc_Void ti_sysbios_interfaces_ISysCall_enterUnprivMode( ti_sysbios_interfaces_ISysCall_Module mod )
{
    mod->enterUnprivMode();
}

/* restorePrivMode */
static inline xdc_Void ti_sysbios_interfaces_ISysCall_restorePrivMode(ti_sysbios_interfaces_ISysCall_Module mod);
static inline xdc_Void ti_sysbios_interfaces_ISysCall_restorePrivMode( ti_sysbios_interfaces_ISysCall_Module mod )
{
    mod->restorePrivMode();
}


/*
 * ======== FUNCTION SELECTORS ========
 */

/* These functions return function pointers for module and instance functions.
 * The functions accept modules and instances declared as types defined in this
 * interface, but they return functions defined for the actual objects passed
 * as parameters. These functions are not invoked by any generated code or
 * XDCtools internal code.
 */

/* enterPrivMode_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_interfaces_ISysCall_enterPrivMode_FxnT)(void);
static inline ti_sysbios_interfaces_ISysCall_enterPrivMode_FxnT ti_sysbios_interfaces_ISysCall_enterPrivMode_fxnP(ti_sysbios_interfaces_ISysCall_Module mod);
static inline ti_sysbios_interfaces_ISysCall_enterPrivMode_FxnT ti_sysbios_interfaces_ISysCall_enterPrivMode_fxnP(ti_sysbios_interfaces_ISysCall_Module mod)
{
    return (ti_sysbios_interfaces_ISysCall_enterPrivMode_FxnT)mod->enterPrivMode;
}

/* enterUnprivMode_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_interfaces_ISysCall_enterUnprivMode_FxnT)(void);
static inline ti_sysbios_interfaces_ISysCall_enterUnprivMode_FxnT ti_sysbios_interfaces_ISysCall_enterUnprivMode_fxnP(ti_sysbios_interfaces_ISysCall_Module mod);
static inline ti_sysbios_interfaces_ISysCall_enterUnprivMode_FxnT ti_sysbios_interfaces_ISysCall_enterUnprivMode_fxnP(ti_sysbios_interfaces_ISysCall_Module mod)
{
    return (ti_sysbios_interfaces_ISysCall_enterUnprivMode_FxnT)mod->enterUnprivMode;
}

/* restorePrivMode_{FxnT,fxnP} */
typedef xdc_Void (*ti_sysbios_interfaces_ISysCall_restorePrivMode_FxnT)(void);
static inline ti_sysbios_interfaces_ISysCall_restorePrivMode_FxnT ti_sysbios_interfaces_ISysCall_restorePrivMode_fxnP(ti_sysbios_interfaces_ISysCall_Module mod);
static inline ti_sysbios_interfaces_ISysCall_restorePrivMode_FxnT ti_sysbios_interfaces_ISysCall_restorePrivMode_fxnP(ti_sysbios_interfaces_ISysCall_Module mod)
{
    return (ti_sysbios_interfaces_ISysCall_restorePrivMode_FxnT)mod->restorePrivMode;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_sysbios_interfaces_ISysCall__top__
#undef __nested__
#endif

#endif /* ti_sysbios_interfaces_ISysCall__include */


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_sysbios_interfaces_ISysCall__nolocalnames)

#ifndef ti_sysbios_interfaces_ISysCall__localnames__done
#define ti_sysbios_interfaces_ISysCall__localnames__done

/* module prefix */
#define ISysCall_Module ti_sysbios_interfaces_ISysCall_Module
#define ISysCall_enterPrivMode ti_sysbios_interfaces_ISysCall_enterPrivMode
#define ISysCall_enterPrivMode_fxnP ti_sysbios_interfaces_ISysCall_enterPrivMode_fxnP
#define ISysCall_enterPrivMode_FxnT ti_sysbios_interfaces_ISysCall_enterPrivMode_FxnT
#define ISysCall_enterUnprivMode ti_sysbios_interfaces_ISysCall_enterUnprivMode
#define ISysCall_enterUnprivMode_fxnP ti_sysbios_interfaces_ISysCall_enterUnprivMode_fxnP
#define ISysCall_enterUnprivMode_FxnT ti_sysbios_interfaces_ISysCall_enterUnprivMode_FxnT
#define ISysCall_restorePrivMode ti_sysbios_interfaces_ISysCall_restorePrivMode
#define ISysCall_restorePrivMode_fxnP ti_sysbios_interfaces_ISysCall_restorePrivMode_fxnP
#define ISysCall_restorePrivMode_FxnT ti_sysbios_interfaces_ISysCall_restorePrivMode_FxnT
#define ISysCall_Module_name ti_sysbios_interfaces_ISysCall_Module_name

#endif /* ti_sysbios_interfaces_ISysCall__localnames__done */
#endif
