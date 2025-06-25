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
 *  Defines licensing binary message types.
 */

#ifndef _INCLUDE_FLC_MESSAGE_TYPES_H_
#define _INCLUDE_FLC_MESSAGE_TYPES_H_

typedef enum FlcMessageType
{
  FLC_MESSAGE_TYPE_UNKNOWN                          = -1,         /**< Unknown message type */
  FLC_MESSAGE_TYPE_ERROR                            = 0,          /**< Error response */
  FLC_MESSAGE_TYPE_BUFFER_LICENSE                   = 1,          /**< Buffer license */
  FLC_MESSAGE_TYPE_CAPABILITY_REQUEST               = 2,          /**< Capability request */
  FLC_MESSAGE_TYPE_CAPABILITY_RESPONSE              = 3,          /**< Capability response from the back office */
  FLC_MESSAGE_TYPE_TRIAL                            = 14,         /**< Trial */
  FLC_MESSAGE_TYPE_SERVED_CAPABILITY_RESPONSE       = 15,         /**< Capability response from a local server */
  FLC_MESSAGE_TYPE_INFORMATION                      = 16,         /**< Information message */
  FLC_MESSAGE_TYPE_SYNC_REQUEST                     = 17,         /**< Sync request */
  FLC_MESSAGE_TYPE_SYNC_MESSAGE                     = 18,         /**< Sync response */
  FLC_MESSAGE_TYPE_SYNC_ACKNOWLEDGMENT              = 19,         /**< Sync acknowledgement */
  FLC_MESSAGE_TYPE_BUFFER_CAPABILITY_RESPONSE       = 22,         /**< Buffer capability response from the back office or a local server */
  FLC_MESSAGE_TYPE_NEXT
}FlcMessageType;


#endif /*_INCLUDE_FLC_MESSAGE_TYPES_H_*/
