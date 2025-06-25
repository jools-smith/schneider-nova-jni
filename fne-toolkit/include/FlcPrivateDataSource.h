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
 *  Provides an interface to the licensing client private data source object.
 *
 *  Private data source can be used to securely store, retrieve, and update
 *  vendor private data.
 */

#ifndef _INCLUDE_FLC_PRIVATEDATASOURCE_H_
#define _INCLUDE_FLC_PRIVATEDATASOURCE_H_

#include <time.h>
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
 * @defgroup privateDataSource Private Data Source
 * @{
 */

/**
 *  Clears all current items from private data source storage
 *
 *  @param  licensing   Licensing environment object
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcPrivateDataSourceClear(FlcLicensingRef    licensing,
                                  FlcErrorRef        error);

/**
 *  Adds an item to the private data source at the given index. 
 *
 *  @param  licensing   Licensing environment object
 *  @param  index       Index ranging from 0 to (max number of items - 1). See FlcPrivateDataSourceGetMaxItemCount.
 *  @param  data        Binary data of the item being added
 *  @param  size        Size of the binary data being added, limited by the max item size. See FlcPrivateDataSourceGetMaxItemSize.
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If the item at the given index already exists, an error is returned. 
 *        Use FlcPrivateDataSourceSetItem to overwrite existing data.
 *
 *  @note Possible error codes include
 *      FLCERR_INDEX_OUT_OF_BOUND,
 *      FLCERR_DATA_EXCEEDS_MAX_SIZE,
 *      FLCERR_ITEM_ALREADY_EXISTS
 */
FlcBool FlcPrivateDataSourceAddItem(FlcLicensingRef    licensing,
                                    FlcUInt32          index,
                                    FlcUInt8 *         data,
                                    FlcSize            size,
                                    FlcErrorRef        error);

/**
 *  Gets an item from the private data source at the given index
 *
 *  @param  licensing   Licensing environment object
 *  @param  index       Index ranging from 0 to (max number of items - 1). See FlcPrivateDataSourceGetMaxItemCount.
 *  @param  data        Pointer to variable to receive private data source item data
 *  @param  size        Pointer to variable to receive private data source item size
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note On success the caller will be responsible for deallocating the private data source data 
 *        storage using the FlcMemoryFree API.
 *
 *  @note If the item at the given index does not exist, an error is returned.
 *
 *  @note Possible error codes include
 *      FLCERR_INDEX_OUT_OF_BOUND,
 */
FlcBool FlcPrivateDataSourceGetItem(FlcLicensingRef    licensing,
                                    FlcUInt32          index,
                                    FlcUInt8 **        data,
                                    FlcSize *          size,
                                    FlcErrorRef        error);

/**
 *  Overwrites the item in the private data source at the given index 
 *
 *  @param  licensing   Licensing environment object
 *  @param  index       Index ranging from 0 to (max number of items - 1). See FlcPrivateDataSourceGetMaxItemCount.
 *  @param  data        Binary item data
 *  @param  size        Size of the binary data being set limited by the max item size. See FlcPrivateDataSourceGetMaxItemSize.
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If the item at the given index does not exist, an error is returned. 
 *        Use FlcPrivateDataSourceAddItem API instead.
 *
 *  @note Possible error codes include
 *        FLCERR_INDEX_OUT_OF_BOUND,
 *        FLCERR_ITEM_NOT_FOUND,
 *        FLCERR_DATA_EXCEEDS_MAX_SIZE
 */
FlcBool FlcPrivateDataSourceSetItem(FlcLicensingRef    licensing,
                                    FlcUInt32          index,
                                    FlcUInt8 *         data,
                                    FlcSize            size,
                                    FlcErrorRef        error);

/**
 *  Deletes an item from the private data source at the given index
 *
 *  @param  licensing   Licensing environment object
 *  @param  index       Index ranging from 0 to (max number of items - 1). See FlcPrivateDataSourceGetMaxItemCount
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *      FLCERR_INDEX_OUT_OF_BOUND,
 *      FLCERR_ITEM_NOT_FOUND,
 */
FlcBool FlcPrivateDataSourceDeleteItem(FlcLicensingRef    licensing,
                                       FlcUInt32          index,
                                       FlcErrorRef        error);

/**
 *  Indicates whether an item exists in the private data source at the given index. 
 *
 *  @param  licensing    Licensing environment object
 *  @param  index        Index ranging from 0 to (max number of items - 1). See FlcPrivateDataSourceGetMaxItemCount
 *  @param  itemExists   Pointer to variable set to FLC_TRUE if a private data source item exists at the supplied
 *                       index, FLC_FALSE otherwise
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *      FLCERR_INDEX_OUT_OF_BOUND
 */
FlcBool FlcPrivateDataSourceItemExists(FlcLicensingRef    licensing,
                                       FlcUInt32          index,
                                       FlcBool *          itemExists,
                                       FlcErrorRef        error);

/**
 *  Returns the maximum number of items possible in the private data source. 
 *
 *  @param  licensing    Licensing environment object
 *  @param  maxCount     Pointer to variable to receive the max number of items in the private data
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Maximum number of items in the private data source is guaranteed to be no less than 16.
 */
FlcBool FlcPrivateDataSourceGetMaxItemCount(FlcLicensingRef    licensing,
                                            FlcUInt32 *        maxCount,
                                            FlcErrorRef        error);

/**
 *  Returns the maximum size of an item in the private data source
 *
 *  @param  licensing     Licensing environment object
 *  @param  maxSize       Ponter to variable to receive the max size of a private data item.
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Maximum item size is guaranteed to be no less than 4K.
 */
FlcBool FlcPrivateDataSourceGetMaxItemSize(FlcLicensingRef    licensing,
                                           FlcUInt32 *        maxSize,
                                           FlcErrorRef        error);

/**
 *  Gets the creation time of the private data source storage
 *
 *  @param  licensing    Licensing environment object
 *  @param  timestamp    Variable to receive the private data source creation time
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note This function returns the time when the STORAGE for the private data source was created.
 *
 *  @note The information obtained from this function can be used to invalidate licenses that were issued
 *        prior to re-creation of the private data source storage.
 *
 *  @note If the private data source STORAGE is created for the very first time on the device, 
 *        the returned timestamp is 0.
 */
FlcBool FlcPrivateDataSourceGetLastCreationTime(FlcLicensingRef    licensing,
                                                time_t *           timestamp,
                                                FlcErrorRef        error);

/** @} */ /* end of privateDataSource group */
/** @} */ /* end of addtogroup licensingClient */


#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_PRIVATEDATASOURCE_H_ */
