/*
 *  Copyright 2019 by Texas Instruments Incorporated.
 *
 */

/*
 *  ======== select.h ========
 *  This header selects an appropriate target name based on TI compiler
 *  predefined macros and includes the appropriate target-specific std.h.
 *
 *  This header is entirely optional: the user can directly select any
 *  existing target by adding definition of the following two symbols to
 *  the compile line:
 *    xdc_target_name__     the short name of the target; e.g., C64P
 *    xdc_target_types__    a package path relative path to the types header
 *                          for the target; e.g., ti/targets/std.h
 *
 *  For more information about these symbols see:
 *  http://rtsc.eclipse.org/docs-tip/Integrating_RTSC_Modules
 */

#if defined (__arm__) && defined (__ARM_EABI__)
#  define xdc_target_types__ gnu/targets/arm/std.h

#  if defined (__ARM_ARCH_7A__) && defined(__VFP_FP__) && !defined(__SOFTFP__)
#      if defined (__ARM_ARCH_EXT_IDIV__)
#          define xdc_target_name__ A15F
#      else
#          define xdc_target_name__ A8F
#      endif
#  elif defined (__ARM_ARCH_7M__)
#      define xdc_target_name__ M3
#  elif defined (__ARM_ARCH_7EM__)
#      if (defined(__VFP_FP__) && !defined(__SOFTFP__))
#          define xdc_target_name__ M4F
#      else
#          define xdc_target_name__ M4
#      endif
#  endif

#endif

#if defined(xdc_target_name__) && defined(xdc_target_types__)
/*
 *  ======== include the selected type header ========
 */
#define xdc_target__ <xdc_target_types__>
#include xdc_target__

#else
  /* if we get here, this header was unable to select an appropriate set of
   * types.  If the target exists, you can avoid the warnings below by
   * explicitly defining the symbols xdc_target_name__ and
   * xdc_target_types__ on the compile line.
   */
#  ifndef xdc_target_name__
#    warning "can't determine an appropriate setting for xdc_target_name__"
#  endif
#  ifndef xdc_target_types__
#    warning "can't determine an appropriate setting for xdc_target_types__"
#  endif
#endif
/*
 *  @(#) gnu.targets; 1, 0, 1,0; 8-9-2019 17:14:49; /db/ztree/library/trees/xdctargets/xdctargets-v00/src/ xlibrary

 */

