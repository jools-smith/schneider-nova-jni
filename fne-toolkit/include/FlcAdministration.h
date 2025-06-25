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
 *  Provides the licensing client storage administrative interface.
 *
 *  The functions provided enable the caller to delete the storage used to 
 *  contain license and private data information.
 */

#ifndef _INCLUDE_FLC_ADMINISTRATION_H_
#define _INCLUDE_FLC_ADMINISTRATION_H_

#include "FlcTypes.h"
#include "FlcError.h"
#include "FlcLicensing.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup licensingClient
 * @{
 */

/**
 * @defgroup administration Administration
 * @{
 */

/**
 *  Enumeration indication the target storage type for the administrative API.
 */
typedef enum FlcAdminStorageType
{
  FLC_ADMIN_TRUSTED_STORAGE  = 0,
  FLC_ADMIN_TRIALS           = 1,
  FLC_ADMIN_SHORT_CODES      = 2,
  FLC_ADMIN_PRIVATE_DATA     = 3
} FlcAdminStorageType;

/**
 *  Deletes storage associated with the provided storage type
 *
 *  @param  licensing   Licensing environment object
 *  @param  type        Type of storage to delete
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If a license source of the specified type exists, it will be synchronized with the empty storage.
 */
FlcBool FlcAdministrationDelete(FlcLicensingRef licensing, FlcAdminStorageType type, FlcErrorRef error);

/**
 *  Deletes storage associated with Trusted Storage
 *
 *  @param  licensing   Licensing environment object
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If Trusted Storage license sources exist, they will be synchronized with the empty storage.
 */
FlcBool FlcAdministrationDeleteTrustedStorage(FlcLicensingRef licensing, FlcErrorRef error);

/**
 *  Deletes storage associated with Trial Licenses
 *
 *  @param  licensing   Licensing environment object
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If a trial license source exists, it will be synchronized with the empty storage.
*/
FlcBool FlcAdministrationDeleteTrials(FlcLicensingRef licensing, FlcErrorRef error);

/**
 *  Deletes storage associated with the short code licenses.
 *
 *  @param  licensing   Licensing environment object
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If a short code license source exists, it will be synchronized with the empty storage.
 */
FlcBool FlcAdministrationDeleteShortCodes(FlcLicensingRef licensing, FlcErrorRef error);

/**
 *  Deletes storage associated with the Private Data Source
 *
 *  @param  licensing   Licensing environment object
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcAdministrationDeletePrivateData(FlcLicensingRef licensing, FlcErrorRef error);

/**
 *  Deletes short code activated licenses corresponding to the specified template id
 *  from short-code storage.
 *
 *  @param  licensing   Licensing environment object
 *  @param  templateId  Template id corresponding to the licenses to be deleted
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcAdministrationDeleteShortCode(FlcLicensingRef licensing, FlcUInt16 templateId, FlcErrorRef error);

/**
 *  Deletes that portion of trusted storage associated with the provided license server instance.
 *
 *  @param  licensing       Licensing environment object
 *  @param  serverInstance  License server instance trusted storage data to delete.
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If a trusted storage license source for the specified license server instance exists,
 *        it will be synchronized with the empty storage.
 */
FlcBool FlcAdministrationDeleteTrustedStorageServerInstance(FlcLicensingRef licensing,
                                                            FlcLicenseServerInstance serverInstance,
                                                            FlcErrorRef error);

/** @} */ /* end of administration group */
/** @} */ /* end of addtogroup licensingClient */


#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_ADMINISTRATION_H_ */
