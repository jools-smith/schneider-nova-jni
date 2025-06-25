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

#ifndef _INCLUDE_FLCSWC_ERRORCODES_H_
#define _INCLUDE_FLCSWC_ERRORCODES_H_

/** @file
 *  Provides software client specific returned error code values.
 */

/**
 * @addtogroup errorCodes
 * @{
 */

typedef enum FlcSwcErrorCode
{
  FLCSWC_MEMORY_ALLOCATION                          = 0x74000001, /**< Failed to allocate the required memory. */
  FLCSWC_FILE_OPEN                                  = 0x74000002, /**< File open failed. */
  FLCSWC_EMPTY_FILE                                 = 0x74000003, /**< Empty file. */
  FLCSWC_FILE_READ                                  = 0x74000004, /**< File read error. */
  FLCSWC_HOSTID_NOT_PRESENT                         = 0x74000005, /**< Hostid not present on platform. */
  FLCSWC_REGEX_EXPRESSION_ERROR                     = 0x74000006, /**< Regular expression pattern error encountered. */
  FLCSWC_COMM_MISSING                               = 0x74000007, /**< Unable to locate FlxComm/FlxComm64 library. */
  FLCSWC_COMM_ERROR                                 = 0x74000008, /**< Generic communications error. */
  FLCSWC_STATE_ERROR                                = 0x74000009, /**< Request invalid for object state. */
  FLCSWC_FILE_SEEK                                  = 0x7400000a, /**< File seek error. */
  FLCSWC_FILE_WRITE                                 = 0x7400000b, /**< File write error. */
  FLCSWC_FILE_RENAME                                = 0x7400000c, /**< File rename error. */
  FLCSWC_FILE_DELETE                                = 0x7400000d, /**< File delete error. */
  FLCSWC_NO_REQUEST_DATA_FOUND                      = 0x7400000e, /**< No request data found. */
  FLCSWC_EXPECTED_HTTP_ACCEPT                       = 0x7400000f, /**< Expected status HTTP ACCEPT 202. */
  FLCSWC_NO_DATA_RECEIVED                           = 0x74000010, /**< No data received from server. */  
  FLCSWC_NEXT,
} FlcSwcErrorCode;

/** @} */ /* end of addtogroup errorCodes */

#endif /*_INCLUDE_FLCSWC_ERRORCODES_H_*/
