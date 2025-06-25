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
 *  Provides the interface to get information from the error object.
 *
 *  The functions provided enable the caller to create and delete error objects
 *  and to query the error object for details on an error.
 */

/**
 * @addtogroup clientCommon
 * @{
 */

/**
 * @defgroup error Error
 * @{
 */

#ifndef _INCLUDE_FLC_ERROR_H_
#define _INCLUDE_FLC_ERROR_H_

#include "FlcTypes.h"
#include "FlcErrorCodes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Opaque data type for the error object.
*/
typedef struct flcError * FlcErrorRef;

/**
 *  Create an error object
 *
 *  @param  error     Pointer to the variable to receive the reference to the error object to be created
 *  @return FLC_TRUE on success else FLC_FALSE
 *
 *  @note The caller is responsible for deleting the error object via the FlcErrorDelete API.
 *
 *  @note The FlcError object is not thread safe and therefore cannot be shared between threads in a
 *        multi-threaded application.
 */
FlcBool FlcErrorCreate(FlcErrorRef * error);

/**
 *  Deletes the error object
 *
 *  @param  error    Pointer to variable containing the reference to the error object to be deleted
 *  @return FLC_TRUE on success else FLC_FALSE
 */
FlcBool FlcErrorDelete(FlcErrorRef * error);

/**
 *  Resets all error information contained in the error object
 *
 *  @param  error    Error object of interest
 *  @return FLC_TRUE on success else FLC_FALSE
 */
FlcBool FlcErrorReset(FlcErrorRef error);

/**
 *  Get error code stored in the provided error object
 *
 *  @param  error    Error object of interest
 *  @return Error code
 */
FlcInt32 FlcErrorGetCode(FlcErrorRef error);

/**
 *  Get system level error code stored in the provided error object
 *
 *  @param  error    Error object of interest
 *  @return System level error code
 */
FlcInt32 FlcErrorGetSystemCode(FlcErrorRef error);

/**
 *  Get error message stored in the provided error object
 *
 *  @param  error    Error object of interest
 *  @return Error message
 */
const FlcChar * FlcErrorGetMessage(FlcErrorRef error);

/**
 *  Get a basic description for the supplied error code
 *
 *  @param  code     Error code of interest
 *  @return Error code description
 */
const FlcChar * FlcErrorCodeGetDescription(FlcInt32 code);

/**
 *  If communicating with a back-office server and the returned code in the
 *  error object is FLCERR_RESPONSE_SERVER_ERROR then the error object system
 *  code may contain a value describing the back-office server error encountered. 
 *  Under these conditions this function may be used to obtain a textual  
 *  description of the error encountered by the back-office server.
 *
 *  @param  code     Back office server error code of interest
 *  @return Error code description
 */
const FlcChar * FlcBackOfficeErrorCodeGetDescription(FlcInt32 code);


/** @} */ /* end of error group */
/** @} */ /* end of addtogroup clientCommon */

#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_ERROR_H_ */
