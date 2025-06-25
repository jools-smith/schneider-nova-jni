/****************************************************************************
  Copyright (c) 2012-2020 Flexera Software LLC.
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
 *  Provides licensing client specific returned error code values.
 */

/**
 * @addtogroup licensingClient
 * @{
 */

/**
 * @defgroup licensingErrorCodes Licensing Error Codes
 * @{
 */

#ifndef _INCLUDE_FLC_LICENSING_ERRORCODES_H_
#define _INCLUDE_FLC_LICENSING_ERRORCODES_H_

#include "FlcErrorCodes.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum FlcLicensingErrorCode
{
  FLCSWC_CORE_MISSING                             = 0x74200001, /**< Unable to locate FlxCore/FlxCore64 library. */
  FLCSWC_CAPABILITY_RESPONSE_ALREADY_PROCESSED    = 0x74200002, /**< Capability response already processed. */ 
  FLCSWC_TS_LICENSE_SOURCE_NOT_FOUND              = 0x74200003, /**< Unable to locate a trusted storage license source. */
  FLCSWC_BUFFER_LICENSE_SOURCE_NOT_FOUND          = 0x74200004, /**< Unable to locate an appropriate buffer license source. */ 
  FLCSWC_CERTIFICATE_LICENSE_SOURCE_NOT_FOUND     = 0x74200005, /**< Unable to locate an appropriate certificate license source. */ 
  FLCSWC_SHORTCODE_RESPONSE_INCOMPLETE            = 0x74200006, /**< Segmented short code response is incomplete. */ 
  FLCSWC_SHORTCODE_RESPONSE_NOT_FOUND             = 0x74200007, /**< Unable to find short code response object. */
  FLCSWC_SHORTCODE_RESPONSE_DECODING_NOT_SET      = 0x74200008, /**< Short code response decoding scheme not set. */
  FLCSWC_SHORTCODE_INVALID_SEGMENT_SIZE_FOR_API   = 0x74200009, /**< Short code API does not support the current segment size specification. */
  FLCSWC_SERVED_BUFFER_LICENSE_SOURCE_NOT_FOUND   = 0x7420000a, /**< Unable to locate an appropriate served buffer license source. */ 
  FLCSWC_NO_CAPABILITY_RESPONSE_RECEIVED          = 0x7420000b, /**< Required capability response not received. */ 
} FlcLicensingErrorCode;

/** @} */ /* end of licensingErrorCodes group */
/** @} */ /* end of addtogroup licensingClient */

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /*_INCLUDE_FLC_LICENSING_ERRORCODES_H_*/
