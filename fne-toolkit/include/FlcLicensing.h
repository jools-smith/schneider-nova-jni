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
 *  Provides an interface to licensing environment functionality.
 */

#ifndef _INCLUDE_FLC_LICENSING_H_
#define _INCLUDE_FLC_LICENSING_H_

#include <time.h>
#include "FlcTypes.h"
#include "FlcLicensingErrorCodes.h"
#include "FlcError.h"
#include "FlcHostIds.h"
#include "FlcMessageType.h"
#include "FlcLicenseServerInstance.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup licensingClient
 * @{
 */

/**
 * @defgroup licensing Licensing
 * @{
 */

/** Opaque data type for the licensing object.
*/
typedef struct flcLicensing * FlcLicensingRef;

/**
 *  Creates the licensing environment object
 *
 *  @param  licensing     Pointer to the variable to receive the reference to the licensing environment to be created
 *  @param  identityData  Pointer to binary data containing information about the client-side publisher identity
 *  @param  identitySize  Size of binary data containing information about the client-side publisher identity
 *  @param  storagePath   Trusted storage path or NULL if using in-memory trusted storage
 *  @param  customHostId  Custom string host id or NULL if not using a custom host id
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCSWC_CORE_MISSING
 */
FlcBool FlcLicensingCreate(FlcLicensingRef * licensing,
                           const FlcUInt8 *  identityData,
                           FlcSize           identitySize,
                           const FlcChar *   storagePath,
                           const FlcChar *   customHostId, 
                           FlcErrorRef       error);
/**
 *  Deletes the licensing environment object
 *
 *  @param  licensing   Pointer to the variable containing the reference to the licensing environment to be deleted
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Deleting the licensing environment will return all acquired licenses and free all license sources.
 */
FlcBool FlcLicensingDelete(FlcLicensingRef * licensing,
                           FlcErrorRef       error);

/**
 *  Gets the custom host id
 *
 *  @param  licensing   Licensing environment object
 *  @param  hostId      Pointer to variable to receive the current custom string host id or NULL if no custom
 *                      host id exists
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetCustomHostId(FlcLicensingRef  licensing,
                           const FlcChar ** hostId,
                           FlcErrorRef      error);

/**
 *  Sets the custom host id
 *
 *  @param  licensing   Licensing environment object
 *  @param  hostId      Pointer to the custom string host id or NULL if no custom host id is desired
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcSetCustomHostId(FlcLicensingRef licensing,
                           const FlcChar * hostId,
                           FlcErrorRef     error);

/**
 *  Gets the current version of the client library 
 *
 *  @param  licensing   Licensing environment object
 *  @param  version     Pointer to variable to receive the current version of the client library
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetClientVersion(FlcLicensingRef  licensing,
                            const FlcChar ** version,
                            FlcErrorRef      error);

/**
 *  Gets the version of FlxCore(64).dll being used by the licensing environment
 *
 *  @param  licensing   Licensing environment object
 *  @param  version     Pointer to variable to receive the version of FlxCore(64).dll being used
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetLicensingVersion(FlcLicensingRef  licensing,
                               const FlcChar ** version,
                               FlcErrorRef      error);

/**
 *  Sets the execution environment host name
 *
 *  @param  licensing   Licensing environment object
 *  @param  hostName    Execution environment host name
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcSetHostName(FlcLicensingRef licensing, 
                       const FlcChar * hostName,
                       FlcErrorRef     error);

/**
 *  Gets the execution environment host name
 *
 *  @param  licensing   Licensing environment object
 *  @param  hostName    Pointer to the variable to receive the execution environment host name
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The pointer returned is valid until the next FlcGeHostName API call.
 */
FlcBool FlcGetHostName(FlcLicensingRef  licensing, 
                       const FlcChar ** hostName,
                       FlcErrorRef      error);

/**
 *  Sets the execution environment host type
 *
 *  @param  licensing   Licensing environment object
 *  @param  hostType    Execution environment host type
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcSetHostType(FlcLicensingRef licensing, 
                       const FlcChar * hostType,
                       FlcErrorRef     error);

/**
 *  Gets the execution environment host type
 *
 *  @param  licensing   Licensing environment object
 *  @param  hostType    Pointer to the variable to receive the execution environment host type
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The pointer returned is valid until the next FlcGeHostType API call.
 */
FlcBool FlcGetHostType(FlcLicensingRef  licensing, 
                       const FlcChar ** hostType,
                       FlcErrorRef      error);

/**
 *  Sets whether or not virtual machine detection is enabled
 *
 *  @param  licensing   Licensing environment object
 *  @param  enabled     Set to FLC_TRUE if virtual machine detection is to enabled, FLC_FALSE otherwise
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcSetVmDetectionEnabled(FlcLicensingRef licensing,
                                 FlcBool         enabled,
                                 FlcErrorRef     error);

/**
 *  Creates and returns a reference to a collection of host ids
 *
 *  @param  licensing   Licensing environment object
 *  @param  hostIds     Pointer to the variable to receive the reference to the collection of host ids
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The caller is responsible for deleting the host ids collection via the 
 *          FlcHostIdsDelete API.
 */
FlcBool FlcGetHostIds(FlcLicensingRef licensing,
                      FlcHostIdsRef * hostIds,
                      FlcErrorRef     error);

/**
 *  Gets the message type associated with a binary message
 *
 *  @param  licensing   Licensing environment object
 *  @param  type        Pointer to variable to receive the message type or FLC_MESSAGE_TYPE_UNKNOWN if
 *                      the supplied binary data is not recognized
 *  @param  message     Pointer to binary message data
 *  @param  size        Size of binary message data
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetMessageType(FlcLicensingRef  licensing,
                          FlcMessageType * type,
                          const FlcUInt8 * message,
                          FlcSize          size,
                          FlcErrorRef      error);
/**
 *  Determines whether or not the supplied date represents a perpetual date
 *
 *  @param  date          Pointer to date
 *  @param  isPerpetual   Pointer to variable set to FLC_TRUE if supplied date represents a 
 *                        perpetual date, FLC_FALSE otherwise
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcDateIsPerpetual(const struct tm * date,
                           FlcBool *         isPerpetual,
                           FlcErrorRef       error);

/**
 *  Determines whether or not the supplied date is expired
 *
 *  @param  date        Pointer to date
 *  @param  isExpired   Pointer to variable set to FLC_TRUE if supplied date is in the future,
 *                      FLC_FALSE otherwise
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcDateIsExpired(const struct tm * date,
                         FlcBool *         isExpired,
                         FlcErrorRef       error);

/**
 *  Frees binary message data memory returned by the licensing environment.
 *
 *  @param  memory  Binary message data memory to free
 *
 *  @note This API should be used to free generated capability request, information message, 
 *        and short code request data in addition to returned private data source item data.
 */
void FlcMemoryFree(void * memory);

/** @} */ /* end of licensing group */
/** @} */ /* end of addtogroup licensingClient */


#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_LICENSING_H_ */
