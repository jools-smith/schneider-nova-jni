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
 *  Enumeration describing the capability request operation types.
 */

/**
 * @addtogroup CapabilityRequest
 * @{
 */

#ifndef _INCLUDE_FLC_CAPABILITYREQUEST_OPERATION_H_
#define _INCLUDE_FLC_CAPABILITYREQUEST_OPERATION_H_

/**
 *  Enumeration describing the capability request operation types.
 */
typedef enum FlcCapabilityRequestOperation
{
  FLC_CAPABILITY_REQUEST_OPERATION_UNKNOWN          = 0,          /**< Unknown */
  FLC_CAPABILITY_REQUEST_OPERATION_REQUEST          = 1,          /**< Request */
  FLC_CAPABILITY_REQUEST_OPERATION_REPORT           = 2,          /**< Report */
  FLC_CAPABILITY_REQUEST_OPERATION_UNDO             = 3,          /**< Undo */
  FLC_CAPABILITY_REQUEST_OPERATION_PREVIEW          = 4,          /**< Preview */
  FLC_CAPABILITY_REQUEST_OPERATION_NEXT
}FlcCapabilityRequestOperation;

/** @} */ /* end of addtogroup CapabilityRequest */

#endif /*_INCLUDE_FLC_CAPABILITYREQUEST_OPERATION_H_*/
