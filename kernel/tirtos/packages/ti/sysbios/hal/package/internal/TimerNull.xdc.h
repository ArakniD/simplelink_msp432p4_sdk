/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-F14
 */

#ifndef ti_sysbios_hal_TimerNull__INTERNAL__
#define ti_sysbios_hal_TimerNull__INTERNAL__

#ifndef ti_sysbios_hal_TimerNull__internalaccess
#define ti_sysbios_hal_TimerNull__internalaccess
#endif

#include <ti/sysbios/hal/TimerNull.h>

#undef xdc_FILE__
#ifndef xdc_FILE
#define xdc_FILE__ NULL
#else
#define xdc_FILE__ xdc_FILE
#endif

/* getNumTimers */
#undef ti_sysbios_hal_TimerNull_getNumTimers
#define ti_sysbios_hal_TimerNull_getNumTimers ti_sysbios_hal_TimerNull_getNumTimers__E

/* getStatus */
#undef ti_sysbios_hal_TimerNull_getStatus
#define ti_sysbios_hal_TimerNull_getStatus ti_sysbios_hal_TimerNull_getStatus__E

/* startup */
#undef ti_sysbios_hal_TimerNull_startup
#define ti_sysbios_hal_TimerNull_startup ti_sysbios_hal_TimerNull_startup__E

/* getMaxTicks */
#undef ti_sysbios_hal_TimerNull_getMaxTicks
#define ti_sysbios_hal_TimerNull_getMaxTicks ti_sysbios_hal_TimerNull_getMaxTicks__E

/* setNextTick */
#undef ti_sysbios_hal_TimerNull_setNextTick
#define ti_sysbios_hal_TimerNull_setNextTick ti_sysbios_hal_TimerNull_setNextTick__E

/* start */
#undef ti_sysbios_hal_TimerNull_start
#define ti_sysbios_hal_TimerNull_start ti_sysbios_hal_TimerNull_start__E

/* stop */
#undef ti_sysbios_hal_TimerNull_stop
#define ti_sysbios_hal_TimerNull_stop ti_sysbios_hal_TimerNull_stop__E

/* setPeriod */
#undef ti_sysbios_hal_TimerNull_setPeriod
#define ti_sysbios_hal_TimerNull_setPeriod ti_sysbios_hal_TimerNull_setPeriod__E

/* setPeriodMicroSecs */
#undef ti_sysbios_hal_TimerNull_setPeriodMicroSecs
#define ti_sysbios_hal_TimerNull_setPeriodMicroSecs ti_sysbios_hal_TimerNull_setPeriodMicroSecs__E

/* getPeriod */
#undef ti_sysbios_hal_TimerNull_getPeriod
#define ti_sysbios_hal_TimerNull_getPeriod ti_sysbios_hal_TimerNull_getPeriod__E

/* getCount */
#undef ti_sysbios_hal_TimerNull_getCount
#define ti_sysbios_hal_TimerNull_getCount ti_sysbios_hal_TimerNull_getCount__E

/* getFreq */
#undef ti_sysbios_hal_TimerNull_getFreq
#define ti_sysbios_hal_TimerNull_getFreq ti_sysbios_hal_TimerNull_getFreq__E

/* getFunc */
#undef ti_sysbios_hal_TimerNull_getFunc
#define ti_sysbios_hal_TimerNull_getFunc ti_sysbios_hal_TimerNull_getFunc__E

/* setFunc */
#undef ti_sysbios_hal_TimerNull_setFunc
#define ti_sysbios_hal_TimerNull_setFunc ti_sysbios_hal_TimerNull_setFunc__E

/* trigger */
#undef ti_sysbios_hal_TimerNull_trigger
#define ti_sysbios_hal_TimerNull_trigger ti_sysbios_hal_TimerNull_trigger__E

/* getExpiredCounts */
#undef ti_sysbios_hal_TimerNull_getExpiredCounts
#define ti_sysbios_hal_TimerNull_getExpiredCounts ti_sysbios_hal_TimerNull_getExpiredCounts__E

/* getExpiredTicks */
#undef ti_sysbios_hal_TimerNull_getExpiredTicks
#define ti_sysbios_hal_TimerNull_getExpiredTicks ti_sysbios_hal_TimerNull_getExpiredTicks__E

/* getCurrentTick */
#undef ti_sysbios_hal_TimerNull_getCurrentTick
#define ti_sysbios_hal_TimerNull_getCurrentTick ti_sysbios_hal_TimerNull_getCurrentTick__E

/* Module_startup */
#undef ti_sysbios_hal_TimerNull_Module_startup
#define ti_sysbios_hal_TimerNull_Module_startup ti_sysbios_hal_TimerNull_Module_startup__E

/* Instance_init */
#undef ti_sysbios_hal_TimerNull_Instance_init
#define ti_sysbios_hal_TimerNull_Instance_init ti_sysbios_hal_TimerNull_Instance_init__E

/* Instance_finalize */
#undef ti_sysbios_hal_TimerNull_Instance_finalize
#define ti_sysbios_hal_TimerNull_Instance_finalize ti_sysbios_hal_TimerNull_Instance_finalize__E

/* per-module runtime symbols */
#undef Module__MID
#ifdef ti_sysbios_hal_TimerNull_Module__id__CR
#define Module__MID (*((CT__ti_sysbios_hal_TimerNull_Module__id *)(xdcRomConstPtr + ti_sysbios_hal_TimerNull_Module__id__C_offset)))
#else
#define Module__MID ti_sysbios_hal_TimerNull_Module__id__C
#endif

#undef Module__DGSINCL
#ifdef ti_sysbios_hal_TimerNull_Module__diagsIncluded__CR
#define Module__DGSINCL (*((CT__ti_sysbios_hal_TimerNull_Module__diagsIncluded *)(xdcRomConstPtr + ti_sysbios_hal_TimerNull_Module__diagsIncluded__C_offset)))
#else
#define Module__DGSINCL ti_sysbios_hal_TimerNull_Module__diagsIncluded__C
#endif

#undef Module__DGSENAB
#ifdef ti_sysbios_hal_TimerNull_Module__diagsEnabled__CR
#define Module__DGSENAB (*((CT__ti_sysbios_hal_TimerNull_Module__diagsEnabled *)(xdcRomConstPtr + ti_sysbios_hal_TimerNull_Module__diagsEnabled__C_offset)))
#else
#define Module__DGSENAB ti_sysbios_hal_TimerNull_Module__diagsEnabled__C
#endif

#undef Module__DGSMASK
#ifdef ti_sysbios_hal_TimerNull_Module__diagsMask__CR
#define Module__DGSMASK (*((CT__ti_sysbios_hal_TimerNull_Module__diagsMask *)(xdcRomConstPtr + ti_sysbios_hal_TimerNull_Module__diagsMask__C_offset)))
#else
#define Module__DGSMASK ti_sysbios_hal_TimerNull_Module__diagsMask__C
#endif

#undef Module__LOGDEF
#ifdef ti_sysbios_hal_TimerNull_Module__loggerDefined__CR
#define Module__LOGDEF (*((CT__ti_sysbios_hal_TimerNull_Module__loggerDefined *)(xdcRomConstPtr + ti_sysbios_hal_TimerNull_Module__loggerDefined__C_offset)))
#else
#define Module__LOGDEF ti_sysbios_hal_TimerNull_Module__loggerDefined__C
#endif

#undef Module__LOGOBJ
#ifdef ti_sysbios_hal_TimerNull_Module__loggerObj__CR
#define Module__LOGOBJ ti_sysbios_hal_TimerNull_Module__loggerObj__R
#define Module__LOGOBJ (*((CT__ti_sysbios_hal_TimerNull_Module__loggerObj *)(xdcRomConstPtr + ti_sysbios_hal_TimerNull_Module__loggerObj__C_offset)))
#else
#define Module__LOGOBJ ti_sysbios_hal_TimerNull_Module__loggerObj__C
#endif

#undef Module__LOGFXN0
#ifdef ti_sysbios_hal_TimerNull_Module__loggerFxn0__CR
#define Module__LOGFXN0 (*((CT__ti_sysbios_hal_TimerNull_Module__loggerFxn0 *)(xdcRomConstPtr + ti_sysbios_hal_TimerNull_Module__loggerFxn0__C_offset)))
#else
#define Module__LOGFXN0 ti_sysbios_hal_TimerNull_Module__loggerFxn0__C
#endif

#undef Module__LOGFXN1
#ifdef ti_sysbios_hal_TimerNull_Module__loggerFxn1__CR
#define Module__LOGFXN1 (*((CT__ti_sysbios_hal_TimerNull_Module__loggerFxn1 *)(xdcRomConstPtr + ti_sysbios_hal_TimerNull_Module__loggerFxn1__C_offset)))
#else
#define Module__LOGFXN1 ti_sysbios_hal_TimerNull_Module__loggerFxn1__C
#endif

#undef Module__LOGFXN2
#ifdef ti_sysbios_hal_TimerNull_Module__loggerFxn2__CR
#define Module__LOGFXN2 (*((CT__ti_sysbios_hal_TimerNull_Module__loggerFxn2 *)(xdcRomConstPtr + ti_sysbios_hal_TimerNull_Module__loggerFxn2__C_offset)))
#else
#define Module__LOGFXN2 ti_sysbios_hal_TimerNull_Module__loggerFxn2__C
#endif

#undef Module__LOGFXN4
#ifdef ti_sysbios_hal_TimerNull_Module__loggerFxn4__CR
#define Module__LOGFXN4 (*((CT__ti_sysbios_hal_TimerNull_Module__loggerFxn4 *)(xdcRomConstPtr + ti_sysbios_hal_TimerNull_Module__loggerFxn4__C_offset)))
#else
#define Module__LOGFXN4 ti_sysbios_hal_TimerNull_Module__loggerFxn4__C
#endif

#undef Module__LOGFXN8
#ifdef ti_sysbios_hal_TimerNull_Module__loggerFxn8__CR
#define Module__LOGFXN8 (*((CT__ti_sysbios_hal_TimerNull_Module__loggerFxn8 *)(xdcRomConstPtr + ti_sysbios_hal_TimerNull_Module__loggerFxn8__C_offset)))
#else
#define Module__LOGFXN8 ti_sysbios_hal_TimerNull_Module__loggerFxn8__C
#endif

#undef Module__G_OBJ
#ifdef ti_sysbios_hal_TimerNull_Module__gateObj__CR
#define Module__G_OBJ (*((CT__ti_sysbios_hal_TimerNull_Module__gateObj *)(xdcRomConstPtr + ti_sysbios_hal_TimerNull_Module__gateObj__C_offset)))
#else
#define Module__G_OBJ ti_sysbios_hal_TimerNull_Module__gateObj__C
#endif

#undef Module__G_PRMS
#ifdef ti_sysbios_hal_TimerNull_Module__gatePrms__CR
#define Module__G_PRMS (*((CT__ti_sysbios_hal_TimerNull_Module__gatePrms *)(xdcRomConstPtr + ti_sysbios_hal_TimerNull_Module__gatePrms__C_offset)))
#else
#define Module__G_PRMS ti_sysbios_hal_TimerNull_Module__gatePrms__C
#endif

#undef Module__GP_create
#define Module__GP_create ti_sysbios_hal_TimerNull_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_sysbios_hal_TimerNull_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_sysbios_hal_TimerNull_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_sysbios_hal_TimerNull_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_sysbios_hal_TimerNull_Module_GateProxy_query

/* Object__sizingError */
#line 1 "Error_inconsistent_object_size_in_ti.sysbios.hal.TimerNull"
typedef char ti_sysbios_hal_TimerNull_Object__sizingError[(sizeof(ti_sysbios_hal_TimerNull_Object) > sizeof(ti_sysbios_hal_TimerNull_Struct)) ? -1 : 1];


#endif /* ti_sysbios_hal_TimerNull__INTERNAL____ */
