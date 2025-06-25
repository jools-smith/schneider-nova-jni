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
 *  Enumeration describing the value type of a dictionary item.
 */

/**
 * @addtogroup dictionary
 * @{
 */

#ifndef _INCLUDE_FLC_DICTIONARY_TYPE_H_
#define _INCLUDE_FLC_DICTIONARY_TYPE_H_

/**
 *  Enumeration describing the value type of the dictionary item.
 */
typedef enum FlcDictionaryValueType
{
  FLC_DICTIONARY_UNKNOWN_VALUE                      = 0,
  FLC_DICTIONARY_STR_VALUE,
  FLC_DICTIONARY_INT_VALUE,
  FLC_DICTIONARY_MAX_VALUE = FLC_DICTIONARY_INT_VALUE
}FlcDictionaryValueType;

/** @} */ /* end of addtogroup dictionary */

#endif /*_INCLUDE_FLC_DICTIONARY_TYPE_H_*/
