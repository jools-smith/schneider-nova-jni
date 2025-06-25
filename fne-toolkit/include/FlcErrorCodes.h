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
 *  Provides common returned error code values.
 */

/**
 * @addtogroup clientCommon
 * @{
 */

/**
 * @defgroup errorCodes Common Error Codes
 * @{
 */

#ifndef _INCLUDE_FLC_ERRORCODES_H_
#define _INCLUDE_FLC_ERRORCODES_H_

#include "FlcSwcErrorCodes.h"

typedef enum FlcErrorCode
{
  FLCERR_SUCCESS                                    = 0x00000000, /**< Success. */
  FLCERR_BAD_PARAM                                  = 0x70000001, /**< One of the input parameters is bad. */
  FLCERR_BUFFER_SIZE_INSUFFICIENT                   = 0x70000002, /**< Provided buffer is not big enough to hold the data. */
  FLCERR_CALLOUT_ERROR_DATA_MISSING                 = 0x70000003, /**< Callout failed, but didn't set error information. */
  FLCERR_DATA_CORRUPTED                             = 0x70000004, /**< Data corruption found. */
  FLCERR_DATA_EXCEEDS_MAX_SIZE                      = 0x70000005, /**< Provided data exceeds maximum size allowed. */
  FLCERR_DATA_INSUFFICIENT                          = 0x70000006, /**< The size of provided data is incorrect. */
  FLCERR_DATE_INVALID                               = 0x70000007, /**< Information for the date is invalid. */
  FLCERR_DICTIONARY_READ_ONLY                       = 0x70000008, /**< No longer used. */
  FLCERR_FEATURE_EXPIRED                            = 0x70000009, /**< Requested feature has expired. */
  FLCERR_FEATURE_HOST_ID_MISMATCH                   = 0x7000000a, /**< Requested feature's hostid does not match system hostid. */
  FLCERR_FEATURE_NOT_FOUND                          = 0x7000000b, /**< Requested feature was not found. */
  FLCERR_FEATURE_NOT_STARTED                        = 0x7000000c, /**< Start date for the requested feature is in the future. */
  FLCERR_FEATURE_VENDOR_NAME_INVALID                = 0x7000000d, /**< Feature was issued by a different vendor. */
  FLCERR_FEATURE_VERSION_NOT_FOUND                  = 0x7000000e, /**< Feature with the requested version was not found. */
  FLCERR_HOST_ID_UNSUPPORTED                        = 0x7000000f, /**< Type of the hostid is currently unsupported. */
  FLCERR_IDENTITY_UNSUPPORTED_VERSION               = 0x70000010, /**< Version of identity is not supported. */
  FLCERR_ITEM_ALREADY_EXISTS                        = 0x70000011, /**< Item already exists in the collection. */
  FLCERR_ITEM_NOT_FOUND                             = 0x70000012, /**< Provided item was not found in the collection. */
  FLCERR_ITEM_TYPE_MISMATCH                         = 0x70000013, /**< Item's value has a different type than expected. */
  FLCERR_INDEX_OUT_OF_BOUND                         = 0x70000014, /**< Provided index is out of bounds. */
  FLCERR_KEY_ALREADY_EXISTS                         = 0x70000015, /**< Key already exists in the collection. */
  FLCERR_KEY_NOT_FOUND                              = 0x70000016, /**< Provided key was not found in the collection. */
  FLCERR_LICENSE_NOT_FOUND                          = 0x70000017, /**< License not found in acquired list, possibly already returned. */
  FLCERR_LICENSE_SOURCE_TYPE_INVALID                = 0x70000018, /**< License source type is invalid for operation. */
  FLCERR_PRODUCT_TYPE_INVALID                       = 0x70000019, /**< No longer used. */
  FLCERR_RESPONSE_EXPIRED                           = 0x7000001a, /**< The allowed time to process response has expired. */
  FLCERR_RESPONSE_HOST_ID_MISMATCH                  = 0x7000001b, /**< Response does not match system hostid. */
  FLCERR_RESPONSE_SERVER_ERROR                      = 0x7000001c, /**< Server was not able to process request correctly. */
  FLCERR_RESPONSE_STALE                             = 0x7000001d, /**< Response is out of order with previous responses. */
  FLCERR_SIGNATURE_INVALID                          = 0x7000001e, /**< Signature didn't pass validation. */
  FLCERR_SIGNATURE_TYPE_MISMATCH                    = 0x7000001f, /**< Inconsistent signature type used. */
  FLCERR_TRIAL_ALREADY_LOADED                       = 0x70000020, /**< This trial has already been loaded. */
  FLCERR_TRIAL_EXPIRED                              = 0x70000021, /**< Trial duration has expired. */
  FLCERR_TRIAL_INVALID_ID                           = 0x70000022, /**< Trial ID is invalid. */
  FLCERR_TS_ANCHOR_BREAK                            = 0x70000023, /**< Storage anchor break found. */
  FLCERR_TS_BINDING_BREAK                           = 0x70000024, /**< Storage binding break found. */
  FLCERR_TS_CORRUPTED                               = 0x70000025, /**< Trusted storage is corrupted. */
  FLCERR_TS_INCONSISTENT_DATA                       = 0x70000026, /**< Trusted storage contains inconsistent data. */
  FLCERR_TS_UNSUPPORTED_VERSION                     = 0x70000027, /**< This version of trusted storage is not supported. */
  FLCERR_STORAGE_CLASS_INCOMPLETE                   = 0x70000028, /**< Storage implementation class provided is not complete. */
  FLCERR_VENDOR_KEYS_EXPIRED                        = 0x70000029, /**< Vendor keys have expired. */
  FLCERR_VENDOR_KEYS_INVALID                        = 0x7000002a, /**< Vendor keys are invalid. */
  FLCERR_VENDOR_KEYS_NOT_ENABLED                    = 0x7000002b, /**< Vendor keys do not support this platform. */
  FLCERR_IDENTITY_MISMATCH                          = 0x7000002c, /**< Identity data has changed; unable to decrypt trusted storage or anchor data. */
  FLCERR_WINDBACK_DETECTED                          = 0x7000002d, /**< Clock windback has been detected */
  FLCERR_WINDBACK_NOT_ENABLED                       = 0x7000002e, /**< Clock windback is disabled; unable to test if windback has happened. */
  FLCERR_DATA_UNSUPPORTED_VERSION                   = 0x7000002f, /**< Data version is not supported. */
  FLCERR_FEATURE_INSUFFICIENT_COUNT                 = 0x70000030, /**< Insufficient count for the requested feature. */
  FLCERR_OBJECT_READ_ONLY                           = 0x70000031, /**< Object cannot be modified because it is being used by another object. */
  FLCERR_VERSION_STRING_INVALID                     = 0x70000032, /**< Version string is invalid. */
  FLCERR_KEY_REVISION_UNAVAILABLE                   = 0x70000033, /**< A signature signed with a revision of key which was not present in identity data. */
  FLCERR_FEATURE_SERVER_HOST_ID_MISMATCH            = 0x70000034, /**< Requested feature's server hostid does not match system hostid. */
  FLCERR_NO_SERVER_DATA_FOUND                       = 0x70000035, /**< No server data found in TS. The Client has probably never received a capability response. */
  FLCERR_NO_SERVER_UPDATE_NEEDED                    = 0x70000036, /**< Regular update from the server is not needed as renew interval is set to 0 by the server. */
  FLCERR_FEATURE_SERVER_NODE_LOCKED                 = 0x70000037, /**< Feature is node locked and cannot be served by the server. */
  FLCERR_FEATURE_SERVER_DUPLICATE                   = 0x70000038, /**< Feature is a duplicate on the server and cannot be served. */
  FLCERR_INPUT_TYPE_MISMATCH                        = 0x70000039, /**< Input type mismatch. */
  FLCERR_NO_SERVER_RESPONDED                        = 0x7000003a, /**< Failed to get a response from any of the servers. */
  FLCERR_NO_NEW_SERVER_RESPONDED                    = 0x7000003b, /**< New servers sent by the configuration server are not responding. */
  FLCERR_CAPABILITY_RESPONSE_DATA_MISSING           = 0x7000003c, /**< Required data is missing from capability response. */
  FLCERR_SYNC_FROM_FNO_INCOMPLETE                   = 0x7000003d, /**< Capability response was not available - sync from the back office has not been completed. */
  FLCERR_IDENTITY_TYPE_INCORRECT                    = 0x7000003e, /**< Identity is of different type than expected. */
  FLCERR_MACHINE_TYPE_MISMATCH                      = 0x7000003f, /**< System machine type does not match expected machine type. */
  FLCERR_NO_UUID_FOUND                              = 0x70000040, /**< Requested unique identifier was not found. */
  FLCERR_BAD_CALLOUT_ERROR_CODE                     = 0x70000041, /**< Callout error was set using an inappropriate error code. */
  FLCERR_BAD_CALLOUT_UNIT_IDENTIFIER                = 0x70000042, /**< Callout error was set using an inappropriate unit identifier. */
  FLCERR_UNSUPPORTED_TOLERANCE_SPECIFIER_VERSION    = 0x70000043, /**< Tolerance specifier version is not supported. */
  FLCERR_NON_CLIENT_TOLERANCE_SPECIFIER             = 0x70000044, /**< A non-client tolerance specifier was specified by the client. */
  FLCERR_BADLY_FORMED_TOLERANCE_SPECIFIER           = 0x70000045, /**< A badly formed tolerance specifier was encountered. */
  FLCERR_TOLERANCE_SPECIFIER_REJECTED               = 0x70000046, /**< A tolerance specifier was rejected as not valid for this client. */
  FLCERR_UNSUPPORTED_TOLERANCE_SPECIFIER_TYPE       = 0x70000047, /**< An unsupported tolerance specifier type was specified. */
  FLCERR_BAD_TOLERANCE_SPECIFIER_RATIO              = 0x70000048, /**< A bad tolerance specifier ratio was specified. */
  FLCERR_INFORMATION_MESSAGE_USAGE_TYPE_CONFLICT    = 0x70000049, /**< Information message can hold either existing or usage-based features, but not both. */
  FLCERR_TS_HOST_ID_MISMATCH                        = 0x7000004a, /**< Trusted storage hostid does not match system hostid. */
  FLCERR_UUID_MISMATCH                              = 0x7000004b, /**< Response UUID does not match system UUID. */
  FLCERR_TS_DOES_NOT_EXIST                          = 0x7000004c, /**< Trusted storage does not exist. */
  FLCERR_UUID_SOURCE_CONFLICT                       = 0x7000004d, /**< The UUID in a message can originate from the back office or can be explicitly set, but not both. */
  FLCERR_INVALID_CHARSET                            = 0x7000004e, /**< Character set is invalid. */
  FLCERR_SHORT_CODE_LICENSE_EXPIRED                 = 0x7000004f, /**< Short code license has expired. */
  FLCERR_PUBLISHER_DATA_NOT_SET                     = 0x70000050, /**< Requested publisher data is not set. */
  FLCERR_CHECKSUM_SEGMENT_LENGTH_MISMATCH           = 0x70000051, /**< Checksum segment length mismatch. */
  FLCERR_SCHEME_NOT_SUPPORTED                       = 0x70000052, /**< Short code scheme is not supported. */
  FLCERR_BAD_SHORT_CODE_CRC                         = 0x70000053, /**< CRC validation of short code failed. */
  FLCERR_REQUEST_HOST_ID_MISMATCH                   = 0x70000054, /**< Request hostid does not match hostid recorded by server. */
  FLCERR_CHECKSUM_FAILURE                           = 0x70000055, /**< Checksum validation failed. */
  FLCERR_HOST_ID_SOURCE_CONFLICT                    = 0x70000056, /**< The hostid in an info message can originate from trusted storage or can be explicitly set, but not both. */
  FLCERR_FEATURE_BUFFER_METERED                     = 0x70000057, /**< Feature is metered and cannot be used in the buffer license source. */
  FLCERR_UNSUPPORTED_CERTIFICATE_KEYWORD            = 0x70000058, /**< Unsupported certificate keyword. */
  FLCERR_UNKNOWN_CERTIFICATE_KEYWORD                = 0x70000059, /**< Unknown certificate keyword. */
  FLCERR_DICTIONARY_SOURCE_CONFLICT                 = 0x7000005a, /**< Vendor dictionary can be requested as a whole, or by key; but not both. */
  FLCERR_DICTIONARY_NOT_INCLUDED                    = 0x7000005b, /**< Flag to include vendor dictionary has not been set. */
  FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT         = 0x7000005c, /**< The specified capability request option conflicts with an option previously set. */
  FLCERR_FEATURE_MAX_COUNT_EXCEEDED                 = 0x7000005d, /**< Feature count exceeds the maximum supported value. */
  FLCERR_FEATURE_OVERDRAFT_NOT_SUPPORTED            = 0x7000005e, /**< Features with an overdraft count are not supported on the client. */
  FLCERR_FEATURE_SERVER_OVERDRAFT_NOT_SUPPORTED     = 0x7000005f, /**< Features with an overdraft count are not supported on the server. */
  FLCERR_FEATURE_SERVER_METERED_NOT_SUPPORTED       = 0x70000060, /**< Features with a metered license model are not supported on the server. */
  FLCERR_LICENSE_MODEL_CONFLICT                     = 0x70000061, /**< The same feature name cannot be used in both metered and concurrent license model. */
  FLCERR_METERED_LICENSE_NOT_REUSABLE               = 0x70000062, /**< Acquired feature uses a metered non-reusable license model and cannot be returned. */
  FLCERR_FEATURE_METERED_ATTRIBUTE_CONFLICT         = 0x70000063, /**< Metered features with the same name must have identical metered attributes. */
  FLCERR_METERED_LICENSE_UNDO_INTERVAL_EXPIRED      = 0x70000064, /**< Undo interval for the acquired feature has expired. */
  FLCERR_METERED_FEATURES_NOT_ENABLED               = 0x70000065, /**< Metered functionality is not enabled. */
  FLCERR_LICENSE_TEMPLATE_ID_NOT_FOUND              = 0x70000066, /**< A short-code license matching the specified license template ID cannot be found. */
  FLCERR_CAPABILITY_RESPONSE_TYPE_INVALID           = 0x70000067, /**< Capability response type is invalid for operation. */
  FLCERR_LICENSE_SOURCE_RESPONSE_TYPE_INCOMPATIBLE  = 0x70000068, /**< License source type and response type is incompatible. */
  FLCERR_LICENSE_SOURCE_SERVER_HOST_ID_MISMATCH     = 0x70000069, /**< License source contains a different server hostid from the response server hostid. */
  FLCERR_LICENSE_SOURCE_SERVER_INSTANCE_MISMATCH    = 0x7000006a, /**< License source contains a different server instance than the capability request or response server instance. */
  FLCERR_CAPABILITY_SERVER_INSTANCE_MISMATCH        = 0x7000006b, /**< Capability response contains a different server instance than the capability request. */
  FLCERR_TS_DUPLICATE_SERVER_HOST_ID                = 0x7000006c, /**< Trusted storage already contains data from one of the capability response server hostids stored in a different instance location. */
  FLCERR_INFORMATION_MESSAGE_OPTION_CONFLICT        = 0x7000006d, /**< The specified information message option conflicts with an option previously set. */
  FLCERR_PREVIEW_RESPONSE_NOT_PROCESSED             = 0x7000006e, /**< Preview response cannot be processed into the license source. */
  FLCERR_FEATURE_PREVIEW_NOT_ACQUIRED               = 0x7000006f, /**< Feature from a preview response cannot be acquired. */
  FLCERR_NEXT
}FlcErrorCode;

typedef enum FlcCoeErrorCode
{
  FLCCOE_CONSTRAINT_FAILURE                         = 0x73000001, /**< A constraint error was encountered. */
  FLCCOE_UNEXPECTED_ENUM_VALUE                      = 0x73000002, /**< Unexpected enumeration value. */
  FLCCOE_MEMORY_ALLOCATION                          = 0x73000021, /**< Failed to allocate the requested memory. */
  FLCCOE_FILE_OPEN                                  = 0x73000041, /**< Failed file open. */
  FLCCOE_FILE_CLOSE                                 = 0x73000042, /**< Failed file close. */
  FLCCOE_FILE_SEEK                                  = 0x73000043, /**< Failed file seek. */
  FLCCOE_FILE_TELL                                  = 0x73000044, /**< Failed file tell. */
  FLCCOE_FILE_READ                                  = 0x73000045, /**< Failed file read. */
  FLCCOE_FILE_WRITE                                 = 0x73000046, /**< Failed file write. */
  FLCCOE_FILE_FLUSH                                 = 0x73000047, /**< Failed file flush. */
  FLCCOE_FILE_SIZE                                  = 0x73000048, /**< Failed file size. */
  FLCCOE_FILE_RESIZE                                = 0x73000049, /**< Failed file resize. */
  FLCCOE_FILE_DELETE                                = 0x7300004a, /**< Failed file delete. */
  FLCCOE_FILE_STAT                                  = 0x7300004b, /**< Failed file stat. */
  FLCCOE_FILE_TRUNCATE                              = 0x7300004c, /**< Failed file truncate. */
  FLCCOE_FILE_RENAME                                = 0x7300004d, /**< Failed file rename. */
  FLCCOE_FILE_ATTRIBUTE                             = 0x7300004e, /**< Failed file attribute change. */
  FLCCOE_FILE_SYNC                                  = 0x7300004f, /**< Failed file sync. */
  FLCCOE_TIME_API                                   = 0x73000061, /**< Call to platform time API failed. */
  FLCCOE_TIME_OVERFLOW                              = 0x73000062, /**< Overflow of storable time value limits occurred. */
  FLCCOE_NO_CLOCK_SUPPORT                           = 0x73000063, /**< Attempt to use clock functionality on system without clock. */
  FLCCOE_PATH_GENERATE                              = 0x73000071, /**< Call to path generation failed. */
  FLCCOE_NETWORK_API                                = 0x73000090, /**< Call to platform network API failed. */
  FLCCOE_DEVICE_IO                                  = 0x73000091, /**< Device IO failure. */
  FLCCOE_DEVICE_NOT_PRESENT                         = 0x73000092, /**< Device not present. */
  FLCCOE_DEVICE_NOT_SUPPORTED                       = 0x73000093, /**< Device not supported. */
  FLCCOE_SCRIPT_ERROR                               = 0x730000a0, /**< Script condition error. */
  FLCCOE_EXECUTE_ERROR                              = 0x730000a1, /**< File execution error. */
  FLCCOE_CRYPTO_CREATE_ERROR                        = 0x730000c0, /**< External crypto create error. */
  FLCCOE_CRYPTO_CIPHER_ERROR                        = 0x730000c1, /**< External crypto cipher error. */
  FLCCOE_CRYPTO_DIGEST_ERROR                        = 0x730000c2, /**< External message digest error. */
  FLCCOE_CRYPTO_HMAC_ERROR                          = 0x730000c3, /**< External hmac error. */
  FLCCOE_CRYPTO_SIGNING_ERROR                       = 0x730000c4, /**< External signing error. */
  FLCCOE_CRYPTO_RANDOM_ERROR                        = 0x730000c5, /**< External random number generator error. */
  FLCCOE_CRYPTO_STATE_ERROR                         = 0x730000c6, /**< External crypto state error. */
  FLCCOE_CRYPTO_DATA_ALIGNMENT_ERROR                = 0x730000c7, /**< External crypto data not block aligned. */
  FLCCOE_NEXT
}FlcCoeErrorCode;

typedef enum FlcCommErrorCode
{
  FLCSWC_COMM_CURL_ERROR                            = 0x75000001, /**< libCurl error, system error code contains the details in this case */
  FLCSWC_COMM_INITIALIZATION                        = 0x75000002, /**< Could not initialize a comm handle. */
  FLCSWC_COMM_WRITE_FAILURE                         = 0x75000003, /**< Received data could not be stored. */
  FLCSWC_COMM_HTTP_ERROR                            = 0x75000005, /**< Server returned an http error code */
  FLCSWC_COMM_READ_FAILURE                          = 0x75000011, /**< Unable to read file */
  FLCSWC_COMM_NEXT
}FlcCommErrorCode;

/** @} */ /* end of errorCodes group */
/** @} */ /* end of addtogroup clientCommon */

#endif /*_INCLUDE_FLC_ERRORCODES_H_*/
