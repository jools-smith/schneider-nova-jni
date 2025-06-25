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
 *  Defines message-status types and message-status codes.
 */

#ifndef _INCLUDE_FLC_MESSAGE_STATUS_CODES_H_
#define _INCLUDE_FLC_MESSAGE_STATUS_CODES_H_

typedef enum FlcMessageStatusType
{
  FLC_MS_TYPE_UNKNOWN                               = 0,
  FLC_MS_TYPE_PUBLIC                                = 1,
  FLC_MS_TYPE_EXTERNAL                              = 2,
  FLC_MS_TYPE_NEXT
}FlcMessageStatusType;

typedef enum FlcMessageStatusCode
{
  FLC_MS_CODE_UNKNOWN                               = 0,          /**< Unknown message status */
  FLC_MS_CODE_ACTIVATION_ID_INVALID                 = 1,          /**< Specified activation ID is invalid */
  FLC_MS_CODE_RIGHTS_ID_INVALID                     = FLC_MS_CODE_ACTIVATION_ID_INVALID, /**< Specified activation ID is invalid */
  FLC_MS_CODE_ACTIVATION_ID_UNAVAILABLE             = 2,          /**< Specified activation ID has no available copies */
  FLC_MS_CODE_RIGHTS_ID_UNAVAILABLE                 = FLC_MS_CODE_ACTIVATION_ID_UNAVAILABLE, /**< Specified activation ID has no available copies */
  FLC_MS_CODE_HOST_ID_INVALID                       = 3,          /**< The host ID is unknown */
  FLC_MS_CODE_DEVICE_ID_INVALID                     = FLC_MS_CODE_HOST_ID_INVALID, /**< The host ID is invalid */
  FLC_MS_CODE_HOST_OBSOLETE                         = 4,          /**< The host ID is marked as obsolete */
  FLC_MS_CODE_DEVICE_OBSOLETE                       = FLC_MS_CODE_HOST_OBSOLETE, /**< The host ID is marked as obsolete */
  FLC_MS_CODE_ACTIVATION_ID_DENIED                  = 5,          /**< The activation ID was denied */
  FLC_MS_CODE_RIGHTS_ID_DENIED                      = FLC_MS_CODE_ACTIVATION_ID_DENIED, /**< The activation ID was denied */
  FLC_MS_CODE_ACTIVATION_ID_EXPIRED                 = 6,          /**< The activation ID has expired */
  FLC_MS_CODE_RIGHTS_ID_EXPIRED                     = FLC_MS_CODE_ACTIVATION_ID_EXPIRED, /**< The activation ID has expired */
  FLC_MS_CODE_FEATURE_COUNT_INSUFFICIENT            = 7,          /**< Insufficient feature count available */
  FLC_MS_CODE_FEATURE_WINDBACK_DETECTED             = 8,          /**< Clock windback detected */
  FLC_MS_CODE_FEATURE_SERVER_HOST_ID_MISMATCH       = 9,          /**< Server host ID does not match */
  FLC_MS_CODE_FEATURE_NOT_STARTED                   = 10,         /**< Feature start date is in the future */
  FLC_MS_CODE_FEATURE_EXPIRED                       = 11,         /**< Feature has expired */
  FLC_MS_CODE_FEATURE_VERSION_NOT_FOUND             = 12,         /**< Matching feature version was not found */
  FLC_MS_CODE_ACTIVATION_ID_COUNT_INVALID           = 13,         /**< Invalid activation ID copies */
  FLC_MS_CODE_RIGHTS_ID_COUNT_INVALID               = FLC_MS_CODE_ACTIVATION_ID_COUNT_INVALID, /**< Invalid activation ID copies */
  FLC_MS_CODE_ACTIVATION_ID_COUNT_EXCEEDED          = 14,         /**< Activation ID copies exceeds available copies */
  FLC_MS_CODE_RIGHTS_ID_COUNT_EXCEEDED              = FLC_MS_CODE_ACTIVATION_ID_COUNT_EXCEEDED, /**< Activation ID copies exceeds available copies */
  FLC_MS_CODE_FEATURE_NOT_AVAILABLE                 = 15,         /**< Feature is not available */
  FLC_MS_CODE_SERVED_FEATURE_NODE_LOCKED            = 16,         /**< Node-locked features cannot be served */
  FLC_MS_CODE_SERVED_FEATURE_DUPLICATE              = 17,         /**< Duplicate feature */
  FLC_MS_CODE_SERVER_MAINTENANCE_INTERVAL_PASSED    = 18,         /**< Backup server's maintenance interval has expired */
  FLC_MS_CODE_ACTIVATION_ID_MISMATCH                = 19,         /**< Activation ID/Host owner mismatch */
  FLC_MS_CODE_RIGHTS_ID_MISMATCH                    = FLC_MS_CODE_ACTIVATION_ID_MISMATCH, /**< Activation ID/Host owner mismatch */
  FLC_MS_CODE_HOST_CREATE_FAILED                    = 20,         /**< Host create failed */
  FLC_MS_CODE_DEVICE_CREATE_FAILED                  = FLC_MS_CODE_HOST_CREATE_FAILED, /**< Host create failed */
  FLC_MS_CODE_CUSTOM_ATTRIBUTE_INVALID              = 21,         /**< Invalid custom attribute */
  FLC_MS_CODE_CUSTOM_ATTRIBUTE_REQUIRED             = 22,         /**< Custom attribute required */
  FLC_MS_CODE_ACTIVATION_ID_NOT_PERMITTED           = 23,         /**< Activation ID not permitted */
  FLC_MS_CODE_RIGHTS_ID_NOT_PERMITTED               = FLC_MS_CODE_ACTIVATION_ID_NOT_PERMITTED, /**< Activation ID not permitted */
  FLC_MS_CODE_SYNC_CLIENT_IS_SERVER                 = 24,         /**< No longer used */
  FLC_MS_CODE_SYNC_HOST_TYPE_MISMATCH               = 25,         /**< No longer used */
  FLC_MS_CODE_SYNC_DEVICE_TYPE_MISMATCH             = FLC_MS_CODE_SYNC_HOST_TYPE_MISMATCH, /**< No longer used */
  FLC_MS_CODE_SYNC_HOST_ID_TYPE_MISMATCH            = 26,         /**< No longer used */
  FLC_MS_CODE_SYNC_DEVICE_ID_TYPE_MISMATCH          = FLC_MS_CODE_SYNC_HOST_ID_TYPE_MISMATCH, /**< No longer used */
  FLC_MS_CODE_SYNC_TIME_MISMATCH                    = 27,         /**< Mismatched or out of order time stamp in sync message */
  FLC_MS_CODE_SYNC_FEATURE_ID_INVALID               = 28,         /**< No longer used */
  FLC_MS_CODE_SYNC_HOST_TYPE_INVALID                = 29,         /**< No longer used */
  FLC_MS_CODE_HOST_UPDATE_FAILED                    = 30,         /**< No longer used */
  FLC_MS_CODE_DEVICE_UPDATE_FAILED                  = FLC_MS_CODE_HOST_UPDATE_FAILED, /**< No longer used */
  FLC_MS_CODE_SYNC_NOT_SERVER                       = 31,         /**< Server sync message from non-server host */
  FLC_MS_CODE_SYNC_FEATURE_MISMATCH                 = 32,         /**< No longer used */
  FLC_MS_CODE_SYNC_BACKUP_SERVER                    = 33,         /**< Server sync message from backup server host */
  FLC_MS_CODE_SYNC_TIME_UNITS_MISMATCH              = 34,         /**< Unexpected time units in sync message */
  FLC_MS_CODE_VIRTUAL_CLIENTS_NOT_SUPPORTED         = 35,         /**< Virtual clients are not supported */
  FLC_MS_CODE_ACTIVATION_ID_NO_DECREASE             = 36,         /**< The activation ID specified cannot have its copies reduced */
  FLC_MS_CODE_RIGHTS_ID_NO_DECREASE                 = FLC_MS_CODE_ACTIVATION_ID_NO_DECREASE, /**< The activation ID specified cannot have its copies reduced */
  FLC_MS_CODE_RIGHTS_ID_DECREASE_METERED            = FLC_MS_CODE_ACTIVATION_ID_NO_DECREASE, /**< The activation ID specified cannot have its copies reduced */
  FLC_MS_CODE_MESSAGE_TYPE_NOT_SUPPORTED            = 37,         /**< Server cannot process this type of message */
  FLC_MS_CODE_REQUEST_DATA_MISSING                  = 38,         /**< Required fields are missing from the capability request */
  FLC_MS_CODE_PUBLISHER_IDENTITY_MISMATCH           = 39,         /**< Message cannot be processed due to mismatched identity */
  FLC_MS_CODE_CLIENT_APPLICATION_TYPE_NOT_SUPPORTED = 40,         /**< Server cannot process capability request from this type of client */
  FLC_MS_CODE_LICENSE_RESPONSE_NOT_SUPPORTED        = 41,         /**< Server does not support clients that require capability responses */
  FLC_MS_CODE_CORRELATION_ID_MISSING                = 42,         /**< The required correlation ID for this application type is missing from the capability request */
  FLC_MS_CODE_FEATURE_DATA_INVALID                  = 43,         /**< Desired features in capability request contain both positive and negative counts */
  FLC_MS_CODE_RETURN_DATA_INVALID                   = 44,         /**< Server records do not match return data in the capability request */
  FLC_MS_CODE_RETURN_FEATURE_NOT_AVAILABLE          = 45,         /**< Feature is not available for return */
  FLC_MS_CODE_RETURN_FEATURE_VERSION_NOT_FOUND      = 46,         /**< The requested version of the feature is not available for return */
  FLC_MS_CODE_RETURN_FEATURE_EXPIRED                = 47,         /**< Feature cannot be returned because it has expired */
  FLC_MS_CODE_RETURN_FEATURE_NOT_STARTED            = 48,         /**< Feature cannot be returned because its start date is in the future */
  FLC_MS_CODE_UNDO_INTERVAL_EXPIRED                 = 49,         /**< The allowed undo interval has passed or the correlation ID is incorrect */
  FLC_MS_CODE_RETURN_FEATURE_COUNT_INSUFFICIENT     = 50,         /**< Server cannot grant the return of the feature due to an insufficient count */
  FLC_MS_CODE_BAD_REQUEST                           = 51,         /**< Unable to process request */
  FLC_MS_CODE_BAD_REQUEST_PARAMETER                 = FLC_MS_CODE_BAD_REQUEST, /**< Unable to process request */
  FLC_MS_CODE_RETURN_FEATURE_NOT_REUSABLE           = 52,         /**< Feature cannot be returned because it is not marked as reusable */
  FLC_MS_CODE_FEATURE_OVERDRAFT_NOT_SUPPORTED       = 53,         /**< Server does not support features with an overdraft count */
  FLC_MS_CODE_FEATURE_METERED_NOT_SUPPORTED         = 54,         /**< Server does not support features with a metered license model */
  FLC_MS_CODE_NOT_REGISTERED_ON_SERVER              = 55,         /**< Requesting host is not registered on the license server */
  FLC_MS_CODE_EARLY_RETURN_NOT_ALLOWED              = 56,         /**< Served buffer features cannot be returned early. */
  FLC_MS_CODE_MODE_SWITCHING_NOT_ALLOWED            = 57,         /**< Client cannot switch from use of served buffer to trusted storage and vice versa. */
  FLC_MS_CODE_CAP_RESPONSE_REJECTED_FOR_OVERAGE     = 58,         /**< Capability response rejected as it would cause overage and overage control is enabled. */
  FLC_MS_CODE_HOST_RETURNED                         = 59,         /**< The host ID is marked as returned */
  FLC_MS_CODE_ACTIVATION_ID_OVER_COMMITTED          = 60,         /**< Total activation ID copies requested exceeds entitled copies */
  FLC_MS_CODE_RIGHTS_ID_OVER_COMMITTED              = FLC_MS_CODE_ACTIVATION_ID_OVER_COMMITTED, /**< Total activation ID copies requested exceeds entitled copies */
  FLC_MS_CODE_ACTIVATION_ID_NO_INCREASE             = 61,         /**< The activation ID specified cannot have its copies increased */
  FLC_MS_CODE_RIGHTS_ID_NO_INCREASE                 = FLC_MS_CODE_ACTIVATION_ID_NO_INCREASE, /**< The activation ID specified cannot have its copies increased */
  FLC_MS_CODE_MISSING_FEATURE_EXPIRATION            = 62,         /**< License Expiration Date not found for the specified activation ID */
  FLC_MS_CODE_SERVER_ENVIRONMENT_TOLERANCE_INTERVAL = 63,         /**< Server is currently running in environment tolerance interval */
  FLC_MS_CODE_SERVER_ENVIRONMENT_TOLERANCE_INTERVAL_EXPIRED= 64,         /**< Server's environment tolerance interval has expired */
  FLC_MS_CODE_FEATURE_METERED_TS_ONLY               = 65,         /**< Features with a metered license model are not supported in buffer licenses */
  FLC_MS_CODE_SERVER_BUSY_PROCESSING_RESERVATIONS   = 66,         /**< Server is currently busy processing reservations (currently unused) */
  FLC_MS_CODE_FEATURE_REJECTED_BY_CHECKOUT_FILTER   = 67,         /**< Feature is not available due to a server checkout filter rejection */
  FLC_MS_CODE_REQUEST_REJECTED_TRUST_BREAK          = 68,         /**< Capability request rejected due to trust break */
  FLC_MS_CODE_INVALID_FEATURE_ID                    = 69,         /**< The back-office rejected the sync message because it contains invalid feature ids */
  FLC_MS_CODE_SERVER_BINDING_BREAK_DETECTED         = 70,         /**< Binding break detected. */
  FLC_MS_CODE_SERVER_BINDING_GRACE_PERIOD_EXPIRED   = 71,         /**< Server's grace period has expired. */
  FLC_MS_CODE_CLONE_SUSPECT                         = 72,         /**< No longer used */
  FLC_MS_CODE_NOT_LICENSED                          = 73,         /**< Not licensed. */
  FLC_MS_CODE_ADDON_EXPIRATION_IGNORED              = 74,         /**< The expiration date override on add-on was ignored */
  FLC_MS_CODE_REQUESTOR_ID_INVALID                  = 75,         /**< The Requestor ID is invalid */
  FLC_MS_CODE_ACTIVATION_ID_PARTIAL_FULFILL         = 76,         /**< The activation ID copies requested were only partially fulfilled */
  FLC_MS_CODE_DECREMENTED_COUNT_GREATER_THAN_CURRENT= 77,         /**< The decremented feature count is greater than the client's current count */
  FLC_MS_CODE_FEATURE_TO_DECREMENT_NOT_FOUND        = 78,         /**< The client has no available count for the feature */
  FLC_MS_CODE_INCREASE_ONLY_BUFFER_LICENSE          = 79,         /**< The number of buffer license copies can increase, not decrease */
  FLC_MS_CODE_REQUEST_REJECTED_BY_RULE              = 80,         /**< Features are not available due to a rule rejection */
  FLC_MS_CODE_REQUEST_REJECTED_BY_RULE_MAX          = 81,         /**< Features are not available due to rule rejection MAX */
  FLC_MS_CODE_RESERVATIONS_UNSUPPORTED              = 82,         /**< Capability request rejected as reservations are not supported when allowDuplicateClients is enabled */
  FLC_MS_CODE_NEXT
}FlcMessageStatusCode;


#endif /*_INCLUDE_FLC_MESSAGE_STATUS_CODES_H_*/
