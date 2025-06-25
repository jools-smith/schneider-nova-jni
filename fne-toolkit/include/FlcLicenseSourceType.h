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

#ifndef _INCLUDE_FLC_LICENSESOURCETYPE_H_
#define _INCLUDE_FLC_LICENSESOURCETYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file
 *  Defines license source types
 */

typedef enum FlcLicenseSourceType
{
  FLC_LICENSE_SOURCE_UNKNOWN         = 0,   /**< Unknown */
  FLC_LICENSE_SOURCE_BUFFER          = 1,   /**< Buffer license source */
  FLC_LICENSE_SOURCE_TRUSTED_STORAGE = 2,   /**< Trusted Storage license source */
  FLC_LICENSE_SOURCE_TRIAL           = 3,   /**< Trial license source */
  FLC_LICENSE_SOURCE_SHORT_CODE      = 4,   /**< Short Code license source */
  FLC_LICENSE_SOURCE_CERTIFICATE     = 5,   /**< Certificate license source */
  FLC_LICENSE_SOURCE_SERVED_BUFFER   = 6    /**< Served buffer license source */
} FlcLicenseSourceType;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _INCLUDE_FLC_LICENSESOURCETYPE_H_ */
