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
 *  Error codes returned by a licensing back office server.
 *
 *  These codes differ from status codes, which appear in response to a
 *  client request message that was processed with errors.
 */

/**
 * @addtogroup clientCommon
 * @{
 */

/**
 * @defgroup backOfficeErrorCodes Server Error Codes
 * @{
 */

#ifndef _INCLUDE_FLC_BACKOFFICE_ERRORCODES_H_
#define _INCLUDE_FLC_BACKOFFICE_ERRORCODES_H_

typedef enum FlcBackOfficeErrorCode
{
  FLC_BOS_ERR_MESSAGE_CORRUPTED                     = 0,          /**< Corrupted binary message */
  FLC_BOS_ERR_MISSING_MESSAGE_TYPE                  = 1,          /**< Message type missing from binary message */
  FLC_BOS_ERR_BAD_MESSAGE_TYPE                      = 2,          /**< Unexpected message type received */
  FLC_BOS_ERR_MISSING_TRANSACTION_ID                = 3,          /**< Message is missing the transaction ID (host ID) */
  FLC_BOS_ERR_MISSING_VENDOR                        = 4,          /**< Message is missing vendor name property */
  FLC_BOS_ERR_CONVERTING_LICENSE                    = 5,          /**< No longer used */
  FLC_BOS_ERR_SIGNING_FAILED                        = 6,          /**< No longer used */
  FLC_BOS_ERR_SIGNATURE_INVALID                     = 7,          /**< Signature didn't pass validation. */
  FLC_BOS_ERR_INVALID_TRANSACTION_ID                = 8,          /**< Invalid host ID for sync message */
  FLC_BOS_ERR_PROCESSING_FULFILLMENT                = 9,          /**< No longer used */
  FLC_BOS_ERR_GENERATING_RESPONSE                   = 10,         /**< Unexpected exception while generating response */
  FLC_BOS_ERR_UNKNOWN_IDENTITY                      = 11,         /**< Unknown publisher identity name */
  FLC_BOS_ERR_UNKNOWN_VENDOR                        = 12,         /**< The publisher name in the message is unknown */
  FLC_BOS_ERR_BACKOFFICE_LICENSE_UNAVAILABLE        = 13,         /**< Back-office not licensed for use with FNE client */
  FLC_BOS_ERR_UNKNOWN_HOST_TYPE                     = 14,         /**< Host type is not known to back-office */
  FLC_BOS_ERR_MISMATCHED_HOST_TYPE                  = 15,         /**< Host type in request does not match back-office records for host */
  FLC_BOS_ERR_MISMATCHED_ID_TYPE                    = 16,         /**< Host ID type in request is not allowed for host type */
  FLC_BOS_ERR_MISMATCHED_VENDOR                     = 17,         /**< No longer used */
  FLC_BOS_ERR_DEVICE_ON_HOLD                        = 18,         /**< Host is on hold, unable to process requests */
  FLC_BOS_ERR_MISMATCHED_DEVICE_TYPE                = 19,         /**< Request mismatched client vs. server */
  FLC_BOS_ERR_MISSING_UUID                          = 20,         /**< Request missing expected UUID */
  FLC_BOS_ERR_MISMATCHED_UUID                       = 21,         /**< Request has mismatched UUID */
  FLC_BOS_ERR_MISMATCHED_HOSTID_FOR_UUID            = 22,         /**< Host's UUID does not match expected UUID for that host ID */
  FLC_BOS_ERR_BAD_REQUEST                           = 23,         /**< Request has invalid content */
  FLC_BOS_ERR_MISMATCHED_ENTERPRISE_ID              = 24,         /**< Enterprise ID in request does not match back-office records for host */
  FLC_BOS_ERR_UNSUPPORTED_ID_TYPE                   = 25,         /**< Host ID type in request is not supported */
  FLC_BOS_ERR_MISMATCHED_RESPONSE_TYPE              = 26,         /**< Host cannot switch between buffered and normal capability response */
  FLC_BOS_ERR_NO_VIRTUAL_HOSTS                      = 27,         /**< Back-office not configured to allow virtual hosts */
  FLC_BOS_ERR_NOT_LICENSED                          = 28,         /**< Back-office not licensed for FlexNet Embedded support */
  FLC_BOS_ERR_NOT_FUTURE_DATE                       = 29,         /**< SOFT breaking type should have a future date for bindingGracePeriodEnd */
  FLC_BOS_ERR_MISMATCHED_BACKUP_HOST_ID             = 30,         /**< Existing server has mismatched backup host ID with incoming request */
  FLC_BOS_ERR_BACKUP_HOST_ID_SAME_AS_PRIMARY        = 31,         /**< Backup host ID same as primary */
  FLC_BOS_ERR_BACKUP_HOST_ID_NOT_SAME_TYPE_AS_PRIMARY= 32,         /**< Backup host ID must have same ID type as primary */
  FLC_BOS_ERR_BACKUP_HOST_ID_ALREADY_IN_USE         = 33,         /**< Host entity already exists with same host ID as backup host */
  FLC_BOS_ERR_UNSUPPORTED_HOST_TYPE                 = 34,         /**< Host type in request does not support the request host class. */
  FLC_BOS_ERR_HOST_CREATE_DENIED                    = 35,         /**< Creation of this unknown host is not allowed. */
  FLC_BOS_ERR_HOST_ACCESS_DENIED                    = 36,         /**< Access to this host is not allowed. */
  FLC_BOS_ERR_LOCK_ACQUISITION_FAILED               = 37,         /**< Unable to aquire lock for request processing */
  FLC_BOS_ERR_LOCK_TIMED_OUT                        = 38,         /**< Lock timed out during request processing */
  FLC_BOS_ERR_TRANSACTION_FAILED                    = 39,         /**< Transaction failed during request processing */
  FLC_BOS_ERR_NEXT,

  /* Updates and Insights Specific */
}FlcBackOfficeErrorCode;

/** @} */ /* end of backOfficeErrorCodes group */
/** @} */ /* end of addtogroup clientCommon */

#endif /*_INCLUDE_FLC_BACKOFFICE_ERRORCODES_H_*/
