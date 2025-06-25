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
 *  Provides an interface to get license information.

    A License Reference (FlcLicenseRef) is returned to the caller after
    successfully acquiring a license. The functions provided enable the caller to get
    information about the license, such as name, version, expiration, etc.
 */

#ifndef _INCLUDE_FLC_LICENSE_H_
#define _INCLUDE_FLC_LICENSE_H_

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
 * @defgroup license License
 * @{
 */

/** Opaque data type for the license object.
*/
typedef struct flcLicense *           FlcLicenseRef;

/**
 *  Deletes the license object
 *
 *  @param  license   Pointer to the variable to containing the reference to the license object to be deleted
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Deleting the license object will release the license object storage but does not return or release
 *        the license feature. This can only be accomplished by FlcReturnLicense (returnable licenses) or
 *        FlcReleaseLicense (non-returnable licenses).
 */
FlcBool FlcLicenseDelete(FlcLicenseRef *            license,
                         FlcErrorRef                error);

/**
 *  Gets the license name
 *
 *  @param  license   License object of interest 
 *  @param  name      Pointer to variable to receive returned license name
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcLicenseGetName(FlcLicenseRef    license, 
                          const FlcChar ** name,
                          FlcErrorRef      error);

/**
 *  Gets the license version
 *
 *  @param  license   License object of interest 
 *  @param  version   Pointer to variable to receive returned license version
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If the license was acquired from more than one feature the version of the first feature
 *        used to satisfy the acquisition request is returned.
 */
FlcBool FlcLicenseGetVersion(FlcLicenseRef    license,
                             const FlcChar ** version,
                             FlcErrorRef      error);

/**
 *  Gets the license count
 *
 *  @param  license   License object of interest 
 *  @param  count     Pointer to variable to receive returned license count
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcLicenseGetCount(FlcLicenseRef    license,
                           FlcInt32 *       count,
                           FlcErrorRef      error);

/**
 *  Detects whether the acquired feature in the license uses a metered license model.
 *
 *  @param  license     License object of interest
 *  @param  isMetered   Pointer to variable to receive returned metered indicator. This indicator
 *                      will be set to FLC_TRUE for a metered feature (MODEL=metered); FLC_FALSE
 *                      otherwise and on error
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcLicenseIsMetered(FlcLicenseRef    license,
                            FlcBool *        isMetered,
                            FlcErrorRef      error);


/**
 *  Detects whether the acquired feature in the license uses a metered reusable license model.
 *
 *  @param  license             License object of interest
 *  @param  isMeteredReusable   Pointer to variable to receive returned reusable metered indicator.
 *                              This indicator will be set to FLC_TRUE for a reusable metered feature
 *                              (MODEL=metered REUSABLE); FLC_FALSE otherwise and on error 
 *  @param  error                 Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcLicenseIsMeteredReusable(FlcLicenseRef    license,
                                    FlcBool *        isMeteredReusable,
                                    FlcErrorRef      error);


/**
 *  Gets serial number string of the license
 *
 *  @param  license       License object of interest 
 *  @param  serialNumber  Pointer to variable to receive returned license serial number (SN value),
 *                        if present,NULL otherwise
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If the license was acquired from more than one feature the serial number string of the
 *        first feature used to satisfy the acquisition request is returned.
 */
FlcBool FlcLicenseGetSerialNumber(FlcLicenseRef     license,
                                  const FlcChar **  serialNumber,
                                  FlcErrorRef       error);

/**
 *  Gets notice string of the license
 *
 *  @param  license   License object of interest 
 *  @param  notice    Pointer to variable to receive returned license notice (NOTICE value) if present,
 *                    NULL otherwise
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If the license was acquired from more than one feature the notice string of the
 *        first feature used to satisfy the acquisition request is returned.
 */
FlcBool FlcLicenseGetNotice(FlcLicenseRef     license, 
                            const FlcChar **  notice,
                            FlcErrorRef       error);

/**
 *  Gets issuer string of the license
 *
 *  @param  license   License object of interest 
 *  @param  issuer    Pointer to variable to receive returned license issuer (ISSUER value) if present,
 *                    NULL otherwise
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If the license was acquired from more than one feature the issuer string of the
 *        first feature used to satisfy the acquisition request is returned.
 */
FlcBool FlcLicenseGetIssuer(FlcLicenseRef     license,
                            const FlcChar **  issuer,
                            FlcErrorRef       error);

/**
 *  Gets requestor id
 *
 *  @param  license      License object of interest 
 *  @param  requestorId  Pointer to variable to receive the requestor id associated with this license
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcLicenseGetRequestorId(FlcLicenseRef     license,
                                 const FlcChar **  requestorId,
                                 FlcErrorRef       error);

/**
 *  Gets acquisition id
 *
 *  @param  license        License object of interest 
 *  @param  acquisitionId  Pointer to variable to receive the acquisition id associated with this license
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcLicenseGetAcquisitionId(FlcLicenseRef     license,
                                   const FlcChar **  acquisitionId,
                                   FlcErrorRef       error);

/**
 *  Gets vendor string of the license
 *
 *  @param  license       License object of interest 
 *  @param  vendorString  Pointer to variable to receive returned license vendor string
 *                        (VENDOR_STRING value) if present, NULL otherwise
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If the license was acquired from more than one feature the vendor string of the
 *        first feature used to satisfy the acquisition request is returned.
 */
FlcBool FlcLicenseGetVendorString(FlcLicenseRef     license, 
                                  const FlcChar **  vendorString,
                                  FlcErrorRef       error);

/**
 *  Gets issued date of the license
 *
 *  @param  license  License object of interest 
 *  @param  issued   Pointer to variable to receive returned license issued date if present,
 *                   NULL otherwise
 *  @param  error      Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If the license was acquired from more than one feature the issued date of the
 *        first feature used to satisfy the acquisition request is returned.
 */
FlcBool FlcLicenseGetIssued(FlcLicenseRef       license, 
                            const struct tm **  issued,
                            FlcErrorRef         error);

/**
 *  Gets expiration date of the license
 *
 *  @param  license    License object of interest 
 *  @param  expiration Pointer to variable to receive returned license expiration date
 *  @param  error        Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If the license was acquired from more than one feature the earliest expiration 
 *        date among the features used to satisfy the acquisition request is returned.
 */
FlcBool FlcLicenseGetExpiration(FlcLicenseRef       license, 
                                const struct tm **  expiration,
                                FlcErrorRef         error);

/**
 *  Gets start date of the license
 *
 *  @param  license    License object of interest 
 *  @param  startDate  Pointer to variable to receive returned license start date
 *                     (START value) if present, NULL otherwise
 *  @param  error        Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If the license was acquired from more than one feature the earliest expiration 
 *        date among the features used to satisfy the acquisition request is returned.
 */
FlcBool FlcLicenseGetStartDate(FlcLicenseRef       license, 
                               const struct tm **  startDate,
                               FlcErrorRef         error);

/**
 *  Returns whether or not the license is perpetual
 *
 *  @param  license      License object of interest 
 *  @param  isPerpetual  Pointer to variable to receive returned perpetual indicater which
 *                       is set to FLC_TRUE if the license is perpetual, and FLC_FALSE if 
 *                       the license contains a real expiration date
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   This is a convenience function. The same determination can be made by calling
 *          FlcDateIsPerpetual on the value returned from FlcLicenseGetExpiration.
 */
FlcBool FlcLicenseIsPerpetual(FlcLicenseRef       license,
                              FlcBool *           isPerpetual,
                              FlcErrorRef         error);

/**
 *  Gets a value representing the non-reusable metered feature undo interval in seconds for
 *  the acquired feature in the provided license.
 *
 *  @param  license      License object of interest 
 *  @param  undoSeconds  Pointer to variable to receive the license feature undo interval
 *                       (MODEL=metered UNDO_INTERVAL=value) in seconds or 0 if the feature
 *                       is not metered or is metered reusable and on error.
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 */
FlcBool FlcLicenseGetMeteredUndoInterval(FlcLicenseRef     license,
                                         FlcUInt32 *       undoSeconds,
                                         FlcErrorRef       error);

/**
 *  Gets a value representing the number of seconds remaining in the non-reusable metered feature
 *  undo interval for the acquired feature in the provided license.
 *
 *  @param  license               License object of interest 
 *  @param  undoSecondsRemaining  Pointer to variable to receive the number of seconds remaining
 *                                in the license feature undo interval (MODEL=metered UNDO_INTERVAL=value).
 *                                Returns 0 if the feature has no undo interval or if the undo 
 *                                interval has expired and on error.
 *  @param  error                   Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 */
FlcBool FlcLicenseGetSecondsLeftInUndoInterval(FlcLicenseRef     license,
                                               FlcUInt32 *       undoSecondsRemaining,
                                               FlcErrorRef       error);

/**
 *  Gets a value indicating whether the license is stale. A license can become stale if a capability 
 *  response is processed and there is no longer a sufficient available count of the feature id used
 *  to satisfy this license request available in trusted storage.
 *
 *  @param  license     License object of interest 
 *  @param  isStale     Pointer to variable to receive returned stale indicater which is set to FLC_TRUE
 *                      if the license is stale, and FLC_FALSE otherwise.
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 */
FlcBool FlcLicenseIsStale(FlcLicenseRef       license,
                          FlcBool *           isStale,
                          FlcErrorRef         error);


/** @} */ /* end of license group */
/** @} */ /* end of addtogroup licensingClient */

#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_LICENSE_H_ */
