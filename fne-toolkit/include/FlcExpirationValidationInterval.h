/****************************************************************************
  Copyright (c) 2012-2024 Flexera Software LLC.
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
 *  Enumeration describing the borrow granularity interval.
 */

/**
 * @addtogroup CapabilityRequest
 * @{
 */

#ifndef _INCLUDE_FLC_EXPIRATION_VALIDATION_INTERVAL_H_
#define _INCLUDE_FLC_EXPIRATION_VALIDATION_INTERVAL_H_

/**
 *  Enumeration describing the borrow granularity interval.
 */
typedef enum FlcExpirationValidationInterval
{
  FLC_EVI_UNKNOWN                                   = 0,
  FLC_EVI_DAY                                       = 1,          /**< default */
  FLC_EVI_HOUR                                      = 2,
  FLC_EVI_MINUTE                                    = 3,
  FLC_EVI_SECOND                                    = 4,
  FLC_EVI_NEXT
}FlcExpirationValidationInterval;

/** @} */ /* end of addtogroup CapabilityRequest */

#endif /*_INCLUDE_FLC_EXPIRATION_VALIDATION_INTERVAL_H_*/
