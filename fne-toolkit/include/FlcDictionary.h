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
 *  Provides the interface to the dictionary object.
 *
 *  These functions enable the caller to retrieve data from the dictionary.
 *  An item in the dictionary is represented as a key-value pair.
 */

#ifndef _INCLUDE_FLC_DICTIONARY_H_
#define _INCLUDE_FLC_DICTIONARY_H_

#include <time.h>

#include "FlcTypes.h"
#include "FlcError.h"
#include "FlcDictionaryType.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup licensingClient
 * @{
 */

 /**
 * @defgroup dictionary Dictionary
 * @{
 */

/** Opaque data type for the dictionary object.
*/
typedef struct flcDictionary * FlcDictionaryRef;

/**
 *  Gets the size of the dictionary object
 *
 *  @param  dictionary  Dictionary object of interest
 *  @param  size          Pointer to variable to receive returned size
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcDictionaryGetSize(FlcDictionaryRef  dictionary,
                             FlcUInt32 *       size,
                             FlcErrorRef       error);

/**
 *  Gets the type of the value at a given index
 *
 *  @param  dictionary  Dictionary object of interest
 *  @param  index         Index, ranging from 0 to (dictionary size - 1)
 *  @param  type          Pointer to variable to receive the returned value type defined by FlcDictionaryValueType
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *      FLCERR_INDEX_OUT_OF_BOUND
 */
FlcBool FlcDictionaryGetValueType(FlcDictionaryRef          dictionary,
                                  FlcUInt32                 index,
                                  FlcDictionaryValueType *  type,
                                  FlcErrorRef               error);

/**
 *  Gets data item (unique key and integer value associated with it) from the dictionary
 *  based on the given index.
 *
 *  @param  dictionary  Dictionary object of interest
 *  @param  index       Index, ranging from 0 to (dictionary size - 1)
 *  @param  key         Pointer to variable to receive the returned key value
 *  @param  value       Pointer to variable to receive the returned integer value
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 *  
 *  @note Possible error codes include
 *      FLCERR_INDEX_OUT_OF_BOUND,
 *      FLCERR_ITEM_TYPE_MISMATCH
 */
FlcBool FlcDictionaryGetIntItem(FlcDictionaryRef  dictionary,
                                FlcUInt32         index,
                                const FlcChar **  key,
                                FlcInt32 *        value,
                                FlcErrorRef       error);

/**
 *  Gets data item (unique key and string value associated with it) from the dictionary
 *  based on the given index.
 *
 *  @param  dictionary  Dictionary object of interest
 *  @param  index       Index, ranging from 0 to (dictionary size - 1)
 *  @param  key         Pointer to variable to receive the returned key value
 *  @param  value       Pointer to variable to receive the returned string value
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 *  
 *  @note Possible error codes include
 *      FLCERR_INDEX_OUT_OF_BOUND,
 *      FLCERR_ITEM_TYPE_MISMATCH
 */

FlcBool FlcDictionaryGetStringItem(FlcDictionaryRef    dictionary,
                                   FlcUInt32           index,
                                   const FlcChar **    key,
                                   const FlcChar **    value,
                                   FlcErrorRef         error);
/**
 *  Checks whether an item with a given key exists in the dictionary
 *
 *  @param  dictionary  Dictionary object of interest
 *  @param  key         Unique key name
 *  @param  exists      Pointer to variable to receive the returned indicator: FLC_TRUE if key exists; FLC_FALSE otherwise.
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcDictionaryKeyExists(FlcDictionaryRef    dictionary,
                               const FlcChar *     key,
                               FlcBool *           exists,
                               FlcErrorRef         error);

/**
 *  Gets the type of the value associated with a unique key
 *
 *  @param  dictionary  Dictionary object of interest
 *  @param  key         Unique key name
 *  @param  type        Pointer to variable to receive the returned value type defined by FlcDictionaryValueType
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *      FLCERR_KEY_NOT_FOUND
 */
FlcBool FlcDictionaryGetValueTypeByKey(FlcDictionaryRef          dictionary,
                                       const FlcChar *           key,
                                       FlcDictionaryValueType *  type,
                                       FlcErrorRef               error);

/**
 *  Gets integer value associated with a unique key
 *
 *  @param  dictionary  Dictionary object of interest
 *  @param  key         Unique key name
 *  @param  value       Pointer to variable to receive the returned integer value
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *  
 *  @note Possible error codes include
 *      FLCERR_ITEM_TYPE_MISMATCH 
 */
FlcBool FlcDictionaryGetIntValue(FlcDictionaryRef    dictionary,
                                 const FlcChar *     key,
                                 FlcInt32 *          value,
                                 FlcErrorRef         error);

/**
 *  Gets string value associated with a unique key
 *
 *  @param  dictionary  Dictionary object of interest
 *  @param  key         Unique key name
 *  @param  value       Pointer to variable to receive the returned integer value
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *  
 *  @note Possible error codes include
 *      FLCERR_ITEM_TYPE_MISMATCH
 */
FlcBool FlcDictionaryGetStringValue(FlcDictionaryRef    dictionary,
                                    const FlcChar *     key,
                                    const FlcChar **    value,
                                    FlcErrorRef         error);

/** @} */ /* end of dictionary group */
/** @} */ /* end of addtogroup licensingClient */


#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_DICTIONARY_H_ */
