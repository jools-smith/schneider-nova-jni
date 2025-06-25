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
 *  Allowed license server instance values.
 */

#ifndef _INCLUDE_FLC_LICENSE_SERVER_INSTANCE_H_
#define _INCLUDE_FLC_LICENSE_SERVER_INSTANCE_H_

typedef enum FlcLicenseServerInstance
{
  FLC_LICENSE_SERVER_UNKNOWN                        = 0,          /**< Unknown */
  FLC_LICENSE_SERVER_DEFAULT                        = 1,          /**< Default license server */
  FLC_LICENSE_SERVER_2                              = 2,          /**< Second license server */
  FLC_LICENSE_SERVER_3                              = 3,          /**< Third license server */
  FLC_LICENSE_SERVER_4                              = 4,          /**< Fourth license server */
  FLC_LICENSE_SERVER_5                              = 5,          /**< Fifth license server */
  FLC_LICENSE_SERVER_6                              = 6,          /**< Sixth license server */
  FLC_LICENSE_SERVER_7                              = 7,          /**< Seventh license server */
  FLC_LICENSE_SERVER_8                              = 8,          /**< Eighth license server */
  FLC_LICENSE_SERVER_9                              = 9,          /**< Ninth license server */
  FLC_LICENSE_SERVER_10                             = 10,         /**< Tenth license server */
  FLC_LICENSE_SERVER_MAX                            = FLC_LICENSE_SERVER_10  /**< Maximum license server value */
}FlcLicenseServerInstance;


#endif /*_INCLUDE_FLC_LICENSE_SERVER_INSTANCE_H_*/
