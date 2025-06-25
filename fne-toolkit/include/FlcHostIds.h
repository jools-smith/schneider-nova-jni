/****************************************************************************
  Copyright (c) 2013-2020 Flexera Software LLC.
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
 *  Provides an interface to query a collection of host ids.

    The functions provided enable the caller to access the returned host id
  collection associated with the current platform, feature or capability response.
 */

#ifndef _INCLUDE_FLC_HOSTIDS_H_
#define _INCLUDE_FLC_HOSTIDS_H_

#include "FlcTypes.h"
#include "FlcError.h"
#include "FlcHostIdType.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup clientCommon
 * @{
 */

/**
 * @defgroup hostids HostIds
 * @{
 */

/** Opaque data type for the host ids collection
*/
typedef struct flcHostIds * FlcHostIdsRef;

/**
 *  Deletes the host ids collection
 *
 *  @param  hostIds     Pointer to the variable containing the reference to the collection of host ids
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcHostIdsDelete(FlcHostIdsRef * hostIds,
                         FlcErrorRef     error);

/**
 *  Gets the count of host ids in the host id collection
 *
 *  @param  hostIds   Host ids collection object of interest
 *  @param  idCount   Pointer to variable to receive returned id count
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcHostIdsGetIdCount(FlcHostIdsRef hostIds,
                             FlcUInt32 *   idCount,
                             FlcErrorRef   error);

/**
 *  Gets information on the host id at a given index in the host id collection
 *
 *  @param  hostIds   Host ids collection object of interest
 *  @param  index       Index, ranging from 0 to (host ids collection size - 1)
 *  @param  idType    Pointer to a variable to receive returned host id type defined by FlcHostIdType
 *  @param  idValue   Pointer to a variable to receive returned host id value
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCERR_INDEX_OUT_OF_BOUND
 */
FlcBool FlcHostIdsGetId(FlcHostIdsRef     hostIds,
                        FlcUInt32         index,
                        FlcInt32 *        idType,
                        const FlcChar **  idValue,
                        FlcErrorRef       error);


/** @} */ /* end of hostids group */
/** @} */ /* end of addtogroup clientCommon */

#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_HOSTIDS_H_ */
