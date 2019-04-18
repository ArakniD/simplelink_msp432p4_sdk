/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-F09
 */

#ifndef ti_sysbios_BIOS__INTERNAL__
#define ti_sysbios_BIOS__INTERNAL__

#ifndef ti_sysbios_BIOS__internalaccess
#define ti_sysbios_BIOS__internalaccess
#endif

#include <ti/sysbios/BIOS.h>

#undef xdc_FILE__
#ifndef xdc_FILE
#define xdc_FILE__ NULL
#else
#define xdc_FILE__ xdc_FILE
#endif

/* linkedWithIncorrectBootLibrary */
#undef ti_sysbios_BIOS_linkedWithIncorrectBootLibrary
#define ti_sysbios_BIOS_linkedWithIncorrectBootLibrary ti_sysbios_BIOS_linkedWithIncorrectBootLibrary__E

/* start */
#undef ti_sysbios_BIOS_start
#define ti_sysbios_BIOS_start ti_sysbios_BIOS_start__E

/* exit */
#undef ti_sysbios_BIOS_exit
#define ti_sysbios_BIOS_exit ti_sysbios_BIOS_exit__E

/* getThreadType */
#undef ti_sysbios_BIOS_getThreadType
#define ti_sysbios_BIOS_getThreadType ti_sysbios_BIOS_getThreadType__E

/* setThreadType */
#undef ti_sysbios_BIOS_setThreadType
#define ti_sysbios_BIOS_setThreadType ti_sysbios_BIOS_setThreadType__E

/* setCpuFreq */
#undef ti_sysbios_BIOS_setCpuFreq
#define ti_sysbios_BIOS_setCpuFreq ti_sysbios_BIOS_setCpuFreq__E

/* getCpuFreq */
#undef ti_sysbios_BIOS_getCpuFreq
#define ti_sysbios_BIOS_getCpuFreq ti_sysbios_BIOS_getCpuFreq__E

/* errorRaiseHook */
#define BIOS_errorRaiseHook ti_sysbios_BIOS_errorRaiseHook__I

/* startFunc */
#define BIOS_startFunc ti_sysbios_BIOS_startFunc__I

/* atExitFunc */
#define BIOS_atExitFunc ti_sysbios_BIOS_atExitFunc__I

/* exitFunc */
#define BIOS_exitFunc ti_sysbios_BIOS_exitFunc__I

/* registerRTSLock */
#define BIOS_registerRTSLock ti_sysbios_BIOS_registerRTSLock__I

/* removeRTSLock */
#define BIOS_removeRTSLock ti_sysbios_BIOS_removeRTSLock__I

/* rtsLock */
#define BIOS_rtsLock ti_sysbios_BIOS_rtsLock__I

/* rtsUnlock */
#define BIOS_rtsUnlock ti_sysbios_BIOS_rtsUnlock__I

/* nullFunc */
#define BIOS_nullFunc ti_sysbios_BIOS_nullFunc__I

/* Module_startup */
#undef ti_sysbios_BIOS_Module_startup
#define ti_sysbios_BIOS_Module_startup ti_sysbios_BIOS_Module_startup__E

/* Instance_init */
#undef ti_sysbios_BIOS_Instance_init
#define ti_sysbios_BIOS_Instance_init ti_sysbios_BIOS_Instance_init__E

/* Instance_finalize */
#undef ti_sysbios_BIOS_Instance_finalize
#define ti_sysbios_BIOS_Instance_finalize ti_sysbios_BIOS_Instance_finalize__E

/* module */
#ifdef ti_sysbios_BIOS_Module__state__VR
#define BIOS_module ((ti_sysbios_BIOS_Module_State *)(xdcRomStatePtr + ti_sysbios_BIOS_Module__state__V_offset))
#define module ((ti_sysbios_BIOS_Module_State *)(xdcRomStatePtr + ti_sysbios_BIOS_Module__state__V_offset))
#else
#define BIOS_module ((ti_sysbios_BIOS_Module_State *)(xdc__MODOBJADDR__(ti_sysbios_BIOS_Module__state__V)))
#if !defined(__cplusplus) ||!defined(ti_sysbios_BIOS__cplusplus)
#define module ((ti_sysbios_BIOS_Module_State *)(xdc__MODOBJADDR__(ti_sysbios_BIOS_Module__state__V)))
#endif
#endif

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_BIOS_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_BIOS_Module__id *)(xdcRomConstPtr + ti_sysbios_BIOS_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_BIOS_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_BIOS_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_BIOS_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_BIOS_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_BIOS_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_BIOS_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_BIOS_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_BIOS_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_BIOS_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_BIOS_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_BIOS_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_BIOS_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_BIOS_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_BIOS_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_BIOS_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_BIOS_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_BIOS_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_BIOS_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_BIOS_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_BIOS_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_BIOS_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_BIOS_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_BIOS_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_BIOS_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_BIOS_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_BIOS_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_BIOS_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_BIOS_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_BIOS_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_BIOS_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_BIOS_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_BIOS_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_BIOS_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_BIOS_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_BIOS_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_BIOS_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_BIOS_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_BIOS_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_BIOS_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_BIOS_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_BIOS_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_BIOS_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_BIOS_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_BIOS_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_BIOS_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_BIOS_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_BIOS_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_BIOS_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_BIOS_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_BIOS_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_BIOS_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_BIOS_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_BIOS_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_BIOS_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_BIOS_Module_GateProxy_query


#endif /* ti_sysbios_BIOS__INTERNAL____ */
