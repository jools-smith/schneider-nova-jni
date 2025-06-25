/****************************************************************************
  Copyright (c) 2008-2020 Flexera Software LLC.
  All Rights Reserved.
  This software has been provided pursuant to a License Agreement
  containing restrictions on its use.  This software contains
  valuable trade secrets and proprietary information of
  Flexera Software LLC and is protected by law.
  It may not be copied or distributed in any form or medium, disclosed
  to third parties, reverse engineered or used in any manner not
  provided for in said License Agreement except with the prior
  written authorization from Flexera Software LLC.
*****************************************************************************/

/** @file
 *  Defines client data types.
 *
 *
 */

#ifndef _INCLUDE_FLC_TYPES_H_
#define _INCLUDE_FLC_TYPES_H_

#ifdef _MSC_VER
#pragma warning( push, 3 )
#endif /* _MSC_VER */

/* For size_t and NULL */
#include <stddef.h>

#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */

#ifndef FLC_USE_CUSTOM_TYPES_HEADER

    #if !defined(_MSC_VER)
        /* For int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t */
        #if defined(__APPLE__) || defined(__hpux)
            #include <inttypes.h>
        #else
            #if defined(__vxworks)
                #include <vxWorks.h>
            #else
                /* For all Posix compliant variants */
                #include <stdint.h>
            #endif
        #endif
    #endif

#else /* FLC_USE_CUSTOM_TYPES_HEADER */
    /** If auto-detection of system header files fails when building from source,
        define FLC_USE_CUSTOM_TYPES_HEADER to provide definitions from your own
        FlcCustomTypes.h file.
        Definitions needed are:
          int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t
    */
    #include "FlcCustomTypes.h"

#endif /* FLC_USE_CUSTOM_TYPES_HEADER */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup clientCommon
 * @{
 */

/**
 * @defgroup types Types
 * @{
 */


/*
 *  Data types
 */
#if defined(_MSC_VER)
    typedef __int8              FlcInt8;        /**< 8-bit, signed integer */
    typedef unsigned __int8     FlcUInt8;       /**< 8-bit, unsigned integer */
    typedef __int16             FlcInt16;       /**< 16-bit, signed integer */
    typedef unsigned __int16    FlcUInt16;      /**< 16-bit, unsigned integer */
    typedef __int32             FlcInt32;       /**< 32-bit, signed integer */
    typedef unsigned __int32    FlcUInt32;      /**< 32-bit, unsigned integer */

#elif defined(__APPLE__)
    typedef int8_t              FlcInt8;        /**< 8-bit, signed integer */
    typedef uint8_t             FlcUInt8;       /**< 8-bit, unsigned integer */
    typedef int16_t             FlcInt16;       /**< 16-bit, signed integer */
    typedef uint16_t            FlcUInt16;      /**< 16-bit, unsigned integer */
    typedef int32_t             FlcInt32;       /**< 32-bit, signed integer */
    typedef uint32_t            FlcUInt32;      /**< 32-bit, unsigned integer */

#else /* For all Posix compliant variants */
    typedef int8_t              FlcInt8;        /**< 8-bit, signed integer */
    typedef uint8_t             FlcUInt8;       /**< 8-bit, unsigned integer */
    typedef int16_t             FlcInt16;       /**< 16-bit, signed integer */
    typedef uint16_t            FlcUInt16;      /**< 16-bit, unsigned integer */
    typedef int32_t             FlcInt32;       /**< 32-bit, signed integer */
    typedef uint32_t            FlcUInt32;      /**< 32-bit, unsigned integer */

#endif

typedef enum FlcBoolValue
{
    FLC_FALSE = 0,
    FLC_TRUE = 1
} FlcBoolValue;

typedef FlcInt32    FlcBool;

typedef char        FlcChar;

typedef FlcUInt32   FlcSize;

/** @} */ /* end of types group. */
/** @} */ /* end of addtogroup clientCommon */

#ifdef __cplusplus
}
#endif

#endif /*_INCLUDE_FLC_TYPES_H */
