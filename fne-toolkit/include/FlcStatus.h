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
 *  Provides an interface to the licensing client to retrieve status
 *  information from a capability response or error status message.
 *
 *  These functions enable the caller to iterate through the status collection
 *  and extract information from individual status items contained within the 
 *  capability response or error status message.
 */

#ifndef _INCLUDE_FLC_STATUS_H_
#define _INCLUDE_FLC_STATUS_H_

#include "FlcTypes.h"
#include "FlcError.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup licensingClient
 * @{
 */

/**
 * @defgroup status Status
 * @{
 */

/** Opaque data type for the status collection object.
*/
typedef struct flcStatusCollection * FlcStatusCollectionRef;

/**
 *  Get number of status items returned.
 *
 *  @param  statusCollection  Status collection oject
 *  @param  size                Pointer to variable to receive the status collection size. 
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcStatusCollectionSize(FlcStatusCollectionRef statusCollection,
                                FlcSize *              size,
                                FlcErrorRef            error);
/**
 *  Get a status item information by index.
 *
 *  @param  statusCollection  Status collection oject
 *  @param  index             Index, ranging from 0 to (status collection size - 1)
 *  @param  code              Pointer to variable to receive the status code
 *  @param  type              Pointer to variable to receive the status type
 *  @param  details           Pointer to variable to receive the status details
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *  @note Possible error codes include
 *      FLCERR_INDEX_OUT_OF_BOUND
 *
 *  @note   For the responses received from FlexNet Embedded server:
 *          status type is internal,
 *          status code comes from FlcMessageStatus.h,
 *          status details contain feature name and version.
 *
 *  @note   For the responses received from FNO:
 *          status type is internal,
 *          status code comes from FlcMessageStatus.h,
 *          status details contain rights ID.
 *
 *  @note   For the responses received from back-office other than FNO:
 *          status category is external,
 *          status code is defined by the manufacturer,
 *          status details are defined by the manufacturer.
 */
FlcBool FlcStatusCollectionGetItem(FlcStatusCollectionRef  statusCollection,
                                   FlcUInt32               index,
                                   FlcInt32 *              code,
                                   FlcInt32 *              type,
                                   const FlcChar **        details,
                                   FlcErrorRef             error);

/**
 *  Format a string containing status item information by index.
 *
 *  @param  statusCollection  Status collection oject
 *  @param  index             Index, ranging from 0 to (status collection size - 1)
 *  @param  formattedInfo     Pointer to variable to receive the formatted status information
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *  @note Possible error codes include
 *      FLCERR_INDEX_OUT_OF_BOUND
 *
 *  @note   The address of the formatted status information returned is valid until the status
 *          collection is deleted.
 *
 *  @note   The status information returned will be overwritten by subsequent calls to this 
 *          function for the same status collection.
 */
FlcBool FlcStatusCollectionFormatItem(FlcStatusCollectionRef  statusCollection,
                                      FlcUInt32               index,
                                      const FlcChar **        formattedInfo,
                                      FlcErrorRef             error);

/**
 *  Get a basic description for the supplied statuscode
 *
 *  @param  code     Status code of interest
 *  @return Error code description
 */
const FlcChar * FlcStatusCodeGetDescription(FlcInt32 code);

/** @} */ /* end of status group */
/** @} */ /* end of addtogroup licensingClient */


#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_STATUS_H_ */
